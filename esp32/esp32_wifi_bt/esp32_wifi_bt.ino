/*********
  Cellular-Solar (CellSol) is a simple interconnect between lora, wifi and com port(s). It is intended to be used for infrastructure-independent comms during or after a disaster.
  (c) 2020 Robots Everywhere, LLC until we are ready to release it under copyleft
  Written by Riley August (HTML/CSS/DHCP/Optimizations), and M K Borri (skeleton). Thanks to Rui Santos for the tutorials. Thanks to Jerry Jenkins for the inspiration. Thanks to Lisa Rein for initiating the project.
  Originally produced as part of the Aaron Swartz Day project https://www.aaronswartzday.org
*********/
#include "config.h"

#ifdef PROVIDE_APK
#include "btt/btt.h"
#endif

#ifdef THE_INTERNET_IS_MADE_OF_CATS
#include "btt/cat.h"
#endif

#ifdef YOU_ARE_EATING_RECURSION
#include "btt/src.h"
#endif

#include "website.cpp"
#include "watchdog.cpp"

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds. Do not change this. Seriously. */

// Load Wi-Fi library
#include <WiFi.h>
#include "time.h"
// taken from genericwifi.h -1dbm to 19.5dbm
#ifdef WIFI_POWER_LEVEL
const wifi_power_t wifilevels[] = {WIFI_POWER_MINUS_1dBm, WIFI_POWER_2dBm, WIFI_POWER_5dBm, WIFI_POWER_7dBm, WIFI_POWER_8_5dBm, WIFI_POWER_11dBm, WIFI_POWER_13dBm, WIFI_POWER_15dBm, WIFI_POWER_17dBm, WIFI_POWER_18_5dBm, WIFI_POWER_19dBm, WIFI_POWER_19_5dBm}; // 0~11
#endif

#include <DNSServer.h>
const byte DNS_PORT = 53;
#include <esp_wifi.h>
#include "BluetoothSerial.h"

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>


#include "esp32-hal-cpu.h"
#define CLKFREQ_HI 240 // megahertz when going as fast as possible
#define CLKFREQ 80 // megahertz when in full mode
#define CLKFREQ_LOW 20 // megahertz when in repeater mode; can't go any lower, annoyingly

#include "pinout.h" // LORA32 Pinout by default, change this file to change the pinout!

#ifndef NODISPLAY
//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif

static RTC_NOINIT_ATTR int lastbatterylevel = 0; // try to save battery levels between runs

static bool lowpowerstart = false;
static uint64_t chipid; // chip id stuff = ESP.getEfuseMac();
static uint16_t uptwo; // chip id stuff = (uint16_t)(chipid >> 32);
static uint32_t dnfor; // chip id stuff  = (uint32_t)(chipid);

// Wireless UART Configuration
long LastReconnect = 0;
int currbatterylevel = 0;
#define SSIDROOT "CellSol "
#define UART_BAUD_RATE 9600
String ssid = SSIDROOT;
String tempstring = ""; // internal function use only
// Set web server port number to 80
WiFiServer server(80);

DNSServer dnsServer;

WiFiClient client;


int batt_delta;
int battimeout = 0;
bool low_batt_announce = false;
int lploops = LPLOOP_BLINK - 10; // give me a blink early so i know we're alive
bool has_lora_been_initialized = false;
bool has_serial_been_initialized = false;
bool has_bluetooth_been_initialized = false;
bool has_wifi_been_initialized = false;
bool has_display_been_initialized = false;

String hextag  = "XXXX"; // usually the last two IP octets; gives pseudonimity to sender
String lasttagimade = "XXXX"; // last one we made for use in checking

byte derpme = 0; // third number of ip address
byte spare_id_nibble = 0; // upper nibble; lower nibble always 0

String fourhex(int num1, int num2)
{
  int num = (num1 * 256) + num2;
  if (num < 0x10)
    return "000" + String(num, HEX);
  if (num < 0x100)
    return "00" + String(num, HEX);
  if (num < 0x1000)
    return "0" + String(num, HEX);
  if (num > 0xFFFF)
    return String(num % 65536, HEX);
  return String(num, HEX);
}


IPAddress AP_IP(192, 168, 255, 1); // ip address of AP
IPAddress Client_IP(CLIENT_IP_ADDR);// this is the IP address we will be using.
IPAddress Client_Gateway(GATEWAY_IP_ADDR);// this is the IP address OF THE ROUTER
IPAddress Client_Subnet(GATEWAY_SUBNET);// usually 255,255,255,0 or 255,255,0,0
IPAddress lwc(255, 255, 255, 255); // ip address of current client
IPAddress IP(255, 255, 255, 255); // used to generate webpages
String ipstring;
String ipstring_c;
String ipstring_a;
String ipstring_b; // used in bluetooth mode
byte last_web_caller; // last web ip that said something
bool broadcast_twice = false;

void BuildNicknameTags()
{
  // use the chip id to generate a (hopefully) unique identifier
  chipid = ESP.getEfuseMac();
  uptwo = (uint16_t)(chipid >> 32);
  dnfor = (uint32_t)(chipid);
  dnfor = uptwo + dnfor; // 6 byte unique-ish ID

#ifdef USE_BATTERY_NOISE_FOR_ID
  spare_id_nibble = ((currbatterylevel + dnfor) & 3840) >> 4; // only generated once. third nibble, fourth nibble is 0 since we are using it for something. so 00~F0. 3840 is F00
#else
  spare_id_nibble = (dnfor & 3840) >> 4;  // third nibble, fourth nibble is 0 since we are using it for something. so 00~F0. 3840 is F00
#endif


  derpme = dnfor & 255;
  if (derpme == 255) // allows using 192.168.x.1 for the AP which saves a lot of config headaches
    derpme = 0;
  AP_IP[2] = derpme;
  ipstring_a = AP_IP.toString();
  ipstring = ipstring_a;
  ssid = SSIDROOT + ipstring;
  TEXT_ALIGN_STRING = centertext ? TEXT_ALIGN_STRING_A : TEXT_ALIGN_STRING_B;
}
bool is_watchdog_on = false;

String status_string()
{
  return (fourhex(derpme, spare_id_nibble) + TAG_END_SYMBOL + "(" + String(currbatterylevel) + "/" + String(batt_delta) + ") " + (is_watchdog_on ? "`" : ",") + String(pseudoseconds) + (broadcast_twice ? "`" : ",") );
}
/*
  String decodeHtml(String text)
  {
  text.replace("&amp;", "&");
  text.replace("&#034;", "\"");
  text.replace("&#039;","'");
  text.replace("&lt;","<");
  text.replace("&gt;",">");
  return text;
  }
  String encodeHtml(String text)
  {
  text.replace("&", "&amp;");
  text.replace("\"", "&#034;");
  text.replace("'", "&#039;");
  text.replace("<", "&lt;");
  text.replace(">", "&gt;");
  return text;
  }
*/

void ResetDisplayViaPin()
{
#ifndef NODISPLAY
  //  Wire.end();
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  for (byte i = 0; i < 20; i++)
    DoBasicSteps();
  digitalWrite(OLED_RST, HIGH);
  has_display_been_initialized = false;
  Wire.begin(OLED_SDA, OLED_SCL);
#endif
}

#ifdef BATT_ADC
void ReadBatteryADC(bool force)
{
  if (force || (millis() > battimeout))
  {
    Watchdog(true);
    if (currbatterylevel > 0)
      lastbatterylevel = currbatterylevel;
    currbatterylevel = (analogRead(BATT_ADC) + analogRead(BATT_ADC) + analogRead(BATT_ADC) + analogRead(BATT_ADC)) / 4;
    if (lastbatterylevel == 0)
      lastbatterylevel = currbatterylevel;
    batt_delta = currbatterylevel - lastbatterylevel;
    if (batt_delta > 100 or batt_delta < -100)
      batt_delta = 0; // unrealistic, discard.
    battimeout = millis() + ADC_INTERVAL; // should be less than that since these instructions take time, but meh
  }
}
#else
void  ReadBatteryADC(bool force)
{
  lastbatterylevel = FULL_BATT;
  currbatterylevel = FULL_BATT;
  batt_delta = 0;
}
#endif


#ifndef NODISPLAY
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
#endif

//packet counter
int txcounter = 0;
int rxcounter = 0;
byte charcounter = 0;
boolean readytosend = false;
byte charcounte2 = 0;
boolean readytosen2 = false;
boolean dodisplay = false;
boolean dodisplaybuf = false;
boolean displayexists = false;
bool displayenabled = true;

boolean enablecomport = false; // set to false for a solderless fix for UART buffer crosstalk hardware issue

String LoRaData;

// Variable to store the HTTP request
String header;

#ifdef COMMUNITY_MEMORY_SIZE
String communitymemory[COMMUNITY_MEMORY_SIZE];
#endif
String string_rx[10];

long LastThingISentViaLora_3 = 0;
long LastThingISentViaLora_2 = 0;
long LastThingISentViaLora_1 = 0;
long LastThingISentViaLora_0 = 0;
long LTISVL_3_time = 0;
long LTISVL_2_time = 0;
long LTISVL_1_time = 0;
long LTISVL_0_time = 0;

String gotstring = "";

// moves old strings out
void cyclestrings(String newone)
{
  newone.replace('\r', ' ');
  newone.replace('\n', ' ');//0xE2 0x80 0x8B
  newone.replace("<", "&lt;"); // prevent script injection; doing it here once is more efficient than doing it repeatedly.
  newone.replace("  ", " ");
  newone.trim();

#ifdef COMMUNITY_MEMORY_SIZE
  for (int i = COMMUNITY_MEMORY_SIZE; i > 1; i--)
  {
    communitymemory[i - 1] = communitymemory[i - 2];
  }
  communitymemory[0] = string_rx[9];
#endif
  for (int i = 10; i > 1; i--)
  {
    timestamp_rx[i - 1] = timestamp_rx[i - 2];
    string_rx[i - 1] = string_rx[i - 2];
  }

  timestamp_rx[0] = pseudoseconds;
  string_rx[0] = newone;
  dodisplay = true;
  DoDisplayIfItExists();
#ifdef user_button_display
  display_on_ping = false;
#else
  display_on_ping = true;
#endif
}



bool wifimode = true; // bluetooth or wifi


#ifdef USER_BUTTON_PIN
#else
#undef MODEFLIP_BUTTON
#endif

// if this exists, only show the display when we are pushing the button that way we know someone is actually looking at it (saves power)
#define user_button_display

char receivedChars[MAXPKTSIZE]; // an array to store the received data
char receivedChar2[MAXPKTSIZE]; // an array to store the received data
int rssi; // last packat received rssi

static char RTC_NOINIT_ATTR byteme[10][MAXPKTSIZE]; // save last x sentences in here for posterity

void TryStoreSentences()
{
  for (byte i = 0; i < 10; i++)
    string_rx[i].toCharArray(byteme[i], MAXPKTSIZE);
}
void TryRetrieveSentences()
{
  for (byte i = 0; i < 10; i++)
    string_rx[i] = String(byteme[i]);
}

BluetoothSerial ESP_BT;

void decode_in_place(char *s) {
  char *d = s;

  while (*s) {
    switch (*s) {
      case '+': *d++ = ' '; s++; break; // turn + into space
      case 13: *d++ = ' '; s++; break; // turn CR into space
      case 10: *d++ = ' '; s++; break; // turn LF into space
      case '%':
        s++; if (!*s) break; // handle malformed input
        *d = hexValue(*s++) << 4;
        if (!*s) break; // handle malformed input
        *d |= hexValue(*s++);
        d ++;
        break;
      default:
        *d++ = *s++; break;
    }
  }

  *d = '\0'; // always add the terminator
}

inline int hexValue(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  return 0;
}

char* string2char(String command) {
  if (command.length() != 0) {
    char *p = const_cast<char*>(command.c_str());
    return p;
  }
}

void led(boolean onoff)
{
#ifdef USER_LED_PIN
  digitalWrite(USER_LED_PIN, onoff);
#endif
}

long disptimeout = 0;
bool display_on_ping = false;

void displayonoff(bool onoff)
{
#ifdef NODISPLAY
  displayenabled = false;
#else
  if (onoff)
  {
    if (has_display_been_initialized == false)
      InitDisplayTryAll();
    display.ssd1306_command(SSD1306_DISPLAYON);
    displayenabled = true;
  }
  else
  {
    if (has_display_been_initialized)
    {
      display.clearDisplay();
      display.display();
      display.ssd1306_command(SSD1306_DISPLAYOFF);
    }
    displayenabled = false;
  }
#endif
}

#ifdef WIFI_IS_HYBRID
int displayswitch = 0; // display switch if we are in hybrid mode
#endif
void DoDisplayIfItExists()
{
#ifndef NODISPLAY
#ifdef user_button_display
  if (digitalRead(USER_BUTTON_PIN) == false or display_on_ping == true) // actually button pushed
#else
  if (display_on_ping == true)
#endif
  {
    if (has_display_been_initialized == false) // try without hard reset first
      InitDisplayTryAll();
    dodisplay = true;
    displayonoff(true);
    disptimeout = millis() + DISPLAY_INTERVAL;
    display_on_ping = false;
  }
  if (millis() > (disptimeout))
  {
    displayonoff(false);
    dodisplaybuf = false;
    dodisplay = false;
  }

  // either update the entire display, or just the buffer indicator (faster)
  if (dodisplay) // update entire display
  {

    if (displayexists and displayenabled)
    {

      display.clearDisplay();
      display.setCursor(81, 0);
      display.print(currbatterylevel);
      display.setCursor(110, 0);
      display.print(charcounter);
      display.setCursor(0, 0);
      if (wifimode)
      {
#ifdef WIFI_IS_CLIENT // getting noise here for some reason
#ifdef WIFI_IS_HYBRID
        display.println((++displayswitch % 2) ? ipstring_c : ipstring_a);
#else
        display.println(ipstring_c);
#endif
#else
        display.println(ipstring_a);
#endif
      }
      else
      {
        display.println(ipstring_b);
      }
      // print the last 3 lines we got, or at least the first 42 characters of each since it's what will fit.
      for (int i = 2; i > -1; i--)
      {
        display.setCursor(0, 45 - (i * 18));
        tempstring = string_rx[i];
        tempstring.replace("&lt;", "<");
        display.println(tempstring.substring(0, 42));
      }
      display.display();
    }
    dodisplaybuf = false;
    dodisplay = false;
  }
#endif
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
    delay(pseudoseconds % 23); // ironically the only place where we need a delay at this point
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

inline void Stop_LORA()
{
  if (has_lora_been_initialized)
  {
    SeeIfAnythingOnRadio();
    LoRa.end();
    SPI.end();
    has_lora_been_initialized = false;
  }
}

void SleepLowBatt()
{
  tempstring = ":SYS:SLEEP " + String(derpme) + ":" + status_string();
  Watchdog(false);
  led(true);
  if (low_batt_announce)
  {
    LoraSendAndUpdate(tempstring);
    cyclestrings(tempstring);
  }
  lastbatterylevel = currbatterylevel;
  lowpowerstart = true;
  displayonoff(false);
  led(false);

  displayonoff(false);

  //display.end();
  // turn off external devices
  Serial.println(tempstring);
  Serial.flush();
  Stop_LORA();
  Stop_BT();
  WiFi.mode(WIFI_OFF);
  server.stop();
  dnsServer.stop();
  TryStoreSentences();
  esp_deep_sleep_start();
}






void UpdateCharCounterIfDisplayExists()
{
#ifndef NODISPLAY
  if (has_display_been_initialized == false)
    InitDisplayTryAll();

  if ((dodisplay == false) and displayexists and displayenabled and dodisplaybuf)
  {
    display.fillRect(110, 0, 45, 11, BLACK); // upperleftx, upperlefty, width, height, color
    display.setCursor(110, 0);
    display.print(charcounter);
    display.display();
    dodisplaybuf = false;
  }
#endif
}

void ReadFromStream(Stream &st, char buf[], byte &cnt, bool &sendout, bool streamexists, int whichtag)
{
  if (streamexists)
  {
    while (st.available() > 0)
    {
      buf[cnt++] = st.read();
      UpdateCharCounterIfDisplayExists();
      if (cnt > MAXPKTSIZE)
      {
        dodisplay = true;
        sendout = true;
        return;
      }
      if ((cnt > 1 and buf[cnt - 1] == 13))
      {
        dodisplay = true;
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
        dodisplay = false;
        hextag = fourhex(derpme, spare_id_nibble + whichtag);
        st.println(":SYS: TAG:" + hextag + " BAT:" + String(currbatterylevel) + " VER:" VERSIONSTRING " UPT" + (broadcast_twice ? ";" : ":") + String(pseudoseconds) + " MEM:"); // keep :SYS: TAG: same across hardware, or edit the bluetooth app to fit
        if (string_rx[0].length() > 0)
        {
          for (int i = 10; i > 0; i--)
          {
            PetTheWatchdog();
            tempstring = string_rx[i - 1];
            tempstring.replace(";lt", "<");

            if (tempstring.length() > 0)
              st.println(tempstring);
          }
        }
      }

      if (buf[0] == 10 or buf[0] == 13 or buf[1] == 10 or buf[1] == 13) // eliminate stray RFs in case we get a CRLF, and don't send empty packets
      {
        cnt = 0;
        buf[0] = 0;
        buf[1] = 0;
        sendout = false;
        dodisplay = false;
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

  if (LoRaData.charAt(0) == hextag.charAt(0) && LoRaData.charAt(1) == hextag.charAt(1) && LoRaData.charAt(2) == hextag.charAt(2))
    return false; // stop broadcast storms
  if (LoRaData.startsWith(lasttagimade)) // failed rebroadcast attempt, so filter out
    return false;

#ifndef GPS_SERIAL_1 // if we already have our own gps, use it. otherwise do this
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
#endif

  long chk = LongChecksum(LoRaData);
  TimeToForget(); // erase lastthing... after a fixed time; prevents broadcast storms
  if (chk == LastThingISentViaLora_0 or chk == LastThingISentViaLora_1 or chk == LastThingISentViaLora_2 or chk == LastThingISentViaLora_3)
    return false;
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

#ifdef SEND_TWICE
    RSSI_2 = RSSI_1;
    RSSI_1 = RSSI_0;
    RSSI_0 = rssi;
#endif
    rssi = LoRa.packetRssi();
    //put the filter here so that it also evaluates garbage packets, in the hope of reducing said garbage later. the logic is that if i'm away from someone, they are away from me, and they may benefit from erxtra loudness.
#ifdef SEND_TWICE
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
          tempstring = String(rssi) + ":" + LoRaData;
        else
          tempstring = " " + String(rssi) + ":" + LoRaData;
        Serial.println(tempstring);
        cyclestrings(tempstring);
        if (has_bluetooth_been_initialized)
          ESP_BT.println(tempstring);
        tempstring = "";
#else
        Serial.println(LoRaData);
        cyclestrings(LoRaData);
        if (has_bluetooth_been_initialized)
          ESP_BT.println(LoRaData);
#endif
        if (LoRaData.startsWith(":SYS:") == false) // avoid spamming
          LoraSendAndUpdate(LoRaData);
      }
    }
    dodisplay = true;
  }
}

unsigned long antispam_timestamp2 = 0;
unsigned long antispam_timestamp1 = 0;
void SendSerialIfReady()
{
  // do actual sending; stay in send mode for as little as possible; this should be followed by the receive function
  if (readytosend and (pseudoseconds > antispam_timestamp1))
  {
    if (enablecomport == false)
    {
      if (String(receivedChars).startsWith(enablecomstring))
      {
#ifdef REPEATER_ONLY
#else
#ifdef DEBUG_OPTION_PAGE
        if (String(receivedChars).startsWith(enablecomstring "#"))
        {
          Serial.println(":SYS:Entering power state FULL");
          HighPowerSetup(true);
        }
        else if (String(receivedChars).startsWith(enablecomstring "@"))
        {
          Serial.println(F(":SYS:Entering power state REPEATER"));
          LowPowerLoop();
        }
        else if (String(receivedChars).startsWith(enablecomstring "!"))
        {
          Serial.println(":SYS:Entering power state SLEEP");
          SleepLowBatt();
        }
        else
#endif
#endif
        {
          Serial.println(":SYS:Serial port TX enabled");
          enablecomport = true;
        }

      }
      else
      {
        Serial.println(":SYS:Enable port TX first: " enablecomstring);
      }
      for (int i = 0; i < MAXPKTSIZE; i++)
      {
        receivedChars[i] = 0;
      }
      charcounter = 0;
      readytosend = false;
      dodisplaybuf = true;
      return;
    }
    //Send LoRa packet to receiver
    //  Serial.print("TX:");
    //  Serial.println(receivedChars);
    hextag = fourhex(derpme, spare_id_nibble + 15);
    lasttagimade = hextag;
    String serstr = String(receivedChars);
    charcounter = 0;
#ifdef GPS_SERIAL_1
    if (serstr.startsWith("UTC:"))
      serstr = fourhex(derpme, spare_id_nibble + 0) + TAG_END_SYMBOL + "UTC:" + String(UTC_Seconds);
    else
#endif
      serstr = hextag + TAG_END_SYMBOL + serstr;
    int highbytenums = serstr.length();
    for (int i = 0; i < MAXPKTSIZE; i++)
    {
      if (receivedChars[i] > 127)
        highbytenums = highbytenums + 50;
      receivedChars[i] = 0;
    }

    if (highbytenums > MAXPKTSIZE)
    {
      Serial.println(":SYS:Serial port noise detected, turning it off.");
      enablecomport = false;
      charcounter = 0;
      readytosend = false;
      dodisplaybuf = true;
      return;
    }

    if (LongChecksum(serstr) != LastThingISentViaLora_0 && (serstr.length() > 5))
    {
      if (has_bluetooth_been_initialized)
        ESP_BT.println(serstr);
      LoraSendAndUpdate(serstr);
      cyclestrings(serstr);
      antispam_timestamp1 = pseudoseconds + ANTISPAM_TIME_SERIAL;
      dodisplay = true;
      readytosend = false;
    }
  }

  if (readytosen2 and (pseudoseconds > antispam_timestamp2))
  {
    //Send LoRa packet to receiver
    hextag = fourhex(derpme, spare_id_nibble + 14);
    lasttagimade = hextag;
    String serstr = String(receivedChar2);
    charcounte2 = 0;
#ifdef GPS_SERIAL_1
    if (serstr.startsWith("UTC:"))
      serstr = fourhex(derpme, spare_id_nibble + 0) + TAG_END_SYMBOL + "UTC:" + String(UTC_Seconds);
    else
#endif
      serstr = hextag + TAG_END_SYMBOL + serstr;
    for (int i = 0; i < MAXPKTSIZE; i++)
    {
      receivedChar2[i] = 0;
    }
    if (LongChecksum(serstr) != LastThingISentViaLora_0 && (serstr.length() > 5))
    {
      LoraSendAndUpdate(serstr);
      antispam_timestamp2 = pseudoseconds + ANTISPAM_TIME_SERIAL;
      Serial.println(serstr);
      dodisplay = true;
      readytosen2 = false;
      cyclestrings(serstr);
    }
  }
}


int countme = 0;


bool InitDisplay(bool hardreset)
{
#ifndef NODISPLAY
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, hardreset, false)) { // Address 0x3C for 128x32
    if (has_serial_been_initialized)
      Serial.println(":SYS:SSD1306 init fail");
    displayexists = false;
    has_display_been_initialized = false;
    return false;
  }
  else
  {
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.clearDisplay();
    //    Serial.println(":SYS:SSD1306 init OK");
    display.display();
    displayexists = true;
    dodisplay = true;
    has_display_been_initialized = true;
    return true;
  }
#endif
  has_display_been_initialized = false;
  return false;
}

void InitDisplayTryAll()
{
#ifndef NODISPLAY
  if (has_display_been_initialized == false)
    has_display_been_initialized = InitDisplay(false);
  if (has_display_been_initialized == false)
    has_display_been_initialized = InitDisplay(true);
  if (has_display_been_initialized == false)
  {
    ResetDisplayViaPin();
    has_display_been_initialized = InitDisplay(false);
  }
  if (has_display_been_initialized == false)
    has_display_been_initialized = InitDisplay(true);
#endif
}

void BlinkMeWhen()
{
  if (++lploops > LPLOOP_BLINK)
  {
    led(true);
    DoBasicSteps();
    lploops = 0;
    led(false);
  }
}

void Start_LORA(bool trydisplay)
{
  if (has_lora_been_initialized)
    return;

  Stop_LORA();
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  //  bool changeclock = (getCpuFrequencyMhz() < CLKFREQ);

  //  if (changeclock)
  //    setCpuFrequencyMhz(CLKFREQ);

  has_lora_been_initialized = LoRa.begin(BAND);
  if (has_lora_been_initialized == false)
    has_lora_been_initialized = LoRa.begin(BAND);

  //  if (changeclock)
  //    setCpuFrequencyMhz(CLKFREQ_LOW);

  if (has_lora_been_initialized == false)
  {
    LoRa.end();
    SPI.end();
    tempstring = ":SYS:Starting LoRa failed! Trying again after sleep.";
    if (has_serial_been_initialized)
    {
      Serial.println(tempstring);
      Serial.flush();
    }
#ifndef NODISPLAY
    if (has_display_been_initialized and trydisplay and displayexists and displayenabled)
    {
      display.setCursor(0, 10);
      display.println(tempstring);
      display.display();
    }
#endif
    esp_deep_sleep_start();

  }
  else
  {
    LoRa.setTxPower(19, 1 ); // PABOOST; for RFO use 14,0
  }
}

void Start_UART()
{
  Serial.begin(UART_BAUD_RATE);
#ifdef GPS_SERIAL_1
  Serial1.begin(9600, SERIAL_8N1, 34, 12);
#endif
  has_serial_been_initialized = true;
}
void Stop_UART()
{
  Serial.flush();
  Serial.end();
#ifdef GPS_SERIAL_1
  Serial1.flush();
  Serial1.end();
#endif
  has_serial_been_initialized = false;
}

void Start_BT()
{
  if (!btStarted()) {
    btStart();
  }
  if (ipstring_b.length() < 2)
    ipstring_b = String(SSIDROOT) + String("BT") + String(derpme);
  ESP_BT.begin(ipstring_b); //Name of your Bluetooth Signal
  has_bluetooth_been_initialized = true;
}

void Stop_BT()
{
  ESP_BT.end();
  btStop(); // we aren't using BT for this one
  has_bluetooth_been_initialized = false;
}

// repeater-only mode for when we are low on juice
void LowPowerSetup()
{
  Watchdog(false);
  Stop_BT();
  dnsServer.stop();
  server.stop();
  WiFi.mode(WIFI_OFF);

#ifndef NODISPLAY
  if (has_display_been_initialized)
  {
    displayonoff(true);
    display.clearDisplay();
    display.display();
    displayonoff(false);
  }
#endif
  dodisplay = false;

  Stop_LORA();

  Stop_UART();

  setCpuFrequencyMhz(CLKFREQ_LOW); //Set CPU clock to 10MHz fo example

  PetTheWatchdog();

  Start_UART();

  Start_LORA(false);


  tempstring = ":SYS:LOWPWR " + String(derpme) + ":" + status_string();
  Serial.println(tempstring);

  if (low_batt_announce)
  {
    LoraSendAndUpdate(tempstring);
    cyclestrings(tempstring);
  }

  ReadBatteryADC(true);
}


void DoBasicSteps()
{
  PetTheWatchdog();
  ReadBatteryADC(false);

  if (has_lora_been_initialized)
    SeeIfAnythingOnRadio();


  ReadFromStream(Serial, receivedChars, charcounter, readytosend, has_serial_been_initialized, 15);

  ReadFromStream(ESP_BT, receivedChar2, charcounte2, readytosen2, has_bluetooth_been_initialized, 14);

  if ((has_serial_been_initialized || has_bluetooth_been_initialized) && has_lora_been_initialized)
    SendSerialIfReady();
}

void DoRepeaterSteps()
{

  if (has_serial_been_initialized == false)
    Start_UART();
  if (has_lora_been_initialized == false)
    Start_LORA(false);
  if (has_display_been_initialized)
    ResetDisplayViaPin();


  //  Serial.println(""+String(has_serial_been_initialized)+String(has_lora_been_initialized)+String(has_display_been_initialized));
  DoBasicSteps();

  if (currbatterylevel < (BATT_TOO_LOW_FOR_ANYTHING))
    SleepLowBatt();

#ifdef USER_BUTTON_PIN
  if (digitalRead(USER_BUTTON_PIN) == false)
    if ((lploops + lploops) < LPLOOP_BLINK) // blink for me if we push the button, to denote aliveness
      if (digitalRead(USER_BUTTON_PIN) == false)
      {
        lploops = LPLOOP_BLINK - 10;
      }
#endif

  BlinkMeWhen();

}

void LowPowerLoop()
{
  LowPowerSetup();
  Watchdog(true);
  while (currbatterylevel < (BATT_HIGH_ENOUGH_FOR_FULL_POWER + BATT_HYSTERESIS_POWER))
  {
    DoRepeaterSteps();
  }
  // continue flow
}

void DoBtSteps()
{
  PetTheWatchdog();


  ReadBatteryADC(false);
  if (currbatterylevel < (BATT_TOO_LOW_FOR_ANYTHING))
    SleepLowBatt();
  if (currbatterylevel < (BATT_HIGH_ENOUGH_FOR_FULL_POWER - BATT_HYSTERESIS_POWER))
  {
    LowPowerLoop();
    HighPowerSetup(true);
  }
  SeeIfAnythingOnRadio();
  ReadFromStream(Serial, receivedChars, charcounter, readytosend, has_serial_been_initialized, 15);
  ReadFromStream(ESP_BT, receivedChar2, charcounte2, readytosen2, has_bluetooth_been_initialized, 14);
  DoDisplayIfItExists();
  UpdateCharCounterIfDisplayExists();
  SendSerialIfReady();
  BlinkMeWhen();
}

#ifdef MODEFLIP
unsigned long modefliptime;
#endif

void DoWifiSteps()
{
  PetTheWatchdog();

  ReadBatteryADC(false);
  if (currbatterylevel < (BATT_TOO_LOW_FOR_ANYTHING))
    SleepLowBatt();
  if (currbatterylevel < (BATT_HIGH_ENOUGH_FOR_FULL_POWER - BATT_HYSTERESIS_POWER))
  {
    LowPowerLoop();
    HighPowerSetup(true);
  }

#ifdef MODEFLIP // switch between full AP and repeater according to a timer, but only if there are no wifi clients.
  if ((MODEFLIP < 60000) && (millis() > (modefliptime + MODEFLIP)))
  {
    if (WiFi.softAPgetStationNum() > 0)
    {
      modefliptime = millis(); // reset it
      Serial.println(":SYS:Modeflip skipped. Clients: " + String(WiFi.softAPgetStationNum()));
    }
    else if (currbatterylevel > MODEFLIP_BATTERY_FULL)
    {
      modefliptime = millis(); // reset it
      //      Serial.println(":SYS:Modeflip skipped. Battery: " + String(currbatterylevel)); // no need to announce either since we're clearly good on resources
    }
#ifdef MODEFLIP_BUTTON
    else if (digitalRead(USER_BUTTON_PIN) == false)
    {
      modefliptime = millis(); // reset it
      Serial.println(":SYS:Modeflip skipped. Button pressed. Battery: " + String(currbatterylevel));
    }
#endif
    else
    {
      bool derpp = true;
      LowPowerSetup();
      Watchdog(true);
      while (millis() < (modefliptime + 60000) && derpp)
      {
        DoRepeaterSteps();
#ifdef MODEFLIP_BUTTON
#ifdef USER_BUTTON_PIN
        if (digitalRead(USER_BUTTON_PIN) == false)
        {
          Serial.println(":SYS:Modeflip aborted. Button pressed. Battery: " + String(currbatterylevel));
          led(true);
          BasicWhileDelay(500); // give me a moment to remove my finger
          derpp = false;
          led(false);
          BasicWhileDelay(500); // give me a moment to remove my finger
        }
#endif
#endif

      }
      HighPowerSetup(true);
      modefliptime = millis();
      Watchdog(true);
    }
  }
#endif

  SeeIfAnythingOnRadio();
  ReadFromStream(Serial, receivedChars, charcounter, readytosend, has_serial_been_initialized, 15);
#ifdef BT_ENABLE_FOR_AP
  ReadFromStream(ESP_BT, receivedChar2, charcounte2, readytosen2, has_bluetooth_been_initialized, 14);
#endif
#ifdef WIFI_IS_CLIENT
#ifdef user_button_display
  if (digitalRead(USER_BUTTON_PIN) == false) // actually button pushed
    if (digitalRead(USER_BUTTON_PIN) == false) // actually button pushed
      if (digitalRead(USER_BUTTON_PIN) == false) // actually button pushed
      {
        ConnectToUpstreamWifi(100); // force the issue; no ping check
      }
#endif
#endif
  DoDisplayIfItExists();
  UpdateCharCounterIfDisplayExists();
  SendSerialIfReady();

  BlinkMeWhen();
}

void setup() {
  setCpuFrequencyMhz(CLKFREQ_LOW);



#ifdef EXT_PWR_PIN
  pinMode(EXT_PWR_PIN, OUTPUT);
  digitalWrite(EXT_PWR_PIN, LOW);
#endif
#ifdef USER_BUTTON_PIN
  pinMode(USER_BUTTON_PIN, INPUT);
#endif
#ifdef USER_LED_PIN
  pinMode(USER_LED_PIN, OUTPUT);
#endif

  led(false);


#ifdef BATT_ADC

  pinMode(BATT_ADC, INPUT);

  esp_sleep_enable_timer_wakeup(SLEEP_TIME * uS_TO_S_FACTOR);

  // determine cause of reset
  esp_reset_reason_t reason = esp_reset_reason();

  // get reg_b if reset not from deep sleep
  if ((reason == ESP_RST_DEEPSLEEP))
  {
    TryRetrieveSentences();
  }

  lowpowerstart = false;

  ReadBatteryADC(true); // important: only one before check

  if (currbatterylevel < (BATT_TOO_LOW_FOR_ANYTHING + BATT_HYSTERESIS_POWER))
  {
    SleepLowBatt();
  }

  if (currbatterylevel < (BATT_HIGH_ENOUGH_FOR_FULL_POWER - BATT_HYSTERESIS_POWER))
  {
    lowpowerstart = true;
  }

  // if battery is low but not critical, switch to repeater-only mode

#else
  currbatterylevel = 1500;
  lowpowerstart = false;
#endif


#ifdef WIFI_POWER_LEVEL
  WiFi.setTxPower(wifilevels[WIFI_POWER_LEVEL]);
#endif

  BuildNicknameTags();


  if (lowpowerstart)
  {
    LowPowerLoop();
  }

  HighPowerSetup(true);


}

bool currentlyserving = false;

unsigned int pausetag;
void BasicWhileDelay(int pausetime)
{
  pausetag = millis();
  while (millis() < (pausetag + pausetime))
  {
    DoBasicSteps(); // DoWifiSteps();
  }
}

#ifdef WIFI_IS_CLIENT // if defined, wifi will attach to an existing AP and act as a web gateway. it may be useful to have a way to get OUT of this mode, but that's for another day
bool CurrentlyConnecting = false;
void ConnectToUpstreamWifi(int pausetime)
{
  if (server.hasClient() || CurrentlyConnecting || client.connected() || currentlyserving)
  {
    led(true);
    BasicWhileDelay(1);
    led(false);
    return;
  }
  CurrentlyConnecting = true;
  led(true);
  Watchdog(false);
  //server.stop();
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  BasicWhileDelay(pausetime);

#ifdef WIFI_IS_HYBRID
  WiFi.mode(WIFI_MODE_APSTA);
  BasicWhileDelay(pausetime);
#ifndef DHCP
  WiFi.config(Client_IP, Client_Gateway, Client_Subnet, Client_Gateway, Client_Gateway);
#endif
  BasicWhileDelay(pausetime);
  WiFi.softAPConfig(AP_IP, AP_IP, IPAddress(255, 255, 255, 0));
  BasicWhileDelay(pausetime);
  if (String(WIFI_UPSTREAM_PWD).length() == 0)
    WiFi.begin(WIFI_UPSTREAM_AP);
  else
    WiFi.begin(WIFI_UPSTREAM_AP, WIFI_UPSTREAM_PWD);
  BasicWhileDelay(pausetime);
  WiFi.softAP(string2char(ssid), NULL, (1 + (derpme % 12)), false, 8); // the derpme thing is to set a channel
#else
  WiFi.mode(WIFI_MODE_STA);
  BasicWhileDelay(pausetime);
#ifndef DHCP
  WiFi.config(Client_IP, Client_Gateway, Client_Subnet, Client_Gateway, Client_Gateway);
#endif
  BasicWhileDelay(pausetime);
  WiFi.begin(WIFI_UPSTREAM_AP, WIFI_UPSTREAM_PWD);
#endif
  BasicWhileDelay(pausetime);
  long i = 0;
  led(false);
  while (WiFi.status() != WL_CONNECTED)
  {
    if (i++ > WIFI_RESET_CYCLES)
      ESP.restart();
    led(true);
    BasicWhileDelay(1);
    led(false);
    BasicWhileDelay(pausetime);
  }
#ifdef DHCP
  Client_IP = WiFi.localIP();
#endif

  LastReconnect = millis();
  ipstring = ipstring_a;//AP_IP.toString();//"Gateway";//Client_IP.toString()+"G";
  dodisplay = true;
  BasicWhileDelay(pausetime);
  server.begin();
  CurrentlyConnecting = false;
}
#endif


void HighPowerSetup(bool echo)
{
  Watchdog(false);
  setCpuFrequencyMhz(CLKFREQ); //Set CPU clock to 80MHz fo example
  led(true);
  ReadBatteryADC(true);
  lowpowerstart = false;

  Start_UART();

  wifimode = true; // adjust later

  //    esp_wifi_set_protocol( WIFI_IF_AP, WIFI_PROTOCOL_LR ); //LR only works between esp32s sadly
  WiFi.mode(WIFI_AP); // WIFI_AP_STA
  //  esp_wifi_set_protocol( WIFI_IF_AP, WIFI_PROTOCOL_11B ); // LR only works between esp32s sadly
  WiFi.softAP(string2char(ssid), NULL, (1 + (derpme % 12)), false, 8); // the derpme thing is to set a channel

  low_batt_announce = POWER_STATE_CHANGE_ANNOUNCE;

  //  Stop_LORA();

  Start_LORA(false);

  //reset OLED display via software
  //initialize OLED
  DoBasicSteps();
  ResetDisplayViaPin();

  //  InitDisplay(true);
  //  displayonoff(true);

  // Connect to Wi-Fi network with SSID and password

  WiFi.softAPConfig(AP_IP, AP_IP, IPAddress(255, 255, 255, 0));

  dnsServer.start(DNS_PORT, "*", AP_IP);
  DoBasicSteps();

#ifdef WIFI_IS_CLIENT
#else
  server.setNoDelay(true);
  client.setTimeout(2);
  server.setTimeout(2);
#endif

#ifdef USER_BUTTON_PIN // true = wifi, false = bluetooth
  int j = 0;
  for (int i = 0; i < 50; i++)
  {
    BasicWhileDelay(1);
    if (digitalRead(USER_BUTTON_PIN))
      j++; // cheapo debouncing
  }
  wifimode = (j > 24);

#endif
  led(false);

#ifdef WIFI_IS_CLIENT
  wifimode = true;
#endif

  if (wifimode == false)
  {
    WiFi.mode(WIFI_OFF);
    server.stop();
    dnsServer.stop();
    ipstring = String(SSIDROOT) + String("BT") + String(derpme);
    esp_wifi_stop();
    Start_BT();
    dodisplay = true;
  }
  else
  {
#ifdef BT_ENABLE_FOR_AP
    Start_BT();
#else
    Stop_BT();
#endif

#ifdef WIFI_IS_CLIENT
    ConnectToUpstreamWifi(100);
#endif

  }
  tempstring = ":SYS:" PYLONTYPE " " + String(derpme) + ":" + status_string();
  if (low_batt_announce)
  {
    LoraSendAndUpdate(tempstring);
    cyclestrings(tempstring);
  }
  if (echo)
    Serial.println(tempstring);

  // reset these
  AP_IP = IPAddress(192, 168, derpme, 1);
  Client_IP = IPAddress(CLIENT_IP_ADDR);
#ifdef DHCP
  Client_IP = WiFi.localIP();
#else
  Client_IP = IPAddress(CLIENT_IP_ADDR);
#endif
  IP = AP_IP;
  ipstring = IP.toString();
  ipstring_a = AP_IP.toString();
  ipstring_c = Client_IP.toString();

  server.stop();
  server.begin();


  //Serial.println(":SYS:IP " + ipstring_a + " " + ipstring_c + " " + ipstring + " " + lwc.toString());

  if (DISPLAY_INTERVAL > 10000)
  {
    display_on_ping = true;
  }

#ifdef TUTORIALSTRINGS
  if (string_rx[2].length() < 3)
  {
    if (wifimode == false)
    {
      //                                   012345678901234567890
      //              012345678901234567890
      string_rx[2] = ":SYS: Bluetooth mode on. To switch to wifi";
      string_rx[1] = "simply reset the unit You can download a  ";
      string_rx[0] = "BT chat app from the wifi page.";
    }
    else
    {

#ifdef WIFI_IS_CLIENT
      //                                   012345678901234567890
      //              012345678901234567890
#ifdef WIFI_IS_HYBRID
      string_rx[2] = ":SYS: AP IP address: " + AP_IP.toString();
#else
      string_rx[2] = ":SYS: This unit talks to upstream wifi.";
#endif
#ifdef DHCP
      string_rx[1] = "(D)Client IP address:" + Client_IP.toString();
#else
      string_rx[1] = "Client IP address:   " + Client_IP.toString();
#endif
      string_rx[0] = "Upstream router:     " + Client_Gateway.toString();
#else
#ifdef BT_ENABLE_FOR_AP
      //                                   012345678901234567890
      //              012345678901234567890
      string_rx[2] = ":SYS: This unit can  also be talked to via";
      string_rx[1] = "Bluetooth. You can   download the app via";
      string_rx[0] = "wifi, then reconnect using it.";
#else
#ifdef MODEFLIP
      //                                   012345678901234567890
      //              012345678901234567890
      string_rx[2] = ":SYS: This unit will switch between full";
      string_rx[1] = "and repeater/serial  once a minute. If you";
      string_rx[0] = "can't connect, try   for another minute.";
#else

      //                                   012345678901234567890
      //              012345678901234567890
      string_rx[2] = ":SYS: To switch to   Bluetooth, reset the";
      string_rx[1] = "unit and immediately press and hold PROG";
      string_rx[0] = "after letting the RST button go.";
#endif
#endif
#endif
    }
  }
#endif


}


bool LoginPageRequested(String url)
{
  if (url.startsWith("/redirect"))
    return true;
  if (url.startsWith("/connecttest.txt"))
    return true;
  if (url.startsWith("/ncsi.txt"))
    return true;
  if (url.startsWith("/hotspot.txt"))
    return true;
  if (url.startsWith("/success.txt"))
    return true;
  if (url.startsWith("/generate_204"))
    return true;
  if (url.startsWith("/hotspot-detect.html"))
    return true;
  return false;
}

void ServeWebPagesAsNecessary()
{
  dnsServer.processNextRequest();

  // Do wifi things here
  client = server.available();   // Listen for incoming clients

#ifdef WIFI_IS_CLIENT
  if (client == false) // we are not connected
  {
    if (millis() > (LastReconnect + RECONNECT_EVERY))
    {
      ConnectToUpstreamWifi(100);
    }
  }
#endif

  if (client) {
    //  Serial.print("2");
    currentlyserving = true;
    //      Serial.println(String(client)); // says 1
#ifdef WIFI_IS_CLIENT
    LastReconnect = millis();
#endif
    unsigned long connect_timeout = millis() + 3000;
    while (client.connected()) {            // loop while the client's connected
      // does not get this far outside lan for some reason
      if (client.available() == false)
      {
        //  Serial.print("3");
        DoBasicSteps();
        dnsServer.processNextRequest();
        if (millis() > connect_timeout)
        {
          client.stop();
          break;
        }

      }
      if (client.available()) // if there's bytes to read from the client,
      {        
        //  Serial.print("4");
        c = client.read();
        serveWebSiteRequests(c);
      }
    }
    // Clear the header variable
    header = "";
    client.stop();
    currentlyserving = false;
  }
}

void loop() {
  if (wifimode)
  {
    DoWifiSteps();
    ServeWebPagesAsNecessary();    
    DoBasicSteps();
  }
  else
  {
    DoBtSteps();
  }
}
