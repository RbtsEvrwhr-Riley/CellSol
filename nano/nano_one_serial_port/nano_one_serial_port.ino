/*********
  Cellular-Solar (CellSol) is a simple interconnect between lora, wifi and com port(s). It is intended to be used for infrastructure-independent comms during or after a disaster.
  (c) 2020 Robots Everywhere, LLC until we are ready to release it under copyleft
  Written by M K Borri. Thanks to Rui Santos for the tutorials. Thanks to Jerry Jenkins for the inspiration. Thanks to Lisa Rein for initiating the project.  Thanks to Robots Everywhere for infrastructure support https://www.robots-everywhere.com
  Originally produced as part of the Aaron Swartz Day project https://www.aaronswartzday.org
  Distributed independently https://www.f3.to/cellsol

  This file is used for Atmel328-based boards (Arduino Uno, Mini, Nano, etc).
*********/

// General-purpose CellSol node
#define DEVICETYPE_START 'a'
#define VERSIONSTRING "0.37"

// Default is 2 MHz / 9600 baud; with GO_A_LOT_SLOWER it is 2 MHz / 9600 baud. Compile/upload for the correct 8 or 16 MHz Nano oscillator.
// #define GO_A_LOT_SLOWER // useful with a small solar panel; Bluetooth draws more power
#define USE_BATTERY_NOISE_FOR_ID // if undefined, same id across powerups. if not, use the last 2 bits as noise.
//#define USE_THIS_ID "abcd" // Optional fixed node ID. Otherwise use UniqueID/EEPROM.
//#define RSSI_TEST // for rssi test

#define APPLICATION // enable simple local pin automation via serial commands
#define BASICPINS
#define LORA_TX_POWER 20 // dBm. Use 14 for a weaker transmitter / smaller power budget.

#define RECALLSIZE 251 // how many bytes to save? (must be <256 sorry)
#define RECALL_STRAY_CHARS // trim to packet, or keep last packet that went away?
#define CONDENSE_TAGS // if it's the same person messaging more than once, condense messages in memory. A bit slow.

//#define SEND_TWICE // if defined, allow sending a packet twice after a pseudorandom delay, in case the first one got lost
#define RSSI_TRE_LO -120 // if sendtwice is defined, below this (for the last 4 received packets), turn on sendtwice
#define RSSI_TRE_HI -100 // if sendtwice is defined, above this (for the last 4 received packets), turn off sendtwice

#define DO_NOT_LOG_SYSTEM_PACKETS
#define REBROADCAST_DISASTER_RADIO_PACKETS
//#define DISPLAY_DISASTER_RADIO_PACKETS

#define VOLTAGECUTOFF 3000//3150 // if defined, system will go back to sleep if the system voltage is less than this.

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>
#include <Prescaler.h>
#include <LowPower.h>
#include <avr/wdt.h>
#include <ArduinoUniqueID.h>
#include <EEPROM.h>

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
// Existing 0.37d initializes LoRa *after* selecting SF9 / 62.5kHz, which
// resets those registers. SF7 / 125kHz keeps compatibility with that on-air
// behavior and with the current ESP32 CellSol firmware.
#define CELLSOL_LORA_SF 7
#define CELLSOL_LORA_BW_HZ 125000L
#define REQUIRE_TAG_FOR_REBROADCAST // if defined, require xxxx: tag for rebroadcast.
//#define REQUIRE_TAG_FOR_REBROADCAST_STRICT // on top of that, the first four characters must be hex digits.
#define BAD_CHARACTERS_MAX_DIVIDER 10
//#define SHOW_RSSI // if enabled, show RSSI for wireless packets coming in.

// Repeat sending the last message until you hear it back.
//#define REPEAT_UNTIL_ACK
//#define REPEAT_DELAY 500 // delay in ms

#ifdef REPEAT_UNTIL_ACK
boolean got_lora_ack = false;
long last_repeat_time = millis();
String message_to_repeat = "";
#endif

//packet counter
byte charcounter = 0;
boolean readytosend = false;

const char devicetype = DEVICETYPE_START;

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

#ifdef APPLICATION
void RunApplication();
bool HandleApplicationCommand(Stream &st, const char *command);
#endif

unsigned long psm = 0;
void PetTheWatchdog() {
  if ((int32_t)(millis() - psm) >= 0)
  {
    wdt_reset();
#ifdef GO_A_LOT_SLOWER
    psm = psm + 62; // 1000/16
#else
    psm = psm + 250; // 1000/4
#endif
    pseudoseconds++;
#ifdef APPLICATION
    RunApplication();
#endif
    //    Serial.println(pseudoseconds);
  }
}


String hextag = "XXXX:"; // usually the last two IP octets; gives pseudonimity to sender
String fourhex(int num)
{
#ifdef USE_THIS_ID
  return String(USE_THIS_ID);
#else
  // Only honor a programmed EEPROM tag if all four bytes are hex digits.
  // An uninitialized/partly written EEPROM must not produce an invalid ID.
  String saved = "";
  for (byte i = 3; i <= 6; i++) {
    const char c = (char)EEPROM.read(i);
    if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
          (c >= 'A' && c <= 'F'))) {
      saved = "";
      break;
    }
    saved += c;
  }
  if (saved.length() == 4) return saved;
  num = (num | 0x1000) & 0xFFFF;
  return String(num, HEX);
#endif
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
  if (result == 0) return 0;
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}
int vcc = 0;

void(* resetFunc) (void) = 0;//declare reset function at address 0


void setup() {
  MCUSR = 0;
  wdt_disable();
  delay(10);
  // sort-of-fix for old arduino bootloader on cheapie pro minis etc.
  // to flash if it doesn't want to: do not power from arduino plug, start flashing process,
  // and turn the arduino on and off in time with flashing retries until it catches
  // it has to be turned on/off: reset won't be sufficient


  LastWeGot.reserve(RECALLSIZE + 6);
  LoRaData.reserve(MAXPKTSIZE + 6);
  sendstr.reserve(MAXPKTSIZE + 6);
  vcc = readVcc();

#ifdef USE_BATTERY_NOISE_FOR_ID
  hextag = fourhex(UniqueID8[6] * 256 + ((UniqueID8[7] & 15) + ((UniqueID8[7] + vcc) & 240))) + TAG_END_SYMBOL; // alter it slightly each time the note is powered on to allow proper pseudonimity // never changes, so run it at setup and leave it alone.
#else
  hextag = fourhex(UniqueID8[6] * 256 + UniqueID8[7]) + TAG_END_SYMBOL; // never changes, so run it at setup and leave it alone.
#endif

  if (hextag.charAt(3) == '0') // this unit is not set up to do service tasks, so don't end in 0
    hextag.setCharAt(3, '1');

  // we want to run this at 0.5Mhz regardless if we are starting at 8 or 16.
#ifdef GO_A_LOT_SLOWER
#ifdef F_CPU
#if (F_CPU==16000000)
  setClockPrescaler(5); //0 == 16Mhz 1 == 8Mhz 2==4Mhz 3==2Mhz etc
  Serial.begin(76800); // actually 2400
  Serial.println(":SYS: TAG:" + hextag + " VCC:" + String(vcc) + devicetype + F("VER:" VERSIONSTRING " UPT:0 CLK:" " 16>0.5"));
#else
  setClockPrescaler(4); //0 == 8Mhz 1 == 4Mhz 2==2Mhz 3==1Mhz etc
  Serial.begin(38400); // actually 2400
  Serial.println(":SYS: TAG:" + hextag + " VCC:" + String(vcc) + devicetype + F("VER:" VERSIONSTRING " UPT:0 CLK:" "8>0.5"));
#endif
#else // assume 8Mhz
  setClockPrescaler(4); //0 == 8Mhz 1 == 4Mhz 2==2Mhz 3==1Mhz etc
  Serial.begin(38400); // actually 2400
  Serial.println(":SYS: TAG:" + hextag + " VCC:" + String(vcc) + devicetype + F("VER:" VERSIONSTRING " UPT:0 CLK:" " 8>0.5"));
#endif
#else
  // we want to run this at 2Mhz regardless if we are starting at 8 or 16.
#ifdef F_CPU
#if (F_CPU==16000000)
  setClockPrescaler(3); //0 == 16Mhz 1 == 8Mhz 2==4Mhz 3==2Mhz etc
  Serial.begin(76800); // actually 9600
  Serial.println(":SYS: TAG:" + hextag + " VCC:" + String(vcc) + devicetype + F("VER:" VERSIONSTRING " UPT:0 CLK:" " 16>2"));
#else
  setClockPrescaler(2); //0 == 8Mhz 1 == 4Mhz 2==2Mhz 3==1Mhz etc
  Serial.begin(38400); // actually 9600
  Serial.println(":SYS: TAG:" + hextag + " VCC:" + String(vcc) + devicetype + F("VER:" VERSIONSTRING " UPT:0 CLK:" " 8>2"));
#endif
#else // assume 8Mhz
  setClockPrescaler(2); //0 == 8Mhz 1 == 4Mhz 2==2Mhz 3==1Mhz etc
  Serial.begin(38400); // actually 9600
  Serial.println(":SYS: TAG:" + hextag + " VCC:" + String(vcc) + devicetype + F("VER:" VERSIONSTRING " UPT:0 CLK:" " 8>2"));
#endif
#endif

  SPI.setClockDivider(SPI_CLOCK_DIV2); // use the named divider, not raw register bits
  //initialize Serial Monitor
  //SPI LoRa pins
  SPI.begin();//SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST);//, DIO0);
  //  LoRa.crc(); // we would rather get a damaged packet than no packet at all; maybe some of it will be readable.

  wdt_enable(WDTO_8S); // note that this is independent of clock speed.

  // stuff broke
  if (!LoRa.begin(BAND)) {
    Serial.println(F("Starting LoRa failed! Resetting."));
    wdt_disable();
    mydelay(200);
    resetFunc(); //call reset
  }
  // LoRa.begin() resets the SX127x registers. Configure radio AFTER begin.
  LoRa.setSpreadingFactor(CELLSOL_LORA_SF);
  LoRa.setSignalBandwidth(CELLSOL_LORA_BW_HZ);
  LoRa.setTxPower(LORA_TX_POWER, PA_OUTPUT_PA_BOOST_PIN);

#ifdef ANNOUNCE
#ifdef GO_A_LOT_SLOWER
  LoraSendAndUpdate(F("RPT UP(2400)\r\n"));
#else
  LoraSendAndUpdate(F("RPT UP(9600)\r\n"));
#endif
#endif

}

bool recalldots = false;

inline bool IsHex(char c);

void PrintNodeStatus(Stream &st)
{
  st.println(":SYS: TAG:" + hextag + " VCC:" + String(vcc) + " " + devicetype +
             " VER:" VERSIONSTRING " UPT" + (broadcast_twice ? ";" : ":") +
             String(pseudoseconds) + " MEM:");
  if (LastWeGot.length() > 0) {
    if (recalldots) st.print(F("....."));
    st.println(LastWeGot);
  }
}

void SetNodeIDFromSerial(Stream &st, const char *newID)
{
#ifdef USE_THIS_ID
  st.println(F(":SYS:ID_FIXED_IN_FIRMWARE"));
#else
  // The four-byte EEPROM tag at offsets 3..6 is a legacy protocol feature.
  // Keep both the storage layout and the on-air change notice unchanged.
  for (byte i = 0; i < 4; i++) {
    if (!IsHex(newID[i])) {
      st.println(F(":SYS:INVALID_ID"));
      return;
    }
  }
  if (newID[3] == '0') {
    st.println(F(":SYS:ID_ENDING_0_RESERVED"));
    return;
  }
  const String oldID = hextag;
  for (byte i = 0; i < 4; i++) EEPROM.update(i + 3, newID[i]);
  hextag = String(newID).substring(0, 4) + TAG_END_SYMBOL;
  st.println(":SYS:TAG_CHANGED " + oldID + " -> " + hextag);
  LoraSendAndUpdate(oldID + ">>>" + hextag.substring(0, 4));
#endif
}

void ReadFromStream(Stream &st, char buf[], byte &cnt, bool &sendout)
{
  static bool droppingLongLine = false;
  if (sendout) return; // finish pending LoRa transmission before reading more
  while (st.available() > 0) {
    const int incoming = st.read();
    if (incoming < 0) break;
    const char c = (char)incoming;
    if (c == '\r' || c == '\n') {
      if (droppingLongLine) {
        droppingLongLine = false;
        cnt = 0;
        buf[0] = '\0';
        continue;
      }
      if (cnt == 0) continue;
      buf[cnt] = '\0';
      const bool special = cnt >= 3 && buf[0] == ',' && buf[2] == ',' &&
                           (buf[1] == ',' || buf[1] == '.');
      if (special) {
        if (cnt == 3) {
          PrintNodeStatus(st);
        } else if (cnt == 7 && IsHex(buf[3]) && IsHex(buf[4]) &&
                   IsHex(buf[5]) && IsHex(buf[6])) {
          SetNodeIDFromSerial(st, buf + 3);
        } else {
#ifdef APPLICATION
          HandleApplicationCommand(st, buf + 3);
#else
          st.println(F(":SYS:UNKNOWN_COMMAND"));
#endif
        }
        cnt = 0;
        buf[0] = '\0';
        continue;
      }
      sendout = true;
      return;
    }
    if (droppingLongLine) continue;
    if (cnt == 0 && (incoming < 6 || incoming > 127)) continue;
    if (cnt >= MAXPKTSIZE - 1) {
      droppingLongLine = true;
      cnt = 0;
      buf[0] = '\0';
      st.println(F(":SYS:RX_TOO_LONG"));
      continue;
    }
    buf[cnt++] = c;
  }
}

long LastToAddChk0 = 0;
long LastToAddChk1 = 0;
String lasttag = "xxxx:";
void AddToLastAndPrune(String st) { // warning: may modify st
  LastToAddChk1 = LastToAddChk0;
  LastToAddChk0 = LongChecksum(st);
  if (LastToAddChk0 == LastToAddChk1)
    return;
  bool addenter = true;
#ifdef CONDENSE_TAGS
  if (st.length() >= 5 && st.startsWith(lasttag)) // condense complete tags
  {
    st.remove(0, 5);
    addenter = false;
  }
  else
  {
    lasttag = st.substring(0, 5);
  }
#endif

  const unsigned int incomingLength = st.length() + 2;
  if (incomingLength > RECALLSIZE)
    st = st.substring(incomingLength - RECALLSIZE);
  if ((LastWeGot.length() + st.length() + 2) > RECALLSIZE)
  {
#ifdef RECALL_STRAY_CHARS
    const unsigned int toRemove = LastWeGot.length() + st.length() + 2 - RECALLSIZE;
    LastWeGot.remove(0, toRemove);
    recalldots = true;
#else
    while (LastWeGot.length() + st.length() + 2 > RECALLSIZE) {
      int newline = LastWeGot.indexOf('\n');
      if (newline < 0) {
        LastWeGot = "";
        break;
      }
      LastWeGot.remove(0, newline + 1);
    }
#endif
  }
  if (addenter)
    LastWeGot = LastWeGot + "\r\n" + st;
  else
    LastWeGot = LastWeGot + "~ " + st;
  LastWeGot.trim();
}

#ifdef SEND_TWICE
int RSSI_0 = RSSI_TRE_HI; //start neutral
int RSSI_1 = RSSI_TRE_HI; //start neutral
int RSSI_2 = RSSI_TRE_HI; //start neutral
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

    // Read bytes first. Disaster Radio datagrams are binary and can contain
    // NULs, so String/trim() must never touch them before forwarding.
    static uint8_t packet[255];
    int len = 0;
    while (LoRa.available() && len < (int)sizeof(packet)) {
      const int c = LoRa.read();
      if (c < 0) break;
      packet[len++] = (uint8_t)c;
    }
    if (len != packetSize) return;

#ifdef REBROADCAST_DISASTER_RADIO_PACKETS
    // TTL, length, sender/receiver, sequence, source, hopCount, metric, data.
    if (len > 16 && packet[1] == len && packet[0] > packet[15]) {
      packet[15]++; // leave the rest of the binary datagram unchanged
      LoRa.beginPacket();
      LoRa.write(packet, len);
      LoRa.endPacket();
#ifdef SEND_TWICE
      if (broadcast_twice) {
        mydelay(pseudoseconds & 7);
        LoRa.beginPacket();
        LoRa.write(packet, len);
        LoRa.endPacket();
      }
#endif
#ifdef DISPLAY_DISASTER_RADIO_PACKETS
      String drstring = String("(DR):") + String(packet[2], HEX) + ":";
      for (int i = 16; i < len && i < 80; i++)
        if (packet[i] >= 32 && packet[i] < 127) drstring += (char)packet[i];
      Serial.println(drstring);
      AddToLastAndPrune(drstring);
#endif
      return;
    }
#endif

    if (len >= MAXPKTSIZE) return; // text messages must fit the local buffer
    LoRaData = "";
    for (int i = 0; i < len; i++) {
      if (packet[i] == 0) return; // not a text message
      LoRaData += (char)packet[i];
    }
    LoRaData.trim();
#ifdef REPEAT_UNTIL_ACK
    if (LoRaData.equals(message_to_repeat)) got_lora_ack = true;
#endif
    if (!FilterIncomingLoRa()) return;
#ifdef SHOW_RSSI
    if (rssi < -99) Serial.print(String(rssi) + ":");
    else Serial.print(" " + String(rssi) + ":");
#endif
    Serial.println(LoRaData);
    if (LoRaData.indexOf(":SYS:") == -1) LoraSendAndUpdate(LoRaData);
    AddToLastAndPrune(LoRaData);
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
  byte lenlen = LoRaData.length();

#ifdef REQUIRE_TAG_FOR_REBROADCAST
  if (lenlen < 5) // too short
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
  if (lenlen < 2) // too short
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
  for (i = 0; i < lenlen; i++)
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
#ifdef REPEAT_UNTIL_ACK
  got_lora_ack = false;
  last_repeat_time = millis();
  message_to_repeat = whattosend;
#endif
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
    if (sendstr.length() > MAXPKTSIZE)
      sendstr.remove(MAXPKTSIZE);

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

#ifdef APPLICATION
// Do not drive the UART pins (D0/D1), LoRa reset (D7), or SPI (D10-D13).
bool SafeDigitalPin(byte pin) {
  return (pin >= 2 && pin <= 6) || pin == 8 || pin == 9;
}

int GetAnalogPin(char digit) {
  switch (digit) {
    case '0': return A0;
    case '1': return A1;
    case '2': return A2;
    case '3': return A3;
    case '4': return A4;
    case '5': return A5;
#if (defined(NUM_ANALOG_INPUTS) && NUM_ANALOG_INPUTS > 6) || defined(A6)
    case '6': return A6;
#endif
#if (defined(NUM_ANALOG_INPUTS) && NUM_ANALOG_INPUTS > 7) || defined(A7)
    case '7': return A7;
#endif
    default: return -1;
  }
}

char appReadKind = 0; // A: analog, R: digital, 0: no periodic read
byte appReadPin = 0;
char appReadLabel = '0';

void RunApplication() {
#ifdef BASICPINS
  if (appReadKind == 'A') {
    Serial.print(F(":SYS:APP:A"));
    Serial.print(appReadLabel);
    Serial.print(':');
    Serial.println(analogRead(appReadPin));
  } else if (appReadKind == 'R') {
    Serial.print(F(":SYS:APP:R"));
    Serial.print(appReadLabel);
    Serial.print(':');
    Serial.println(digitalRead(appReadPin));
  }
#endif
}

bool HandleApplicationCommand(Stream &st, const char *command) {
#ifdef BASICPINS
  const char operation = command[0];
  const char digit = command[1];
  const char value = command[2];
  if ((operation == 'D' || operation == 'd') && digit && value && !command[3]) {
    const int pin = digit - '0';
    if (!SafeDigitalPin(pin) || (value != '0' && value != '1' && value != digit)) {
      st.println(F(":SYS:APP:INVALID_DIGITAL_COMMAND"));
      return true;
    }
    const bool high = value == '1' || value == digit; // accept legacy D22 = D2 HIGH
    pinMode(pin, OUTPUT);
    digitalWrite(pin, high ? HIGH : LOW);
    st.print(F(":SYS:APP:D"));
    st.print(pin);
    st.println(high ? F(":1") : F(":0"));
    return true;
  }
  if ((operation == 'A' || operation == 'a' || operation == 'R' || operation == 'r') &&
      digit && !value) {
    const int pin = digit - '0';
    const bool analog = operation == 'A' || operation == 'a';
    const int actualPin = analog ? GetAnalogPin(digit) : pin;
    if (actualPin < 0 || (!analog && !SafeDigitalPin(pin))) {
      st.println(F(":SYS:APP:INVALID_READ_PIN"));
      return true;
    }
    appReadKind = analog ? 'A' : 'R';
    appReadPin = actualPin;
    appReadLabel = digit;
    // Reading an output must not silently reconfigure it as an input.
    RunApplication(); // one immediate reading, then once per pseudo-second
    return true;
  }
  if ((operation == 'X' || operation == 'x') && !digit) {
    appReadKind = 0;
    st.println(F(":SYS:APP:READ_STOPPED"));
    return true;
  }
#endif
  st.println(F(":SYS:APP:UNKNOWN_COMMAND"));
  return true;
}
#endif

int numloops = 9999;

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

#ifdef RSSI_TEST
unsigned int rssi_test_timer_a;
unsigned int rssi_test_timer_b;
#endif

void loop() {
  //Serial.println(availableMemory());
  PetTheWatchdog();
  SeeIfAnythingOnRadio();
#ifdef REPEAT_UNTIL_ACK
  if (!got_lora_ack)
  {
    if ((millis() - last_repeat_time) > REPEAT_DELAY and message_to_repeat.length() > 1)
    {
      LoraSendAndUpdate(message_to_repeat);
    }
  }
#endif


  ReadFromStream(Serial, receivedChars, charcounter, readytosend); // add other streams as needed.
  if (SendSerialIfReady())
  {
    LowPower.idle(SLEEP_30MS, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF); // multiply sleep times by 4. safe to turn UART off for a bit since we just sent a message so it's unlikely we will be sending another one so soon
  }

  if (++numloops > 10000)
  {
    vcc = readVcc();
#ifdef VOLTAGECUTOFF
    while (vcc < VOLTAGECUTOFF)
    {
      Serial.print(":SYS:LOWPWR!");
      Serial.println(vcc);
      Serial.flush();
      LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF); // go to sleep and hope that the situation has improved when we wake up; can't really do much else
      vcc = readVcc();
    }
#endif
    numloops = 0;
  }
  else
  {
    mydelay(4); // actually 16
  }

#ifdef RSSI_TEST
  if (pseudoseconds > rssi_test_timer_a)
  {
    pinMode(A6, INPUT_PULLUP);
    rssi_test_timer_a = pseudoseconds + 4;
    Serial.println(analogRead(A6));
    if (analogRead(A6) > 2)
    {
      LoraSendAndUpdate(hextag + "Hello from tester " + String(rssi_test_timer_b++));
    }
  }
#endif


}
