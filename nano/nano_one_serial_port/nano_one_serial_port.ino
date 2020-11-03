/*********
  Cellular-Solar (CellSol) is a simple interconnect between lora, wifi and com port(s) by spiritplumber. Intended to be used to allow for comms after a natural disaster.
  (c) 2020 Robots Everywhere, LLC until we are ready to release it under copyleft
  Written by Riley August (HTML, CSS), and spiritplumber (skeleton). Thanks to Rui Santos for the tutorials. Thanks to Jerry Jenkins for the inspiration
*********/

#define VERSIONSTRING "0.24"

//  Actual running speed is 2Mhz. BE SURE TO SET THE SPEED CORRECTLY FOR YOUR ARDUINO WHEN PROGRAMMING THIS. Nothing bad happens if you get it wrong but it'll run at the wrong baud rate (4800 or 19200).
//#define GO_A_LOT_SLOWER // if defined, operate at 0.5Mhz, and keeps serial port running at 2400bps, further slows down processing. Useful for drone-droppable pylons that need a small panel. not recommended for bluetooth use since the bluetooth module will eat the most anyway.
#define USE_BATTERY_NOISE_FOR_ID // if undefined, same id across powerups. if not, use the last 2 bits as noise.

#define RECALLSIZE 251 // how many bytes to save? (must be <256 sorry)
#define RECALL_STRAY_CHARS // trim to packet, or keep last packet that went away?
#define CONDENSE_TAGS // if it's the same person messaging more than once, condense messages in memory

#define SEND_TWICE // if defined, allow sending a packet twice after a pseudorandom delay, in case the first one got lost
#define RSSI_TRE_LO -130 // if sendtwice is defined, below this (for the last 4 received packets), turn on sendtwice
#define RSSI_TRE_HI -100 // if sendtwice is defined, above this (for the last 4 received packets), turn off sendtwice

#define DO_NOT_LOG_SYSTEM_PACKETS

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>
#include <Prescaler.h>
#include <LowPower.h>
#include <avr/wdt.h>
#include <ArduinoUniqueID.h>

//define the pins used by the LoRa transceiver module
// it's pretty much how you wire the whole thing, too
#define SCK 13
#define MISO 12
#define MOSI 11
#define SS 10
#define RST 7 // 8 and 9 are used by altsoftserial
//#define DIO0 8 // not used right now?
//#define ANNOUNCE // tell the world we are alive

#define TAG_END_SYMBOL ':'
//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 9151E5 //Mhz just to stay away from center band
#define REQUIRE_TAG_FOR_REBROADCAST // if defined, require xxxx: tag for rebroadcast.
//#define REQUIRE_TAG_FOR_REBROADCAST_STRICT // on top of that, the first four characters must be hex digits.
#define BAD_CHARACTERS_MAX_DIVIDER 10
//#define SHOW_RSSI // if enabled, show RSSI for wireless packets coming in.

//packet counter
byte charcounter = 0;
boolean readytosend = false;

String LoRaData = ""; // god this is lazy. change to character array please
String LastWeGot = ""; // god this is lazy. change to character array please
String sendstr = "";

long LastThingISentViaLora_3 = 0; // this is now a checksum
long LastThingISentViaLora_2 = 0; // this is now a checksum
long LastThingISentViaLora_1 = 0; // this is now a checksum
long LastThingISentViaLora_0 = 0; // this is now a checksum
long LTISVL_3_time = 0;
long LTISVL_2_time = 0;
long LTISVL_1_time = 0;
long LTISVL_0_time = 0;


bool broadcast_twice = false;

#define MAXPKTSIZE 200
char receivedChars[MAXPKTSIZE]; // an array to store the received data
int rssi; // last packat received rss

unsigned long pseudoseconds = 0;
long UTC_Seconds = -1;

unsigned long psm = 0;
void PetTheWatchdog() {
  if (millis() > psm)
  {
    wdt_reset();
#ifdef GO_A_LOT_SLOWER
    psm = psm + 62; // 1000/16
#else
    psm = psm + 250; // 1000/4
#endif
    pseudoseconds++;
    //    Serial.println(pseudoseconds);
  }
}


String hextag = "XXXX:"; // usually the last two IP octets; gives pseudonimity to sender
String fourhex(int num)
{
  if (num < 0x10)
    return "000" + String(num, HEX);
  if (num < 0x100)
    return "00" + String(num, HEX);
  if (num < 0x1000)
    return "0" + String(num, HEX);
  return String(num % 65536, HEX);
}



inline void mydelay(int num) __attribute__((always_inline));
void mydelay(int num)
{
#ifdef GO_A_LOT_SLOWER
  delay((num / 4) | 1);
#else
  delay(num);
#endif
}

// This is currently unused, but we may want to use it in order to get battery level. The arduino pylon is very cheap to run, so we shouldn't have to worry about it. Even so...
long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0) ;
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif

  mydelay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both

  long result = (high << 8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

int vcc = 0;

void(* resetFunc) (void) = 0;//declare reset function at address 0


void setup() {

  LastWeGot.reserve(RECALLSIZE + 6);
  LoRaData.reserve(MAXPKTSIZE + 6);
  sendstr.reserve(MAXPKTSIZE + 6);
  vcc = readVcc();

#ifdef USE_BATTERY_NOISE_FOR_ID
  hextag = fourhex(UniqueID8[6] * 256 + ((UniqueID8[7] & 15) + ((UniqueID8[7] + vcc) & 240))) + TAG_END_SYMBOL; // alter it slightly each time the note is powered on to allow proper pseudonimity // never changes, so run it at setup and leave it alone.
#else
  hextag = fourhex(UniqueID8[6] * 256 + UniqueID8[7]) + TAG_END_SYMBOL; // never changes, so run it at setup and leave it alone.
#endif

  // we want to run this at 0.5Mhz regardless if we are starting at 8 or 16.
#ifdef GO_A_LOT_SLOWER
#ifdef F_CPU
#if (F_CPU==16000000)
  setClockPrescaler(5); //0 == 16Mhz 1 == 8Mhz 2==4Mhz 3==2Mhz etc
  Serial.begin(76800); // actually 2400
  Serial.println(":SYS: TAG:" + hextag + " VCC:" + String(vcc) + F(" VER:" VERSIONSTRING " UPT:0 CLK:" " 16>0.5"));
#else
  setClockPrescaler(4); //0 == 8Mhz 1 == 4Mhz 2==2Mhz 3==1Mhz etc
  Serial.begin(38400); // actually 2400
  Serial.println(":SYS: TAG:" + hextag + " VCC:" + String(vcc) + F(" VER:" VERSIONSTRING " UPT:0 CLK:" "8>0.5"));
#endif
#else // assume 8Mhz
  setClockPrescaler(4); //0 == 8Mhz 1 == 4Mhz 2==2Mhz 3==1Mhz etc
  Serial.begin(38400); // actually 2400
  Serial.println(":SYS: TAG:" + hextag + " VCC:" + String(vcc) + F(" VER:" VERSIONSTRING " UPT:0 CLK:" " 8>0.5"));
#endif
#else
  // we want to run this at 2Mhz regardless if we are starting at 8 or 16.
#ifdef F_CPU
#if (F_CPU==16000000)
  setClockPrescaler(3); //0 == 16Mhz 1 == 8Mhz 2==4Mhz 3==2Mhz etc
  Serial.begin(76800); // actually 9600
  Serial.println(":SYS: TAG:" + hextag + " VCC:" + String(vcc) + F(" VER:" VERSIONSTRING " UPT:0 CLK:" " 16>2"));
#else
  setClockPrescaler(2); //0 == 8Mhz 1 == 4Mhz 2==2Mhz 3==1Mhz etc
  Serial.begin(38400); // actually 9600
  Serial.println(":SYS: TAG:" + hextag + " VCC:" + String(vcc) + F(" VER:" VERSIONSTRING " UPT:0 CLK:" " 8>2"));
#endif
#else // assume 8Mhz
  setClockPrescaler(2); //0 == 8Mhz 1 == 4Mhz 2==2Mhz 3==1Mhz etc
  Serial.begin(38400); // actually 9600
  Serial.println(":SYS: TAG:" + hextag + " VCC:" + String(vcc) + F(" VER:" VERSIONSTRING " UPT:0 CLK:" " 8>2"));
#endif
#endif

  SPI.setClockDivider(1); // spi abuse courtesy of the fact that we are running at 2Mhz anyway // SPI_CLOCK_DIV2
  //initialize Serial Monitor
  //SPI LoRa pins
  SPI.begin();//SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST);//, DIO0);

  wdt_enable(WDTO_8S); // note that this is independent of clock speed.

  // stuff broke
  if (!LoRa.begin(BAND)) {
    Serial.println(F("Starting LoRa failed! Resetting."));
    wdt_disable();
    mydelay(200);
    resetFunc(); //call reset
  }

#ifdef GO_A_LOT_SLOWER
  LoRa.setTxPower(14, 1 ); // tests show that this is necessary to run from a small panel, which is the only reason why you'd want to slow down anyway
#else
  LoRa.setTxPower(19, 1 ); // PABOOST; for RFO use 14,0
#endif

#ifdef ANNOUNCE
#ifdef GO_A_LOT_SLOWER
  LoraSendAndUpdate(F("RPT UP(2400)\r\n"));
#else
  LoraSendAndUpdate(F("RPT UP(9600)\r\n"));
#endif
#endif

}

bool recalldots = false;

void ReadFromStream(Stream &st, char buf[], byte &cnt, bool &sendout)
{
  while (st.available() > 0)
  {
    buf[cnt++] = st.read();
    if (cnt > MAXPKTSIZE)
    {
      sendout = true;
      return;
    }
    if ((cnt > 1 and buf[cnt - 1] == 13))
    {
      sendout = true;
    }

    // eat ascii 255s that that show up
    if ((buf[0]<6 or buf[0]>127) and cnt == 1)
      cnt = 0;

    if (buf[0] == ',' and buf[2] == ',' and (buf[1] == ',' or buf[1] == '.') and (buf[3] == 13 or buf[3] == 10)) // special: send status string and memory
    {
      cnt = 0;
      buf[0] = 0;
      buf[1] = 0;
      buf[2] = 0;
      sendout = false;
      st.println(":SYS: TAG:" + hextag + " VCC:" + String(vcc) + " VER:" VERSIONSTRING " UPT"+(broadcast_twice?";":":") + String(pseudoseconds) + " MEM:"); // keep :SYS: TAG: same across hardware, or edit the bluetooth app to fit
      if (LastWeGot.length() > 0)
      {
        if (recalldots)
          st.print(F("....."));
        st.println(LastWeGot);
      }
    }
    if (buf[0] == 10 or buf[0] == 13) // eliminate stray RFs in case we get a CRLF, and don't send empty packets
    {
      cnt = 0;
      buf[0] = 0;
      buf[1] = 0;
      sendout = false;
    }
  }
}

int LastToAddChk0 = 0;
int LastToAddChk1 = 0;
String lasttag = "xxxx:";
void AddToLastAndPrune(String st) { // warning: may modify st
  LastToAddChk1 = LastToAddChk0;
  LastToAddChk0 = LongChecksum(st);
  if (LastToAddChk0 == LastToAddChk1)
    return;
  bool addenter = true;
#ifdef CONDENSE_TAGS
  if (st.startsWith(lasttag)) // condense tags
  {
    st.remove(0, 5);
    addenter = false;
  }
  else
  {
    lasttag = st.substring(0, 4);
  }
#endif

  if ((LastWeGot.length() + st.length() + 2) > RECALLSIZE)
  {
    LastWeGot.remove(0, st.length());
#ifdef RECALL_STRAY_CHARS
    recalldots = true;
#else
    LastWeGot.remove(0, LastWeGot.indexOf('\n')); // uncomment me for
#endif
  }
  if (addenter)
    LastWeGot = LastWeGot + "\r\n" + st;
  else
    LastWeGot = LastWeGot + "~ " + st;
  LastWeGot.trim();
}

#ifdef SEND_TWICE
int RSSI_0=RSSI_TRE_HI;//start neutral
int RSSI_1=RSSI_TRE_HI;//start neutral
int RSSI_2=RSSI_TRE_HI;//start neutral
#endif

void SeeIfAnythingOnRadio() {
  //see if there's anything on the radio, and if there is, be ready to send it
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    //received a packet
    rssi = LoRa.packetRssi();
//put the filter here so that it also evaluates garbage packets, in the hope of reducing said garbage later.
#ifdef SEND_TWICE
  RSSI_2 = RSSI_1;
  RSSI_1 = RSSI_0;
  RSSI_0 = rssi;
  if (RSSI_2 < RSSI_TRE_LO or RSSI_1 < RSSI_TRE_LO or RSSI_0 < RSSI_TRE_LO or rssi < RSSI_TRE_LO)
    broadcast_twice = true;
  else if (RSSI_2 > RSSI_TRE_HI and RSSI_1 > RSSI_TRE_HI and RSSI_0 > RSSI_TRE_HI and rssi > RSSI_TRE_HI)
    broadcast_twice = false;
#endif

    //read packet
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      LoRaData.trim();
      if (FilterIncomingLoRa())//(LoRaData.length() > 1) and (LoRaData.substring(1).equals(LastThingISentViaLora_0.substring(1)) == false))
      {
        
#ifdef SHOW_RSSI
        if (rssi < -99)
          Serial.println(String(rssi) + ":" + LoRaData);
        else
          Serial.println(" " + String(rssi) + ":" + LoRaData);
#else
        Serial.println(LoRaData);
#endif
        if (LoRaData.startsWith(":SYS:") == false) // avoid spamming
          LoraSendAndUpdate(LoRaData);
        AddToLastAndPrune(LoRaData);
      }
    }
  }
}

// decide whether to actually deal with this packet or not
inline bool IsValidChar(char i) {
  return (i == 13 || i == 10 || (i > 31 && i < 128));
}
inline bool IsHex(char i) {
  return ((i > 64 && i < 71) || (i > 96 && i < 103) || (i > 47 && i < 58)); // AF, af, 09
}
bool FilterIncomingLoRa() {
#ifdef REQUIRE_TAG_FOR_REBROADCAST
  if (LoRaData.length() < 5) // too short
    return false;
  if (LoRaData.charAt(0) == hextag.charAt(0) && LoRaData.charAt(1) == hextag.charAt(1) && LoRaData.charAt(2) == hextag.charAt(2))
    return false; // stop broadcast storms
#ifdef REQUIRE_TAG_FOR_REBROADCAST_STRICT
  if (IsHex(LoRaData.charAt(0)) == false || IsHex(LoRaData.charAt(1)) == false || IsHex(LoRaData.charAt(2)) == false || IsHex(LoRaData.charAt(3)) == false)
    return false;
#endif
  if (LoRaData.charAt(4) != TAG_END_SYMBOL) // not our format
    return false;
#else
  if (LoRaData.length() < 2) // too short
    return false;
#endif
  long chk = LongChecksum(LoRaData);
  TimeToForget(); // erase lastthing... after a fixed time; prevents broadcast storms
  if (chk == LastThingISentViaLora_0 or chk == LastThingISentViaLora_1 or chk == LastThingISentViaLora_2 or chk == LastThingISentViaLora_3)
    return false;

  if (LoRaData.charAt(3) == '0' and LoRaData.charAt(5) == 'U' and LoRaData.charAt(6) == 'T' and LoRaData.charAt(7) == 'C' and LoRaData.charAt(8) == ':') // Do a bunch of checks to make sure we're getting a good packet
  {
    if (LoRaData.charAt(9) == '1' or LoRaData.charAt(9) == '2')
    {
      if (LoRaData.charAt(14) > 47 && LoRaData.charAt(14) < 58)
      {
        UTC_Seconds = (LoRaData.charAt(9)  - '0') * 100000 + // display fix type
                      (LoRaData.charAt(10) - '0') * 10000 +
                      (LoRaData.charAt(11) - '0') * 1000 +
                      (LoRaData.charAt(12) - '0') * 100 +
                      (LoRaData.charAt(13) - '0') * 10 +
                      (LoRaData.charAt(14) - '0') * 1;

#ifdef DO_NOT_LOG_SYSTEM_PACKETS
        LoraSendAndUpdate(LoRaData); // send here, but don't cycle strings or output to serial(s)
        return false; // send here, but don't cycle strings or output to serial(s)
#endif
      }
    }
  }

    
  byte i = 0;
  byte testbyte = 0;
  for (i = 0; i < LoRaData.length(); i++)
  {
    if (IsValidChar(LoRaData.charAt(i)) == false)
      testbyte++;
  }
  if (testbyte > (i / BAD_CHARACTERS_MAX_DIVIDER))
  {
    return false;
  }
  // looks like we're good!
  return true;
}


#define ANTISPAMTIME 3 // in (pseudo) seconds. for radio
#define ANTISPAM_TIME_SERIAL 1 // for serial
void TimeToForget()
{
  PetTheWatchdog();
  LastThingISentViaLora_3 = ((pseudoseconds - LTISVL_3_time) > ANTISPAMTIME) ? 0 : LastThingISentViaLora_3;
  LastThingISentViaLora_2 = ((pseudoseconds - LTISVL_2_time) > ANTISPAMTIME) ? 0 : LastThingISentViaLora_2;
  LastThingISentViaLora_1 = ((pseudoseconds - LTISVL_1_time) > ANTISPAMTIME) ? 0 : LastThingISentViaLora_1;
  LastThingISentViaLora_0 = ((pseudoseconds - LTISVL_0_time) > ANTISPAMTIME) ? 0 : LastThingISentViaLora_0;
}

unsigned long antispam_timestamp = 0;
void LoraSendAndUpdate(String whattosend)
{
  PetTheWatchdog();

  LTISVL_3_time = LTISVL_2_time;
  LTISVL_2_time = LTISVL_1_time;
  LTISVL_1_time = LTISVL_0_time;
  LTISVL_0_time = pseudoseconds;
  LastThingISentViaLora_3 = LastThingISentViaLora_2;
  LastThingISentViaLora_2 = LastThingISentViaLora_1;
  LastThingISentViaLora_1 = LastThingISentViaLora_0;
  LastThingISentViaLora_0 = LongChecksum(whattosend);
  LoRa.beginPacket();
  LoRa.print(whattosend);
  LoRa.endPacket();
  if (broadcast_twice)
  {
    mydelay(pseudoseconds & 7);
    LoRa.beginPacket();
    LoRa.print(whattosend);
    LoRa.endPacket();
  }
}

// important: this should be copy/pasted exactly between hardware types.
long LongChecksum(String str)
{
  if (str.length() < 2)
    return -1;
  long ret = 0;
  for (byte i = 1; i < str.length() - 1; i++) // skip the first and last characters to allow for a bit of extra noise
  {
    int c = str.charAt(i);
    if (c > 31 && c < 128) // ignore invalid characters and also crlfs
      ret = ret + ((c * i) & 16777215); // mildly weird, but it catches transpositions, so sending "east" and "tase" isn't the same. 16777215 is $00FFFFFF
  }
  return ret;
}

bool SendSerialIfReady()
{
  // do actual sending; stay in send mode for as little as possible; this should be followed by the receive function; stagger these
  if (readytosend and (pseudoseconds > antispam_timestamp))
  {
    //Send LoRa packet to receiver
    sendstr = hextag + receivedChars;
    sendstr.replace('\n', ' ');
    sendstr.replace('\r', ' ');
    sendstr.replace("  ", " ");
    sendstr.trim();

    LoraSendAndUpdate(sendstr);
    AddToLastAndPrune(sendstr);
    readytosend = false;
    charcounter = 0;
    for (int i = 0; i < MAXPKTSIZE; i++)
    {
      receivedChars[i] = 0;
    }
    antispam_timestamp = pseudoseconds + ANTISPAM_TIME_SERIAL;
    return true;
  }
  return false;
}

int numloops = 0;

/*
  // free RAM check for debugging. SRAM for ATmega328p = 2048Kb.
  int availableMemory() {
    // Use 1024 with ATmega168
    int size = 2048;
    byte *buf;
    while ((buf = (byte *) malloc(--size)) == NULL);
        free(buf);
    return size;
  }
*/
void loop() {
  //Serial.println(availableMemory());
  PetTheWatchdog();
  SeeIfAnythingOnRadio();
  ReadFromStream(Serial, receivedChars, charcounter, readytosend); // add other streams as needed.
  if (SendSerialIfReady())
  {
    LowPower.idle(SLEEP_30MS, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF); // multiply sleep times by 4. safe to turn UART off for a bit since we just sent a message so it's unlikely we will be sending another one so soon
  }

  if (++numloops > 10000)
  {
    vcc = readVcc();
    numloops = 0;
  }
  else
  {
    mydelay(4); // actually 16
  }
}
