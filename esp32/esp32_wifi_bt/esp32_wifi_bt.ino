/*********
  Cellular-Solar (CellSol) is a simple interconnect between lora, wifi and com port(s). It is intended to be used for infrastructure-independent comms during or after a disaster.
  (c) 2020 Robots Everywhere, LLC until we are ready to release it under copyleft
  Written by Riley August (HTML/CSS/DHCP/Optimizations), and M K Borri (other). Thanks to Rui Santos for the tutorials. Thanks to Jerry Jenkins for the inspiration. Thanks to Lisa Rein for initiating the project.  Thanks to Robots Everywhere for infrastructure support https://www.robots-everywhere.com
  Originally produced as part of the Aaron Swartz Day project https://www.aaronswartzday.org
  Distributed independently https://www.f3.to/cellsol

  This file is used for esp32-based boards (Lora32, TTGO T-Beam, etc.)
*********/

// Firmware version.
#define VERSIONSTRING "0.33c"

#include "config.h" // Please make config changes in config.h and pinout.h instead of here!

#ifdef PROVIDE_APK
#include "btt/btt.h"
#endif

#ifdef PROVIDE_CAT_PICTURE
#include "btt/cat.h"
#endif

#ifdef PROVIDE_SOURCE_CODE
#include "btt/src.h"
#endif


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds. Do not change this. Seriously. */


//Libraries for LoRa
#include <SPI.h>
#include "pinout.h"
#ifdef LORA32V3
#include "CellSolRadioV3.h" // Heltec V3 has an SX1262, not an SX127x
#else
#include <LoRa.h> // Sandeep Mistry LoRa, for SX127x-based boards
#endif

// Load Wi-Fi library
#include <Network.h>
#include <NetworkClient.h>
#include <NetworkServer.h>
#include <WiFiAP.h>
#include <WiFi.h>
#include <Preferences.h>
#include "time.h"
// taken from genericwifi.h -1dbm to 19.5dbm
#ifdef WIFI_POWER_LEVEL
const wifi_power_t wifilevels[] = {WIFI_POWER_MINUS_1dBm, WIFI_POWER_2dBm, WIFI_POWER_5dBm, WIFI_POWER_7dBm, WIFI_POWER_8_5dBm, WIFI_POWER_11dBm, WIFI_POWER_13dBm, WIFI_POWER_15dBm, WIFI_POWER_17dBm, WIFI_POWER_18_5dBm, WIFI_POWER_19dBm, WIFI_POWER_19_5dBm}; // 0~11
#endif

#include <DNSServer.h>
const byte DNS_PORT = 53;
#include <esp_wifi.h>

#ifdef LORA32V3

//#include <BLEDevice.h>
//#include <BLEUtils.h>
//#include <BLEServer.h>
//#include <BLE2902.h>
#include <HardwareBLESerial.h>
HardwareBLESerial &ESP_BT = HardwareBLESerial::getInstance();

#else

#include "BluetoothSerial.h"
BluetoothSerial ESP_BT;

#endif


// this one is its own thing
#include <IRCClient.h>

// watchdog
#include <esp_task_wdt.h>
#define WDT_TIMEOUT 4 // in seconds

#include "esp32-hal-cpu.h"
#define CLKFREQ_HI 240 // megahertz when going as fast as possible
#define CLKFREQ 80 // megahertz when in full mode
#define CLKFREQ_LOW 20 // megahertz when in repeater mode; can't go any lower, annoyingly

// pinout.h is loaded before the radio driver, because V3 needs its SX1262 pins.


#ifdef MINDISPLAY
#define NODISPLAY
#include <Wire.h>
//#include "ACROBOTIC_SSD1306_minimal/ACROBOTIC_SSD1306_minimal.h" // pared down SS1306 driver that only has the 5x7 font
#include "ACROBOTIC_SSD1306_minimal/ACROBOTIC_SSD1306_minimal.cpp" // pared down SS1306 driver that only has the 5x7 font
#endif

#ifndef NODISPLAY
//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif

// LORA32 setup variables
static bool lowpowerstart = false;
static uint64_t chipid; // chip id stuff = ESP.getEfuseMac();
static uint16_t uptwo; // chip id stuff = (uint16_t)(chipid >> 32);
static uint32_t dnfor; // chip id stuff  = (uint32_t)(chipid);


// Web constants for text align, used for website display
#define TEXT_ALIGN_STRING_A "center"
#define TEXT_ALIGN_STRING_B "justify"
String TEXT_ALIGN_STRING = TEXT_ALIGN_STRING_A;
static RTC_NOINIT_ATTR bool centertext = true;// if this, center, otherwise, justify


// Wireless UART Configuration
uint32_t LastReconnect = 0;
int currbatterylevel = 0;

#define UART_BAUD_RATE 9600
String ssid = SSIDROOT;
String tempstring = ""; // internal function use only

bool wifimode = true; // bluetooth or wifi

// Radio parameters remain compile-time for mesh interoperability. These
// local-network/IRC preferences are persisted across reboots in NVS.
enum CellSolMode : uint8_t {
  CELLSOL_AP = 0,
  CELLSOL_BT = 1,
  CELLSOL_AP_BT = 2,
  CELLSOL_STA_BT = 3,
  CELLSOL_AP_STA_BT = 4
};

struct CellSolSettings {
  uint8_t mode;
  String nodeID; // optional 4-hex-digit pylon ID; empty uses hardware-derived ID
  int apThirdOctet; // -1 = derive the local AP subnet from hardware, independent of node ID
  String apPrefix;
  String upstreamSSID;
  String upstreamPassword;
  bool dhcp;
  String clientIP;
  String gatewayIP;
  String subnet;
  uint8_t wifiPower;
  bool ircEnabled;
  String ircServer;
  uint16_t ircPort;
  String ircChannel;
  String ircNick;
  String ircPrefix;
  bool ircChannelTag;
  bool ircForwardPylons;
};

CellSolSettings settings;
uint8_t activeMode = CELLSOL_AP;
String pendingOldNodeID = ""; // saved before an ID change, announced once radio starts

// The fourth hex digit belongs to attached clients (0=pylon, e=BT, f=serial).
// Changing the pylon ID therefore changes only the first three hex digits.
int HexNibble(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  return -1;
}

bool ParsePylonNodeID(const String &id, uint8_t &idHighByte, uint8_t &clientPrefix) {
  if (id.length() != 4) return false;
  int n[4];
  for (int i = 0; i < 4; ++i) {
    n[i] = HexNibble(id[i]);
    if (n[i] < 0) return false;
  }
  // The submitted fourth nibble is ignored; the low nibble is for clients.
  idHighByte = (n[0] << 4) | n[1];
  clientPrefix = n[2] << 4;
  return true;
}

bool UsesUpstreamWifi() {
  return activeMode == CELLSOL_STA_BT || activeMode == CELLSOL_AP_STA_BT;
}

bool HasLocalAP() {
  return activeMode == CELLSOL_AP || activeMode == CELLSOL_AP_BT || activeMode == CELLSOL_AP_STA_BT;
}

const char *PylonTypeLabel() {
  switch (activeMode) {
    case CELLSOL_BT: return "BT)";
    case CELLSOL_AP_BT: return "AP+BT)";
    case CELLSOL_STA_BT: return settings.ircEnabled ? "STA+BT+I)" : "STA+BT)";
    case CELLSOL_AP_STA_BT: return settings.ircEnabled ? "AP+STA+BT+I)" : "AP+STA+BT)";
    default: return "AP)";
  }
}

void LoadCellSolSettings() {
  Preferences prefs;
#if defined(WIFI_IS_HYBRID)
  const uint8_t defaultMode = CELLSOL_AP_STA_BT;
#elif defined(WIFI_IS_CLIENT)
  const uint8_t defaultMode = CELLSOL_STA_BT;
#elif defined(BT_ENABLE_FOR_AP)
  const uint8_t defaultMode = CELLSOL_AP_BT;
#else
  const uint8_t defaultMode = CELLSOL_AP;
#endif
  settings.mode = defaultMode;
  settings.nodeID = "";
  settings.apThirdOctet = -1;
  settings.apPrefix = SSIDROOT;
  settings.upstreamSSID = WIFI_UPSTREAM_AP;
  settings.upstreamPassword = WIFI_UPSTREAM_PWD;
#ifdef DHCP
  settings.dhcp = true;
#else
  settings.dhcp = false;
#endif
  settings.clientIP = IPAddress(CLIENT_IP_ADDR).toString();
  settings.gatewayIP = IPAddress(GATEWAY_IP_ADDR).toString();
  settings.subnet = IPAddress(GATEWAY_SUBNET).toString();
  settings.wifiPower = WIFI_POWER_LEVEL;
  if (settings.wifiPower > 11) settings.wifiPower = 11;
#if defined(WIFI_IS_CLIENT) && defined(IRC_SERVER)
  settings.ircEnabled = true;
#else
  settings.ircEnabled = false;
#endif
  settings.ircServer = IRC_SERVER;
  settings.ircPort = IRC_PORT;
  settings.ircChannel = IRC_CHAN_ROOT;
  settings.ircNick = IRC_NICK_ROOT;
  settings.ircPrefix = FWD_PREFIX;
#ifdef IRC_CHAN_HEXTAG
  settings.ircChannelTag = true;
#else
  settings.ircChannelTag = false;
#endif
#ifdef ALLOW_PYLON_FORWARD
  settings.ircForwardPylons = true;
#else
  settings.ircForwardPylons = false;
#endif

  if (!prefs.begin("cellsol", true))
    return; // defaults still work if NVS cannot be opened
  settings.mode = prefs.getUChar("mode", settings.mode);
  if (settings.mode > CELLSOL_AP_STA_BT) settings.mode = CELLSOL_AP;
  String savedID = prefs.getString("nodeid", "");
  uint8_t savedIP, savedPrefix;
  if (ParsePylonNodeID(savedID, savedIP, savedPrefix)) {
    savedID.toLowerCase();
    savedID.setCharAt(3, '0');
    if (savedID != "0000") settings.nodeID = savedID;
  }
  // A v0.33b manual ID also chose the AP subnet. Preserve that subnet on
  // upgrade, but let later ID changes leave it alone.
  const int storedAPOctet = prefs.getInt("apoct", -2);
  if (storedAPOctet >= 0 && storedAPOctet <= 254)
    settings.apThirdOctet = storedAPOctet;
  else if (storedAPOctet == -2 && settings.nodeID.length())
    settings.apThirdOctet = savedIP;
  settings.apPrefix = prefs.getString("aproot", settings.apPrefix);
  settings.upstreamSSID = prefs.getString("upssid", settings.upstreamSSID);
  settings.upstreamPassword = prefs.getString("uppass", settings.upstreamPassword);
  settings.dhcp = prefs.getBool("dhcp", settings.dhcp);
  settings.clientIP = prefs.getString("clientip", settings.clientIP);
  settings.gatewayIP = prefs.getString("gateway", settings.gatewayIP);
  settings.subnet = prefs.getString("subnet", settings.subnet);
  settings.wifiPower = prefs.getUChar("wifipwr", settings.wifiPower);
  if (settings.wifiPower > 11) settings.wifiPower = 11;
  settings.ircEnabled = prefs.getBool("ircon", settings.ircEnabled);
  settings.ircServer = prefs.getString("ircserver", settings.ircServer);
  settings.ircPort = prefs.getUShort("ircport", settings.ircPort);
  settings.ircChannel = prefs.getString("ircchan", settings.ircChannel);
  settings.ircNick = prefs.getString("ircnick", settings.ircNick);
  settings.ircPrefix = prefs.getString("ircprefix", settings.ircPrefix);
  settings.ircChannelTag = prefs.getBool("irctag", settings.ircChannelTag);
  settings.ircForwardPylons = prefs.getBool("ircpeer", settings.ircForwardPylons);
  String oldID = prefs.getString("idprev", "");
  if (ParsePylonNodeID(oldID, savedIP, savedPrefix)) {
    oldID.toLowerCase();
    pendingOldNodeID = oldID;
  }
  prefs.end();
}

bool SaveCellSolSettings(const CellSolSettings &value, const String &oldID = "") {
  Preferences prefs;
  if (!prefs.begin("cellsol", false)) return false;
  // Record the previous ID first. If power is lost during saving, a reboot
  // can still notify the network once the new ID actually takes effect.
  if (oldID.length() && prefs.putString("idprev", oldID) == 0) {
    prefs.end();
    return false;
  }
  prefs.putUChar("mode", value.mode);
  prefs.putString("nodeid", value.nodeID);
  prefs.putInt("apoct", value.apThirdOctet);
  prefs.putString("aproot", value.apPrefix);
  prefs.putString("upssid", value.upstreamSSID);
  prefs.putString("uppass", value.upstreamPassword);
  prefs.putBool("dhcp", value.dhcp);
  prefs.putString("clientip", value.clientIP);
  prefs.putString("gateway", value.gatewayIP);
  prefs.putString("subnet", value.subnet);
  prefs.putUChar("wifipwr", value.wifiPower);
  prefs.putBool("ircon", value.ircEnabled);
  prefs.putString("ircserver", value.ircServer);
  prefs.putUShort("ircport", value.ircPort);
  prefs.putString("ircchan", value.ircChannel);
  prefs.putString("ircnick", value.ircNick);
  prefs.putString("ircprefix", value.ircPrefix);
  prefs.putBool("irctag", value.ircChannelTag);
  prefs.putBool("ircpeer", value.ircForwardPylons);
  prefs.end();
  return true;
}

// serial on/off
boolean enablecomport = false; // set to false for a solderless fix for UART buffer crosstalk hardware issue

// Set web server port number to 80
NetworkServer server(80);
DNSServer dnsServer;
NetworkClient wificlient1;

// WEB SERVICE CONFIGURATION - What files do we want to serve?
#ifdef PROVIDE_APK
extern const long int btt_apk_size;
extern const unsigned char btt_apk[];
#endif
#ifdef PROVIDE_CAT_PICTURE
extern const long int cat_jpg_size;
extern const unsigned char cat_jpg[];
#endif
#ifdef PROVIDE_SOURCE_CODE
extern const long int src_zip_size;
extern const unsigned char src_zip[];
#endif

// BATTERY POWER CONFIGURATION
static RTC_NOINIT_ATTR int lastbatterylevel = 0; // try to save battery levels between runs
int batt_delta;
uint32_t battimeout = 0;
bool low_batt_announce = false;

// SERIAL and IP CONFIGURATION
int lploops = LPLOOP_BLINK - 10; // give me a blink early so i know we're alive
bool has_lora_been_initialized = false;
bool has_serial_been_initialized = false;
bool has_bluetooth_been_initialized = false;
bool has_wifi_been_initialized = false;
bool has_display_been_initialized = false;

// USER TAG CONFIGURATION
String hextag  = "XXXX"; // usually the last two IP octets; gives pseudonimity to sender
String lasttagimade = "XXXX"; // last one we made for use in checking

// Temp bytes for IP addresses
byte derpme = 0; // upper byte of the radio node ID (no longer tied to the AP IP)
byte spare_id_nibble = 0; // upper nibble; lower nibble always 0
String generatedNodeID = ""; // hardware-derived ID for restoring automatic mode
byte generatedAPOctet = 0; // independent of manual node identity


//packet counter
int txcounter = 0;
int rxcounter = 0;
byte charcounter = 0;
boolean readytosend = false;
byte charcounte2 = 0;
boolean readytosen2 = false;

// display flags
boolean dodisplay = false;
boolean dodisplaybuf = false;
boolean displayexists = false;
bool displayenabled = true;

#ifdef REPEAT_UNTIL_ACK
boolean got_lora_ack = false;
long last_repeat_time = millis();
String message_to_repeat = "";
#endif


#ifndef NODISPLAY
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
#endif


uint32_t disptimeout = 0;
bool display_on_ping = false;


/**********************************************
   Watchdog block; creates and manages a task watchdog for esp32 to prevent unwanted power off
 **********************************************/
unsigned long pseudoseconds = 0;
unsigned long psm = 0;
long UTC_Seconds = 0;
bool is_watchdog_on = false;
void PetTheWatchdog() {
  if ((uint32_t)(millis() - psm) >= 1000)
  {
    if (is_watchdog_on)
      esp_task_wdt_reset();
    // apparently the esp32 is smart enough to update tick size
    /*
      if (getCpuFrequencyMhz() < CLKFREQ)
        psm = psm+1000;
      else
        psm=psm+1000;
    */
    psm = psm + 1000;
    pseudoseconds++;

    DoUTC(true);
  }
#ifdef GPS_SERIAL_1
  else
    DoUTC(false);
#endif
}

#define CONFIG_FREERTOS_NUMBER_OF_CORES 2
void Watchdog(bool onoff) // gets turned back on by the adc reading, assumtion is that we can survive for 5 seconds we hope
{
  if (onoff == is_watchdog_on)
  {
    battimeout = millis() + ADC_INTERVAL;
    return;
  }

  if (onoff)
  {
    esp_task_wdt_config_t twdt_config = {
      .timeout_ms = WDT_TIMEOUT * 1000,
      .idle_core_mask = 0,
      .trigger_panic = true,
    };
    esp_err_t err = esp_task_wdt_init(&twdt_config);
    if (err == ESP_ERR_INVALID_STATE) // already initialized by the core
      err = esp_task_wdt_reconfigure(&twdt_config);
    if (err == ESP_OK)
    {
      if (esp_task_wdt_status(NULL) != ESP_OK)
        err = esp_task_wdt_add(NULL);
      is_watchdog_on = (err == ESP_OK);
    }
    if (!is_watchdog_on)
      Serial.println(":SYS:Watchdog registration failed");
  }
  else
  {
    if (esp_task_wdt_status(NULL) == ESP_OK)
      esp_task_wdt_delete(NULL);
    is_watchdog_on = false;
    // Don't deinitialize the system watchdog: other tasks may use it.
  }
  battimeout = millis() + ADC_INTERVAL;
}

/**
   END WATCHDOG BLOCK
*/


/*******************************************************************************
   TCP/IP Block
 *******************************************************************************/
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

// util method
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

  generatedAPOctet = derpme;
  generatedNodeID = fourhex(derpme, spare_id_nibble);
  uint8_t configuredOctet, configuredPrefix;
  if (ParsePylonNodeID(settings.nodeID, configuredOctet, configuredPrefix)) {
    // Manual IDs replace both ID bytes, so battery noise has no influence.
    derpme = configuredOctet;
    spare_id_nibble = configuredPrefix;
  }
  AP_IP[2] = settings.apThirdOctet >= 0 ? settings.apThirdOctet : generatedAPOctet;
  ipstring_a = AP_IP.toString();
  ipstring = ipstring_a;
  ssid = settings.apPrefix + ipstring;
  TEXT_ALIGN_STRING = centertext ? TEXT_ALIGN_STRING_A : TEXT_ALIGN_STRING_B;
}
String status_string()
{
  return (fourhex(derpme, spare_id_nibble) + TAG_END_SYMBOL + "(" + String(currbatterylevel) + "/" + String(batt_delta) + ") " + (is_watchdog_on ? "`" : ",") + String(pseudoseconds) + (broadcast_twice ? "`" : ",") ).c_str();
}
/*
   DUMMY METHODS FOR HTML ENCODE/DECODE - TODO: remove this comment as these methods got replaced?
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


/****************************************************************
   LoRa32 Radio block - LoRa, WiFi, and BT
*/


String LoRaData;

// Variable to store the HTTP request
String header;

#ifdef COMMUNITY_MEMORY_SIZE
String communitymemory[COMMUNITY_MEMORY_SIZE];
#endif
String string_rx[10];
long timestamp_rx[10];
String TimeAgoString(long secs) // keep it simple for display
{
  if (secs == pseudoseconds || secs < 0)
    return "?";
  if (secs < 60)
    return String(secs) + "s";
  if (secs < 3600)
    return String(secs / 60) + "m";
  if (secs < 86400)
    return String(secs / 3600) + "h";
  return String(secs / 86400) + "d";
}


long LastThingISentViaLora_3 = 0;
long LastThingISentViaLora_2 = 0;
long LastThingISentViaLora_1 = 0;
long LastThingISentViaLora_0 = 0;
long LTISVL_3_time = 0;
long LTISVL_2_time = 0;
long LTISVL_1_time = 0;
long LTISVL_0_time = 0;

String gotstring = "";


#ifdef USER_BUTTON_PIN
#else
#undef MODEFLIP_BUTTON
#endif

// if this exists, only show the display when we are pushing the button that way we know someone is actually looking at it (saves power)
#define user_button_display

#define MAXPKTSIZE  200 // lora packet is 255 bytes and we will need some for the header
#define MAXPKTSIZEP 201 // lora packet is 255 bytes and we will need some for the header
#define MAXPKTSIZEM 199 // lora packet is 255 bytes and we will need some for the header
#define MAX_HTTP_REQUEST_SIZE 768 // includes percent-encoded UTF-8 and browser headers
char receivedChars[MAXPKTSIZE]; // an array to store the received data
char receivedChar2[MAXPKTSIZE]; // an array to store the received data
int rssi; // last packat received rssi

#ifdef BATT_ADC
static char RTC_NOINIT_ATTR byteme[10][MAXPKTSIZE]; // save last x sentences in here for posterity
#endif

void TryStoreSentences()
{
#ifdef BATT_ADC
  for (byte i = 0; i < 10; i++)
    string_rx[i].toCharArray(byteme[i], MAXPKTSIZE);
#endif
}
void TryRetrieveSentences()
{
#ifdef BATT_ADC
  for (byte i = 0; i < 10; i++)
    string_rx[i] = String(byteme[i]);
#endif
}

inline bool IsHex(char i);
inline int hexValue(char c);
void decode_in_place(char *s) {
  char *d = s;

  while (*s) {
    switch (*s) {
      case '+': *d++ = ' '; s++; break; // turn + into space
      case 13: *d++ = ' '; s++; break; // turn CR into space
      case 10: *d++ = ' '; s++; break; // turn LF into space
      case '%':
        if (IsHex(s[1]) && IsHex(s[2])) {
          char decoded = (hexValue(s[1]) << 4) | hexValue(s[2]);
          *d++ = decoded ? decoded : ' '; // don't allow an embedded NUL
          s += 3;
        } else {
          *d++ = *s++; // preserve malformed percent escapes
        }
        break;
      default:
        *d++ = *s++; break;
    }
  }

  *d = '\0'; // always add the terminator
}

// used to avoid c.str for everything since the bluetooth library for BLE serial is a bit bolted on
namespace
{
template<typename T>
void ESP_BT_println(T value) {
#ifdef LORA32V3
  ESP_BT.println(value.c_str());
#else
  ESP_BT.println(value);
#endif
}
}

inline int hexValue(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  return 0;
}

// Never return c_str() from a by-value String: its pointer would dangle.
// moves old strings out
void cyclestrings(String newone)
{
  newone.replace('\r', ' ');
  newone.replace('\n', ' ');//0xE2 0x80 0x8B
  newone.replace("&", "&amp;"); // HTML-escape once for the web output
  newone.replace("<", "&lt;");
  newone.replace(">", "&gt;");
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

inline void Stop_LORA()
{
  if (has_lora_been_initialized)
  {
    SeeIfAnythingOnRadio();
    LoRa.end();
#ifdef LORA32V3
    // The RadioLib adapter leaves SPI ownership to the sketch.
    SPI.end();
#endif
    has_lora_been_initialized = false;
  }
}


// Both Stream and HardwareBLESerial support these byte and text methods.
// One implementation avoids the old two copies developing different bugs.
template <typename T>
void ReadFromStream(T &st, char buf[], byte &cnt, bool &sendout, bool streamexists, int whichtag)
{
  if (!streamexists || sendout)
    return; // do not overwrite a complete message before it is transmitted

  while (st.available() > 0)
  {
    int incoming = st.read();
    if (incoming < 0)
      break;
    char ch = (char)incoming;

    if (cnt == 0 && ((unsigned char)ch < 6 || (unsigned char)ch > 127))
      continue; // discard noise at the start of a line

    if (ch == '\r' || ch == '\n')
    {
      if (cnt == 0)
        continue; // ignore blank lines and the LF half of CRLF

      if (cnt == 3 && buf[0] == ',' && buf[2] == ',' && (buf[1] == ',' || buf[1] == '.'))
      {
        cnt = 0;
        buf[0] = '\0';
        hextag = fourhex(derpme, spare_id_nibble + whichtag);
        String status = ":SYS: TAG:" + hextag + " BAT:" + String(currbatterylevel) +
                        " VER:" VERSIONSTRING " UPT" + (broadcast_twice ? ";" : ":") +
                        String(pseudoseconds) + " MEM:";
        st.println(status.c_str());
        for (int i = 9; i >= 0; i--)
        {
          PetTheWatchdog();
          if (string_rx[i].length() == 0)
            continue;
          tempstring = string_rx[i];
          tempstring.replace("&lt;", "<");
          tempstring.replace("&gt;", ">");
          tempstring.replace("&amp;", "&");
          st.println(tempstring.c_str());
        }
      }
      else
      {
        buf[cnt] = '\0';
        sendout = true;
        dodisplay = true;
      }
      return;
    }

    if (cnt >= MAXPKTSIZE - 1)
    {
      buf[MAXPKTSIZE - 1] = '\0';
      sendout = true;
      dodisplay = true;
      return;
    }
    buf[cnt++] = ch;
    buf[cnt] = '\0';
    UpdateCharCounterIfDisplayExists();
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

#ifdef REBROADCAST_DISASTER_RADIO_PACKETS // what it says on the tin: do we also want to display it?
  //Byte 0   Byte 1        Byte 2 - 5  Byte 6 - 9  Byte 10   Byte 11 - 14  Byte 15   Byte 16   Byte 17 - 255
  //ttl      totalLength   sender      receiver    sequence  source        hopCount  metric    datagram
  //https://github.com/sudomesh/disaster-radio/wiki/Protocol#packet-structure
  if (lenlen > 16 && lenlen == (uint8_t)LoRaData.charAt(1) &&
      (uint8_t)LoRaData.charAt(0) > (uint8_t)LoRaData.charAt(15))
  {
    LoRaData.setCharAt(15, (LoRaData.charAt(15) + 1) % 256); // increment the hop count
    LoraSendAndUpdate(LoRaData); // send here, but don't cycle strings or output to serial(s)
#ifdef DISPLAY_DISASTER_RADIO_PACKETS // only do this if we are rebroadcasting them, nobody likes a stalker
    String drstring = "(DR)" + TAG_END_SYMBOL + fourhex(LoRaData.charAt(2), LoRaData.charAt(3)) + fourhex(LoRaData.charAt(4), LoRaData.charAt(5)) + TAG_END_SYMBOL + LoRaData.substring(16, 176);
    Serial.println(drstring);
    cyclestrings(drstring);
    if (has_bluetooth_been_initialized)
      ESP_BT_println(drstring);
#ifdef IRC_SERVER
    irc_broadcast( drstring);
#endif
#endif
    return false; // send here, but don't cycle strings or output to serial(s)
  }
#endif




#ifdef REQUIRE_TAG_FOR_REBROADCAST
  if (lenlen < 5) // too short
    return false;
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

  if (LoRaData.charAt(0) == hextag.charAt(0) && LoRaData.charAt(1) == hextag.charAt(1) && LoRaData.charAt(2) == hextag.charAt(2))
    return false; // stop broadcast storms
  if (LoRaData.startsWith(lasttagimade)) // failed rebroadcast attempt, so filter out
    return false;

#ifndef GPS_SERIAL_1 // if we already have our own gps, use it. otherwise do this
  if (lenlen > 14 and LoRaData.charAt(3) == '0' and LoRaData.charAt(5) == 'U' and LoRaData.charAt(6) == 'T' and LoRaData.charAt(7) == 'C' and LoRaData.charAt(8) == ':') // Do a bunch of checks to make sure we're getting a good packet
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


String UTC_String(long secs)
{
  if (secs < 100000)
    return "N/A";
  //  String st = (secs > 199999) ? "A" : "V";
  String st;
  secs = secs % 100000;

  int h, m, s;

  s = secs % 60;

  secs = (secs - s) / 60;
  m = secs % 60;

  secs = (secs - m) / 60;
  h = secs;

  st = st + String(h) + ":" + String(m) + ":" + String(s);
  return st;
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
    {
      broadcast_twice = true;
    }
    else if (RSSI_2 > RSSI_TRE_HI and RSSI_1 > RSSI_TRE_HI and RSSI_0 > RSSI_TRE_HI and rssi > RSSI_TRE_HI)
    {
      broadcast_twice = false;
    }
#endif



    // Read the actual packet length. readString() stops at a timeout and
    // trim() would silently corrupt binary Disaster Radio datagrams.
    if (packetSize > 0 && packetSize <= 255) {
      char packet[255];
      size_t count = LoRa.readBytes(packet, packetSize);
      LoRaData = "";
      LoRaData.reserve(count);
      for (size_t n = 0; n < count; n++)
        LoRaData += packet[n];
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
          ESP_BT_println(tempstring);
        tempstring = "";
#else
        Serial.println(LoRaData);
        cyclestrings(LoRaData);
        if (has_bluetooth_been_initialized)
          ESP_BT_println(LoRaData);
#endif
        if (LoRaData.startsWith(":SYS:") == false) // avoid spamming
          LoraSendAndUpdate(LoRaData);

#ifdef IRC_SERVER
        irc_broadcast( LoRaData);
#endif
      }
    }
    dodisplay = true;
  }
}

#ifdef GPS_SERIAL_1
char gpsnom[18];
unsigned long next_utc_to_send = 0;
void DoUTC(bool changed_pseudosecond) // harvests UTC time since that's all we care about. that means keep the last 18 characters.
{
  if (has_serial_been_initialized == false)
    return;

  if (Serial1.available())
  {
    for (byte i = 18; i > 1; i--)
    {
      gpsnom[i - 1] = gpsnom[i - 2];
    }
    gpsnom[0] = Serial1.read();

    //GxRMC or GxGGA string has UTC date right in front, so eat it up
    if (gpsnom[17] == '$' and (gpsnom[14] == 'R' or gpsnom[14] == 'G') and (gpsnom[13] == 'M' or gpsnom[13] == 'G') and (gpsnom[12] == 'C' or gpsnom[12] == 'A') and (gpsnom[0] == 'A' or gpsnom[4] == 'A' or gpsnom[0] == 'V' or gpsnom[4] == 'V'))
    {
      UTC_Seconds =    ((gpsnom[0] == 'A' or gpsnom[4] == 'A') ? 200000 : 100000) + // easier to parse if we know how many leading zeros we have. 2 = A , 1 = V
                       (gpsnom[10] - '0') * 36000
                       + (gpsnom[9] - '0') * 3600
                       + (gpsnom[8] - '0') * 600
                       + (gpsnom[7] - '0') * 60
                       + (gpsnom[6] - '0') * 10
                       + (gpsnom[5] - '0') * 1; // this is actually seconds from start of day

      if (pseudoseconds > next_utc_to_send)
      {
        String utcstring = fourhex(derpme, spare_id_nibble + 0) + TAG_END_SYMBOL + "UTC:" + String(UTC_Seconds);
        LoraSendAndUpdate(utcstring);
        //cyclestrings(utcstring); // do not spam yourself
#ifdef IRC_SERVER
        //irc_broadcast( utcstring); // do not spam IRC
#endif
        next_utc_to_send = pseudoseconds + (GPS_SERIAL_1 - 1); // one hour should do it
      }
    }
  }
}
#else
#ifdef WIFI_IS_CLIENT
void DoUTC(bool changed_pseudosecond)// use NTP here maybe? for now, harvests UTC time since that's all we care about
{
  // keep up until the next UTC message
  if (changed_pseudosecond)
  {
    UTC_Seconds++;
    if (UTC_Seconds > 286399) // number of seconds in a day (A fix)
      UTC_Seconds = 200000;
    else if (UTC_Seconds > 186399 and UTC_Seconds < 200000) // number of seconds in a day (V fix)
      UTC_Seconds = 100000;
    else if (UTC_Seconds < 99000) // invalid, so leave it alone
      UTC_Seconds = 0;
  }
}
#else
void DoUTC(bool changed_pseudosecond)// harvests UTC time since that's all we care about
{
  // keep up until the next UTC message
  if (changed_pseudosecond)
  {
    UTC_Seconds++;
    if (UTC_Seconds > 286399) // number of seconds in a day (A fix)
      UTC_Seconds = 200000;
    else if (UTC_Seconds > 186399 and UTC_Seconds < 200000) // number of seconds in a day (V fix)
      UTC_Seconds = 100000;
    else if (UTC_Seconds < 99000) // invalid, so leave it alone
      UTC_Seconds = 0;
  }
}
#endif
#endif

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
      if ((unsigned char)receivedChars[i] > 127)
        highbytenums = highbytenums + 50;
      receivedChars[i] = 0;
    }
    readytosend = false; // including duplicate/short messages

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
        ESP_BT_println(serstr);
      LoraSendAndUpdate(serstr);
      cyclestrings(serstr);

#ifdef IRC_SERVER
      irc_broadcast( serstr);
#endif

      antispam_timestamp1 = pseudoseconds + ANTISPAM_TIME_SERIAL;
      dodisplay = true;
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
    readytosen2 = false; // including duplicate/short messages
    if (LongChecksum(serstr) != LastThingISentViaLora_0 && (serstr.length() > 5))
    {
      LoraSendAndUpdate(serstr);
      antispam_timestamp2 = pseudoseconds + ANTISPAM_TIME_SERIAL;
      Serial.println(serstr);
      dodisplay = true;
      cyclestrings(serstr);
#ifdef IRC_SERVER
      irc_broadcast( serstr);
#endif
    }
  }
}


#ifndef NO_BINARY_SUPPORT
// the binary file must be imported as a header using bin2c because I don't want a filesystem on this build, it's too much overhead rn
void SendBinaryFile(String contenttype, const unsigned char file[], const long int filesize)
{
  // this obviously should be its own function since we are using it more than once.
  wificlient1.print("HTTP/1.1 200 OK\r\n"
                    "Content-Description: File Transfer\r\n"
                    "Content-type:");
  wificlient1.print(contenttype);
  wificlient1.print("\r\nConnection: close\r\n"
                    "Content-Transfer-Encoding: binary\r\n"
                    "Content-Length: ");
  wificlient1.println(filesize);
  wificlient1.println();
  char ch;
  byte stuff[32]; // 32 bytes is a surprisingly good compromise between speed and memory overhead
  byte j = 0;
  int i;
  for (i = 0; i < filesize; i++)
  {
    stuff[j] = (byte)file[i];
    if (++j == 32)
    {
      wificlient1.write(stuff, 32);
      j = 0;
    }
    if (i == (filesize - 1))
    {
      wificlient1.write(stuff, j);
    }

    // attempt slow operation while the data gets sent; other wifi clients will just have to wait and that's all there is to it
    if (i % 64 == 0)
    {
      PetTheWatchdog();

      ReadFromStream(Serial, receivedChars, charcounter, readytosend, has_serial_been_initialized, 15);
      ReadFromStream(ESP_BT, receivedChar2, charcounte2, readytosen2, has_bluetooth_been_initialized, 14);
    }
    if (i % 512 == 0)
      SeeIfAnythingOnRadio();
    if (i % 512 == 128)
      DoDisplayIfItExists();
    if (i % 512 == 256)
      UpdateCharCounterIfDisplayExists();
    if (i % 512 == 384)
      SendSerialIfReady();
  }
  wificlient1.flush();
  wificlient1.println();
}
#endif


void Start_LORA(bool trydisplay)
{
  if (has_lora_been_initialized)
    return;

  Serial.println("4.0");
  Stop_LORA();
  Serial.println("4.1");

#ifdef LORA32V3
  // RadioLib adapter initializes SPI and the SX1262 in begin().
#else
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);
#endif

#ifdef LORA32V3
  // This adapter retains settings and applies them during SX1262 begin().
  LoRa.setSpreadingFactor(CELLSOL_LORA_SF);
  LoRa.setSignalBandwidth(CELLSOL_LORA_BW_HZ);
#endif

  //  LoRa.crc(); // we would rather get a damaged packet than no packet at all; maybe some of it will be readable.
  Serial.println("4.5");

  //  bool changeclock = (getCpuFrequencyMhz() < CLKFREQ);

  //  if (changeclock)
  //    setCpuFrequencyMhz(CLKFREQ);

  Serial.println("4.6");
  has_lora_been_initialized = LoRa.begin(BAND);
  if (has_lora_been_initialized == false)
    has_lora_been_initialized = LoRa.begin(BAND);
  if (has_lora_been_initialized == false)
    has_lora_been_initialized = LoRa.begin(BAND);
  if (has_lora_been_initialized == false)
    has_lora_been_initialized = LoRa.begin(BAND);

  Serial.println("4.7");
  if (has_lora_been_initialized)
  {
#ifndef LORA32V3
    // SX127x registers are valid only after begin(), which also resets the
    // radio. The old pre-begin setters did not survive that reset.
    LoRa.setSpreadingFactor(CELLSOL_LORA_SF);
    LoRa.setSignalBandwidth(CELLSOL_LORA_BW_HZ);
#endif
  }
  //  if (changeclock)
  //    setCpuFrequencyMhz(CLKFREQ_LOW);

  if (has_lora_been_initialized == false)
  {
    LoRa.end();
#ifdef LORA32V3
    SPI.end();
#endif
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
    LoRa.setTxPower(20); // use the driver's PA boost setting on SX127x
    if (pendingOldNodeID.length()) {
      const String activeID = fourhex(derpme, spare_id_nibble);
      if (pendingOldNodeID != activeID)
        LoraSendAndUpdate(pendingOldNodeID + TAG_END_SYMBOL + ">>>" + activeID);
      Preferences prefs;
      if (prefs.begin("cellsol", false)) {
        if (prefs.remove("idprev")) pendingOldNodeID = "";
        prefs.end();
      }
    }
  }
}

void Start_UART()
{
  Serial.begin(UART_BAUD_RATE);
#ifdef GPS_SERIAL_1
  Serial1.begin(9600, SERIAL_8N1, 34, 12);
#endif
  has_serial_been_initialized = true;

  Serial.println("UART up\n");
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
  if (has_bluetooth_been_initialized)
    return;
  if (ipstring_b.length() < 2)
    ipstring_b = settings.apPrefix + String("BT") + String(derpme);

#ifdef LORA32V3
  has_bluetooth_been_initialized = ESP_BT.beginAndSetupBLE(ipstring_b.c_str());
#else
  has_bluetooth_been_initialized = ESP_BT.begin(ipstring_b);
#endif
}

void Stop_BT()
{
  if (!has_bluetooth_been_initialized)
    return;
  ESP_BT.end();
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
#ifdef IRC_SERVER
    irc_broadcast( tempstring);
#endif
  }

  ReadBatteryADC(true);
}


void DoBasicSteps()
{
  PetTheWatchdog();
  ReadBatteryADC(false);
#ifdef LORA32V3
  if (has_bluetooth_been_initialized)
    ESP_BT.poll();
#endif

  if (has_lora_been_initialized)
  {
    SeeIfAnythingOnRadio();

#ifdef REPEAT_UNTIL_ACK
    if (LoRaData.length() >= 1) // if no packet, don't change ack state, just broadcast again.
    {
      got_lora_ack = (LoRaData.equals(message_to_repeat));
    }
    if (!got_lora_ack)
    {
      if ((millis() - last_repeat_time) > REPEAT_DELAY and message_to_repeat.length() > 1)
      {
        LoraSendAndUpdate(message_to_repeat);
      }
    }
#endif
  }


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

#ifdef BATT_ADC
  if (currbatterylevel < (BATT_TOO_LOW_FOR_ANYTHING))
    SleepLowBatt();
#endif

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
/**
   END LORA32 RADIO BLOCK
*/



/********************************************
   DISPLAY BLOCK: USED FOR HANDLING OLED
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
void led(boolean onoff)
{
#ifdef USER_LED_PIN
  digitalWrite(USER_LED_PIN, onoff);
#endif
}

void displayonoff(bool onoff)
{
#ifdef NODISPLAY
  displayenabled = false;
#else
  if (onoff)
  {
    if (has_display_been_initialized == false)
      InitDisplayTryAll();
    if (!displayexists) return;
    display.ssd1306_command(SSD1306_DISPLAYON);
    displayenabled = true;
  }
  else
  {
    if (!displayenabled) return;
    if (has_display_been_initialized && displayexists)
    {
      display.clearDisplay();
      display.display();
      display.ssd1306_command(SSD1306_DISPLAYOFF);
    }
    displayenabled = false;
  }
#endif
}

int displayswitch = 0; // display switch if we are in hybrid mode

void DoDisplayIfItExists()
{
#ifndef NODISPLAY
  // Client/gateway mode used to set DISPLAY_INTERVAL to ~23 days at compile
  // time. Now that modes are runtime-selectable, keep that policy at runtime:
  // upstream-powered gateway modes show the display continuously, while
  // local AP/BT modes wake for the button or an explicit display request.
  const bool keepDisplayOn = UsesUpstreamWifi();
#ifdef user_button_display
  const bool displayRequested = (digitalRead(USER_BUTTON_PIN) == LOW) || display_on_ping;
#else
  const bool displayRequested = display_on_ping;
#endif
  if (displayRequested || (keepDisplayOn && (!displayenabled || !has_display_been_initialized)))
  {
    const bool wasOff = !displayenabled || !has_display_been_initialized;
    if (has_display_been_initialized == false) // try without hard reset first
      InitDisplayTryAll();
    displayonoff(true);
    if (displayRequested || wasOff) dodisplay = true;
    display_on_ping = false;
  }
  if (displayRequested)
    disptimeout = millis() + DISPLAY_INTERVAL;
  if (!keepDisplayOn && displayenabled && (int32_t)(millis() - disptimeout) >= 0)
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
        if (UsesUpstreamWifi()) {
          if (activeMode == CELLSOL_AP_STA_BT)
            display.println((++displayswitch % 2) ? ipstring_c : ipstring_a);
          else
            display.println(ipstring_c);
        } else {
          display.println(ipstring_a);
        }
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

void UpdateCharCounterIfDisplayExists()
{
#ifndef NODISPLAY
  if (!displayenabled) return;
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
#ifdef MINDISPLAY
    if (pseudoseconds > 3)
    {
      oled.clearDisplay();
      oled.sendCommand(SSD1306_Display_Off_Cmd);
    }
#endif
    led(true);
    DoBasicSteps();
    lploops = 0;
    led(false);
  }
}


/**
   END DISPLAY BLOCK
*/


/********************************************
   POWER MANAGEMENT BLOCK: USED FOR HANDLING BATTERY AND SLEEP
*/
#ifdef BATT_ADC
void ReadBatteryADC(bool force)
{
  if (force || (int32_t)(millis() - battimeout) >= 0)
  {
    Watchdog(true);
    if (currbatterylevel > 0)
      lastbatterylevel = currbatterylevel;
    currbatterylevel = (analogRead(BATT_ADC) + analogRead(BATT_ADC) + analogRead(BATT_ADC) + analogRead(BATT_ADC)) / 4;
    #ifdef LORA32V3
    currbatterylevel = currbatterylevel * 3 / 2;
    #endif
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

void SleepLowBatt()
{
#ifdef BATT_ADC
  tempstring = ":SYS:SLEEP " + String(derpme) + ":" + status_string();
  Watchdog(false);
  led(true);
  if (low_batt_announce)
  {
    LoraSendAndUpdate(tempstring);
    cyclestrings(tempstring);
#ifdef IRC_SERVER
    irc_broadcast( tempstring);
#endif
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
#endif
}

/**
   END POWER MANAGEMENT BLOCK
*/
/**
   OPERATIONS BLOCK - this is where logic for pylon goes
*/

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
#ifdef LORA32V3
  if (has_bluetooth_been_initialized)
    ESP_BT.poll();
#endif


#ifdef BATT_ADC
  ReadBatteryADC(false);
  if (currbatterylevel < (BATT_TOO_LOW_FOR_ANYTHING))
    SleepLowBatt();
  if (currbatterylevel < (BATT_HIGH_ENOUGH_FOR_FULL_POWER - BATT_HYSTERESIS_POWER))
  {
    LowPowerLoop();
    HighPowerSetup(true);
  }
#endif
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
#ifdef LORA32V3
  if (has_bluetooth_been_initialized)
    ESP_BT.poll();
#endif

#ifdef BATT_ADC
  ReadBatteryADC(false);
  if (currbatterylevel < (BATT_TOO_LOW_FOR_ANYTHING))
    SleepLowBatt();
  if (currbatterylevel < (BATT_HIGH_ENOUGH_FOR_FULL_POWER - BATT_HYSTERESIS_POWER))
  {
    LowPowerLoop();
    HighPowerSetup(true);
  }
#endif

#ifdef MODEFLIP // switch between full AP and repeater according to a timer, but only if there are no wifi clients.
  if ((activeMode == CELLSOL_AP) && (MODEFLIP < 60000) && ((uint32_t)(millis() - modefliptime) > MODEFLIP))
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
      while ((uint32_t)(millis() - modefliptime) < 60000 && derpp)
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
  ReadFromStream(ESP_BT, receivedChar2, charcounte2, readytosen2, has_bluetooth_been_initialized, 14);
  DoDisplayIfItExists();
  UpdateCharCounterIfDisplayExists();
#ifdef user_button_display
  // A short PROG press wakes the OLED. Only a deliberate 3-second hold
  // forces an upstream WiFi reconnect, once per press.
  static uint32_t reconnectPressedAt = 0;
  static bool reconnectTriggered = false;
  if (UsesUpstreamWifi() && digitalRead(USER_BUTTON_PIN) == LOW) {
    if (reconnectPressedAt == 0)
      reconnectPressedAt = millis();
    else if (!reconnectTriggered && (uint32_t)(millis() - reconnectPressedAt) >= 3000) {
      reconnectTriggered = true;
      ConnectToUpstreamWifi(100);
    }
  } else {
    reconnectPressedAt = 0;
    reconnectTriggered = false;
  }
#endif
  SendSerialIfReady();

  BlinkMeWhen();
}

/**
   SETUP METHOD - Called first by the ESP32.
*/
void setup() {

  Serial.begin(9600);
  Network.begin(); // unified Network API, required before network interfaces
  LoadCellSolSettings();

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


  BuildNicknameTags();


  if (lowpowerstart)
  {
    LowPowerLoop();
  }

  HighPowerSetup(true);


}

bool currentlyserving = false;

void BasicWhileDelay(int pausetime)
{
  const uint32_t started = millis();
  while ((uint32_t)(millis() - started) < (uint32_t)pausetime)
  {
    DoBasicSteps(); // DoWifiSteps();
  }
}

int failed_irc_connections = 0;
bool CurrentlyConnecting = false;
void ConnectToUpstreamWifi(int pausetime)
{
  if (!UsesUpstreamWifi() || server.hasClient() || CurrentlyConnecting || wificlient1.connected() || currentlyserving)
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

  const bool hybrid = activeMode == CELLSOL_AP_STA_BT;
  WiFi.mode(hybrid ? WIFI_MODE_APSTA : WIFI_MODE_STA);
  WiFi.setTxPower(wifilevels[settings.wifiPower]);
  BasicWhileDelay(pausetime);
  if (!settings.dhcp)
    WiFi.config(Client_IP, Client_Gateway, Client_Subnet, Client_Gateway, Client_Gateway);
  if (hybrid) {
    WiFi.softAPConfig(AP_IP, AP_IP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(ssid.c_str(), NULL, 1 + (derpme % 11), false, 8);
    dnsServer.start(DNS_PORT, "*", AP_IP);
  }
  BasicWhileDelay(pausetime);
  if (settings.upstreamPassword.length())
    WiFi.begin(settings.upstreamSSID.c_str(), settings.upstreamPassword.c_str());
  else
    WiFi.begin(settings.upstreamSSID.c_str());
  BasicWhileDelay(pausetime);
  const uint32_t started = millis();
  led(false);
  while (WiFi.status() != WL_CONNECTED && (uint32_t)(millis() - started) < 12000)
  {
    led(true);
    BasicWhileDelay(1);
    led(false);
    BasicWhileDelay(pausetime);
  }
  if (WiFi.status() != WL_CONNECTED) {
    // A typo in the upstream credentials must not lock the user out of the
    // settings page. Run a temporary local AP until the next power cycle.
    Serial.println(":SYS:Upstream WiFi unavailable; recovery AP enabled");
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(AP_IP, AP_IP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(ssid.c_str(), NULL, 1 + (derpme % 11), false, 8);
    dnsServer.start(DNS_PORT, "*", AP_IP);
    activeMode = CELLSOL_AP_BT;
    ipstring = AP_IP.toString();
    server.begin();
    CurrentlyConnecting = false;
    Watchdog(true);
    return;
  }

  Client_IP = WiFi.localIP();

  LastReconnect = millis();
  ipstring = ipstring_a;//AP_IP.toString();//"Gateway";//Client_IP.toString()+"G";
  dodisplay = true;
  BasicWhileDelay(pausetime);
  server.begin();
  CurrentlyConnecting = false;
  failed_irc_connections = 0;
}



#ifdef MINDISPLAY
void DoMinDisplay()
{
  dodisplaybuf = false;
  dodisplay = false;
#ifdef USER_BUTTON_PIN
  if (digitalRead(USER_BUTTON_PIN) == true)
    return;
#endif
  oled.sendCommand(SSD1306_Display_On_Cmd);
  oled.clearDisplay();              // Clear screen
  oled.setTextXY(0, 0);             // Set cursor position, start of line 0
  oled.putString(String("CellSol") + VERSIONSTRING "(" PYLONMODEL ":" + PylonTypeLabel());
  oled.setTextXY(1, 0);             // Set cursor position, start of line 1
  oled.putString(wifimode ? ipstring_a : "Bluetooth");
  if (UsesUpstreamWifi()) oled.putString(wifimode ? ipstring_c : "");
  oled.setTextXY(2, 0);
  oled.putString(string_rx[2].substring(0, 24));
  oled.setTextXY(3, 0);
  oled.putString(string_rx[2].substring(25, 49));
  oled.setTextXY(4, 0);
  oled.putString(string_rx[1].substring(0, 24));
  oled.setTextXY(5, 0);
  oled.putString(string_rx[1].substring(25, 49));
  oled.setTextXY(6, 0);
  oled.putString(string_rx[0].substring(0, 24));
  oled.setTextXY(7, 0);
  oled.putString(string_rx[0].substring(25, 49));
}
#endif


void HighPowerSetup(bool echo)
{
  Start_UART();

  Watchdog(false);
  setCpuFrequencyMhz(CLKFREQ); //Set CPU clock to 80MHz fo example
  led(true);
  ReadBatteryADC(true);
  lowpowerstart = false;


  activeMode = settings.mode;
  wifimode = activeMode != CELLSOL_BT;

  // Resolve saved upstream IPv4 configuration before initiating WiFi.
  IPAddress configuredIP, configuredGateway, configuredSubnet;
  if (configuredIP.fromString(settings.clientIP) &&
      configuredGateway.fromString(settings.gatewayIP) &&
      configuredSubnet.fromString(settings.subnet)) {
    Client_IP = configuredIP;
    Client_Gateway = configuredGateway;
    Client_Subnet = configuredSubnet;
  }

  low_batt_announce = POWER_STATE_CHANGE_ANNOUNCE;

  if (enablecomstring == "")
    enablecomport = true; // for tbeam etc.

  Start_LORA(false);

  //reset OLED display via software
  //initialize OLED
  DoBasicSteps();
  ResetDisplayViaPin();

  //  InitDisplay(true);
  //  displayonoff(true);

  DoBasicSteps();
  server.setNoDelay(true);
  wificlient1.setTimeout(2);
  server.setTimeout(2);

#ifdef USER_BUTTON_PIN // true = wifi, false = bluetooth
  // PROG at boot: standard AP -> Bluetooth (old behavior); any saved
  // non-AP mode -> temporary AP. This makes bad NVS choices recoverable.
  int j = 0;
  for (int i = 0; i < 50; i++) {
    BasicWhileDelay(1);
    if (digitalRead(USER_BUTTON_PIN)) j++;
  }
  if (j <= 24) activeMode = activeMode == CELLSOL_AP ? CELLSOL_BT : CELLSOL_AP;
#endif
  led(false);
  wifimode = activeMode != CELLSOL_BT;

  if (activeMode == CELLSOL_BT) {
    WiFi.mode(WIFI_OFF);
    server.stop();
    dnsServer.stop();
    ipstring = settings.apPrefix + String("BT") + String(derpme);
    esp_wifi_stop();
    Start_BT();
    dodisplay = true;
  } else {
    if (activeMode == CELLSOL_AP_BT || UsesUpstreamWifi()) Start_BT();
    else if (has_bluetooth_been_initialized) Stop_BT();

    if (UsesUpstreamWifi()) {
      ConnectToUpstreamWifi(100);
    } else {
      WiFi.mode(WIFI_AP);
      WiFi.setTxPower(wifilevels[settings.wifiPower]);
      WiFi.softAPConfig(AP_IP, AP_IP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(ssid.c_str(), NULL, 1 + (derpme % 11), false, 8);
      dnsServer.start(DNS_PORT, "*", AP_IP);
    }
    server.stop();
    server.begin();
  }
  tempstring = String(":SYS:(" PYLONMODEL ";") + PylonTypeLabel() + " " + String(derpme) + ":" + status_string();
  if (low_batt_announce)
  {
    LoraSendAndUpdate(tempstring);
    cyclestrings(tempstring);
#ifdef IRC_SERVER
    irc_broadcast( tempstring);
#endif
  }
  if (echo)
    Serial.println(tempstring);

  // reset these
  AP_IP = IPAddress(192, 168, AP_IP[2], 1);
  IP = AP_IP;
  ipstring = IP.toString();
  ipstring_a = AP_IP.toString();
  ipstring_c = Client_IP.toString();

  //Serial.println(":SYS:IP " + ipstring_a + " " + ipstring_c + " " + ipstring + " " + lwc.toString());

  // Give every V2 boot a visible status screen. Gateway modes then keep it
  // on; battery-operated AP/BT modes sleep the OLED after DISPLAY_INTERVAL.
  display_on_ping = true;

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

      //                                   012345678901234567890
      //              012345678901234567890
      if (UsesUpstreamWifi()) {
        string_rx[2] = ":SYS: Upstream WiFi: " + settings.upstreamSSID;
        string_rx[1] = String(settings.dhcp ? "(D)Client IP: " : "Client IP: ") + Client_IP.toString();
        string_rx[0] = "Router: " + Client_Gateway.toString();
      } else if (activeMode == CELLSOL_AP_BT) {
      //                                   012345678901234567890
      //              012345678901234567890
        string_rx[2] = ":SYS: This unit also supports Bluetooth";
        string_rx[1] = "You can download a BT app over WiFi";
        string_rx[0] = "and reconnect over Bluetooth.";
      } else {
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
      }
    }
  }
#endif



#ifdef MINDISPLAY
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  Wire.begin(OLED_SDA, OLED_SCL);
  oled.init();                      // Initialze SSD1306 OLED display
  DoMinDisplay();
#endif
}

/****************************************************************
   WEBSITE SERVING CODE.
   This includes a full HTTP server that runs as a serial UART
   It is coded and coupled explicitly for performance
   K. Borri
 ****************************************************************/

void send_html_header(int redir = -1) // 0: redirect to root. positive: refresh every x seconds
{
  PetTheWatchdog();
  wificlient1.print("<!DOCTYPE html><html><head>"
                    "<title>CellSol WiFi Pylon " VERSIONSTRING "</title>"
                    "<meta http-equiv=\"Content-type\" content=\"text/html;charset=UTF-8\">");
  if (redir == 0)
    wificlient1.print("<meta http-equiv=\"refresh\" content=\"0;URL='/'\" />");
  else if (redir > -1 && redir < 99)
    wificlient1.print("<meta http-equiv=\"refresh\" content=\"" + String(redir) + "\">");
  /*
    wificlient1.println("<meta purpose=\"Deus Nolens Exitus\" name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"icon\" href=\"data:,\">"
                   "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: " + TEXT_ALIGN_STRING + ";}.button { background-color: #4CAF50; border: none; color: white; padding: 0px 0px; text-decoration: none; font-size: 20px; margin: 0px; cursor: pointer;}.button2 {background-color: #555555;}</style>"
                   "</head>");
  */
  wificlient1.print("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"icon\" href=\"data:,\">"

                    "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: ");
  wificlient1.print(TEXT_ALIGN_STRING);
  wificlient1.println(";}.button {border: none; color: white; padding: 0px 0px; text-decoration: none; font-size: 1.2em; margin: 0px; cursor: pointer;}.button2 {}</style></head>");
}
bool client_on_lan = true;
String hoststring = "/";

void send_string_by_itself (String st)
{
  wificlient1.println("HTTP/1.1 200 OK");
  wificlient1.println("Content-type:text/plain");
  wificlient1.println("Connection: close");
  wificlient1.println();
  //  wificlient1.println(encodeHtml(st));
  wificlient1.println(st);
  wificlient1.println();
}
void send_redirect_to_main(bool doip)
{
  PetTheWatchdog();
  wificlient1.println("HTTP/1.1 302 Found");
  if (doip)
  {
    if (client_on_lan)
      wificlient1.println("Location: http://" + ipstring); //ipstring
    else
      wificlient1.println("Location: http://" + hoststring); //ipstring
  }
  else
    wificlient1.println("Location: /");// http://" + ipstring + "/");
  wificlient1.println("Connection: close");
  wificlient1.println();
}

#ifdef TX_IFRAME
void send_form_iframe()
{
  send_ok_response();
  send_html_header();
  wificlient1.println("<body>"
                      "<small><small>TX&gt;</small></small> <input id=\"msgfield\" type=\"text\" maxlength=\"160\" name=\"input1\"><button onClick=sendMsg() value=\"Send\">Send</button><br>"
                      "</body></html>");
}
#endif
void send_ok_response()
{
  PetTheWatchdog();
  wificlient1.println("HTTP/1.1 200 OK");
  wificlient1.println("Content-type:text/html");
  wificlient1.println("Connection: close");
  wificlient1.println();
}

// The settings URL is deliberately unadvertised, not password protected.
// Keep its path in config.h; never put upstream WiFi passwords into GET URLs.
#define MAX_SETTINGS_POST 2048

String htmlAttribute(String value) {
  value.replace("&", "&amp;");
  value.replace("<", "&lt;");
  value.replace(">", "&gt;");
  value.replace("\"", "&quot;");
  value.replace("'", "&#39;");
  return value;
}

void settingsField(const char *label, const char *name, const String &value,
                   int maxLength, bool password = false) {
  wificlient1.print("<p><label>");
  wificlient1.print(label);
  wificlient1.print(" <input name=\"");
  wificlient1.print(name);
  wificlient1.print("\" type=\"");
  wificlient1.print(password ? "password" : "text");
  wificlient1.print("\" maxlength=\"");
  wificlient1.print(maxLength);
  wificlient1.print("\" value=\"");
  wificlient1.print(htmlAttribute(value));
  wificlient1.println("\"></label></p>");
}

void settingsCheck(const char *label, const char *name, bool checked) {
  wificlient1.print("<p><label><input type=\"checkbox\" name=\"");
  wificlient1.print(name);
  wificlient1.print("\" value=\"1\"");
  if (checked) wificlient1.print(" checked");
  wificlient1.print("> ");
  wificlient1.print(label);
  wificlient1.println("</label></p>");
}

void settingsModeOption(uint8_t id, const char *label) {
  wificlient1.print("<option value=\"");
  wificlient1.print(id);
  wificlient1.print("\"");
  if (settings.mode == id) wificlient1.print(" selected");
  wificlient1.print(">");
  wificlient1.print(label);
  wificlient1.println("</option>");
}

void sendSettingsPage(const String &message = "") {
  send_ok_response();
  send_html_header();
  wificlient1.println("<body style=\"max-width:44em;margin:1em auto;padding:0 1em;text-align:left\">"
                      "<h2>CellSol settings</h2><p>Changes are stored in flash and take effect after saving and restarting. "
                      "PROG held at boot selects Bluetooth from standard AP mode, or a temporary recovery AP from any other mode.</p>"
                      "<p>Anyone who knows this URL can change these values. Use on a trusted local network.</p>");
  if (message.length()) {
    wificlient1.print("<p><strong>");
    wificlient1.print(htmlAttribute(message));
    wificlient1.println("</strong></p>");
  }
  wificlient1.print("<form action=\"/");
  wificlient1.print(SETTINGS_PAGE_PATH);
  wificlient1.println("/save\" method=\"POST\" accept-charset=\"utf-8\">"
                      "<fieldset><legend>Node identity</legend><p>The pylon owns IDs ending in 0; "
                      "the final hex digit is reserved for attached WiFi, Bluetooth, and serial clients.</p>");
  wificlient1.print("<p>Current pylon ID: <code>");
  wificlient1.print(fourhex(derpme, spare_id_nibble));
  wificlient1.println("</code></p>");
  settingsField("Node ID (4 hex digits; last digit becomes 0; 0000 = automatic)",
                "nodeid", settings.nodeID.length() ? settings.nodeID : "0000", 4);
  wificlient1.println("<p>An ID change broadcasts <code>oldID:&gt;&gt;&gt;newID</code> "
                      "over LoRa after restart. Manually chosen IDs do not use battery noise.</p>"
                      "</fieldset><fieldset><legend>Local connection</legend>"
                      "<p><label>Operating mode <select name=\"mode\">");
  settingsModeOption(CELLSOL_AP, "WiFi AP (PROG button can select Bluetooth)");
  settingsModeOption(CELLSOL_BT, "Bluetooth only (hold PROG at boot for recovery AP)");
  settingsModeOption(CELLSOL_AP_BT, "WiFi AP + Bluetooth");
  settingsModeOption(CELLSOL_STA_BT, "Upstream WiFi client + Bluetooth");
  settingsModeOption(CELLSOL_AP_STA_BT, "WiFi AP + upstream WiFi client + Bluetooth");
  wificlient1.println("</select></label></p>");
  wificlient1.print("<p>Current local AP address: <code>");
  wificlient1.print(AP_IP.toString());
  wificlient1.println("</code></p>");
  settingsField("Local AP third IPv4 octet (0-254; blank = automatic)",
                "apoct", settings.apThirdOctet < 0 ? "" : String(settings.apThirdOctet), 3);
  settingsField("AP/Bluetooth name prefix", "aproot", settings.apPrefix, 17);
  settingsField("WiFi transmit power (0 to 11)", "power", String(settings.wifiPower), 2);
  wificlient1.println("</fieldset><fieldset><legend>Upstream WiFi / router</legend>");
  settingsField("Upstream SSID", "usssid", settings.upstreamSSID, 32);
  settingsField("Upstream password (leave blank to keep current)", "uspass", "", 63, true);
  settingsCheck("Clear stored upstream password", "clearpass", false);
  settingsCheck("Use DHCP", "dhcp", settings.dhcp);
  settingsField("Static client IPv4", "clientip", settings.clientIP, 15);
  settingsField("Router / gateway IPv4", "gateway", settings.gatewayIP, 15);
  settingsField("Subnet mask", "subnet", settings.subnet, 15);
  wificlient1.println("</fieldset><fieldset><legend>IRC gateway</legend>");
  settingsCheck("Enable IRC relay (upstream WiFi required)", "ircon", settings.ircEnabled);
  settingsField("IRC server", "ircserver", settings.ircServer, 64);
  settingsField("IRC port", "ircport", String(settings.ircPort), 5);
  settingsField("IRC channel (without #)", "ircchan", settings.ircChannel, 48);
  settingsField("IRC nick prefix", "ircnick", settings.ircNick, 20);
  settingsField("Forward-to-radio prefix", "ircprefix", settings.ircPrefix, 16);
  settingsCheck("Append node hex tag to IRC channel", "irctag", settings.ircChannelTag);
  settingsCheck("Relay other CellSol IRC pylons without a text prefix", "ircpeer", settings.ircForwardPylons);
  wificlient1.println("</fieldset><p><button type=\"submit\">Save settings and restart</button></p>"
                      "</form><p><a href=\"/\">Back to chat</a></p></body></html>");
}

// Exact form key matching matters: 'irc' must not match 'ircon', etc.
String formValue(const String &body, const char *key) {
  String needle = String(key) + "=";
  int start = 0;
  while (start < (int)body.length()) {
    int end = body.indexOf('&', start);
    if (end < 0) end = body.length();
    if (body.substring(start, start + needle.length()) == needle) {
      String encoded = body.substring(start + needle.length(), end);
      char decoded[MAX_SETTINGS_POST + 1];
      encoded.toCharArray(decoded, sizeof(decoded));
      decode_in_place(decoded);
      return String(decoded);
    }
    start = end + 1;
  }
  return "";
}

bool invalidIRCName(const String &value) {
  for (size_t i = 0; i < value.length(); i++) {
    if ((uint8_t)value[i] <= 32 || value[i] == ',') return true;
  }
  return false;
}

bool readSettingsPostBody(const String &requestHeader, String &body) {
  int at = requestHeader.indexOf("Content-Length:");
  if (at < 0) return false;
  int end = requestHeader.indexOf('\n', at);
  String countText = requestHeader.substring(at + 15, end >= 0 ? end : requestHeader.length());
  countText.trim();
  long count = countText.toInt();
  if (count <= 0 || count > MAX_SETTINGS_POST) return false;
  body.reserve(count);
  uint32_t started = millis();
  while (body.length() < (size_t)count && (uint32_t)(millis() - started) < 3000) {
    if (wificlient1.available()) {
      body += char(wificlient1.read());
      started = millis();
    } else {
      DoBasicSteps();
      delay(1);
    }
    PetTheWatchdog();
  }
  return body.length() == (size_t)count;
}

String updateSettingsFromForm(const String &body) {
  CellSolSettings next = settings;
  String mode = formValue(body, "mode");
  if (mode.length() != 1 || mode[0] < '0' || mode[0] > '4') return "Invalid mode";
  next.mode = mode[0] - '0';
  next.nodeID = formValue(body, "nodeid");
  next.nodeID.trim();
  if (next.nodeID.length()) {
    uint8_t configuredOctet, configuredPrefix;
    if (!ParsePylonNodeID(next.nodeID, configuredOctet, configuredPrefix))
      return "Node ID must have four hexadecimal digits (or be blank for automatic)";
    next.nodeID.toLowerCase();
    next.nodeID.setCharAt(3, '0'); // mask the low nibble, do not reject it
    if (next.nodeID == "0000") next.nodeID = ""; // revert to generated identity
  }
  String apOctet = formValue(body, "apoct");
  apOctet.trim();
  next.apThirdOctet = -1;
  if (apOctet.length()) {
    if (apOctet.length() > 3) return "Invalid local AP octet";
    for (size_t i = 0; i < apOctet.length(); i++)
      if (apOctet[i] < '0' || apOctet[i] > '9') return "Invalid local AP octet";
    if (apOctet.toInt() > 254) return "Local AP octet must be between 0 and 254";
    next.apThirdOctet = apOctet.toInt();
  }
  next.apPrefix = formValue(body, "aproot");
  next.upstreamSSID = formValue(body, "usssid");
  String password = formValue(body, "uspass");
  if (formValue(body, "clearpass") == "1") next.upstreamPassword = "";
  else if (password.length()) next.upstreamPassword = password;
  next.dhcp = formValue(body, "dhcp") == "1";
  next.clientIP = formValue(body, "clientip");
  next.gatewayIP = formValue(body, "gateway");
  next.subnet = formValue(body, "subnet");
  String power = formValue(body, "power");
  if (!power.length() || power.length() > 2) return "Invalid WiFi power";
  for (size_t i = 0; i < power.length(); i++) if (power[i] < '0' || power[i] > '9') return "Invalid WiFi power";
  if (power.toInt() > 11) return "WiFi power must be 0 to 11";
  next.wifiPower = power.toInt();
  next.ircEnabled = formValue(body, "ircon") == "1";
  next.ircServer = formValue(body, "ircserver");
  String port = formValue(body, "ircport");
  if (!port.length() || port.length() > 5) return "Invalid IRC port";
  for (size_t i = 0; i < port.length(); i++) if (port[i] < '0' || port[i] > '9') return "Invalid IRC port";
  long portNum = port.toInt();
  if (portNum < 1 || portNum > 65535) return "IRC port must be 1 to 65535";
  next.ircPort = portNum;
  next.ircChannel = formValue(body, "ircchan");
  if (next.ircChannel.startsWith("#")) next.ircChannel.remove(0, 1);
  next.ircNick = formValue(body, "ircnick");
  next.ircPrefix = formValue(body, "ircprefix");
  next.ircChannelTag = formValue(body, "irctag") == "1";
  next.ircForwardPylons = formValue(body, "ircpeer") == "1";

  const IPAddress nextAP(192, 168,
                         next.apThirdOctet < 0 ? generatedAPOctet : next.apThirdOctet, 1);
  if (!next.apPrefix.length() || next.apPrefix.length() + nextAP.toString().length() > 32 ||
      next.upstreamSSID.length() > 32 || next.upstreamPassword.length() > 63 ||
      next.ircServer.length() > 64 || next.ircChannel.length() > 48 ||
      next.ircNick.length() > 20 || next.ircPrefix.length() > 16)
    return "One or more values are too long (AP SSID including IP must fit 32 bytes)";
  if (next.mode == CELLSOL_STA_BT || next.mode == CELLSOL_AP_STA_BT) {
    if (!next.upstreamSSID.length()) return "Upstream WiFi SSID is required in client mode";
  }
  if (next.ircEnabled && (!next.ircServer.length() || !next.ircChannel.length() || !next.ircNick.length()))
    return "IRC server, channel, and nickname are required";
  if (invalidIRCName(next.ircServer) || invalidIRCName(next.ircChannel) ||
      invalidIRCName(next.ircNick) || next.ircPrefix.indexOf('\r') >= 0 ||
      next.ircPrefix.indexOf('\n') >= 0)
    return "IRC server/channel/nick cannot contain whitespace or commas";
  IPAddress addr;
  if (!addr.fromString(next.clientIP) || !addr.fromString(next.gatewayIP) ||
      !addr.fromString(next.subnet))
    return "Invalid IPv4 address or subnet mask";
  const String oldNodeID = fourhex(derpme, spare_id_nibble);
  const String newNodeID = next.nodeID.length() ? next.nodeID : generatedNodeID;
  const bool identityChanges = oldNodeID != newNodeID ||
                               (settings.nodeID.length() && !next.nodeID.length());
  if (!SaveCellSolSettings(next, identityChanges ? oldNodeID : ""))
    return "Could not write NVS preferences";
  settings = next;
  return ""; // success
}

#ifdef SERVE_FAQ_PAGE
void send_faq_page()
{
  send_ok_response();
  send_html_header();
  wificlient1.println( "<style> html{text-align: start;}</style>"
                       "<body><h1>CellSol WiFi Pylon " VERSIONSTRING " Help</h1>"
                       "<p>The full help/howto is available, when the internet works, at <a href=\"http://f3.to/cellsol/\">http://f3.to/cellsol/</a></p><p>"
                       "<h2>About the Chat</h2>"
                       "<p>By using the chat in the main page, you will be able to communicate with people who are in range of the CellSol network, or people who are using CellSol gateways."
                       "<p>Each message is sent out to neighboring pylons, which can be a few kilometers apart, and rebroadcast."
                       "<p>You can consider this system akin to a single IRC/Discord/Twitch chat channel, except it will work when the internet at large will not."
                       "<p>Any phone with a Bluetooth chat app (Android APK can be downloaded from this very pylon, iPhone or others will have to use their own) can also use the Bluetooth pylons or pocket nodes. They require very little power to operate."
                       "<p>Please note that CellSol does not store your location or identity data, but also please note that there is no encryption for these messages (to make it easier for other systems to interoperate with). This is intended to be used during or after natural disasters, so that should really not be a concern."
                       "<h3>Identifying Users</h3>"
                       "<p>The tag in front of your message is a pseudonymous identifier: it is used to tell people apart. It is four hex digits. (Sorry, no nicknames). Example: 0abc."
                       "<h2>About the Network</h2>"
                       "<p>CellSol is free to use and does not depend on any infrastructure, each pylon is self-contained: just deploy a few of them in an area and you are good to go."
                       "<p>This is a lot like existing LoRa mesh chat systems, except that it's intended to leave repeaters in place."
                       "<p>The mesh topology prioritizes redundancy over speed or cleanliness, so you may occasionally get a garbled message: we try to display those in case their meaning can be understood despite the garbling."
                       "<p>The best use for a standalone repeater is somewhere between areas with traffic, on a road for example. The best use for a Bluetooth pylon is in someone's pocket or backpack, connected to their phone."
                       "<p>The best use for a WiFi pylon is somewhere that has a stable internet connection (satellite, etc.) or generally somewhere where people go (a waypoint, base camp, etc.)"
                       "<h2>About This Pylon</h2>"
#ifdef GPS_SERIAL_1
                       "<p>It has integrated GPS or timebase, and will send a time pulse every 1 hour to other nodes." // we should make the gps fix available...
#endif

#ifdef DISPLAY_DISASTER_RADIO_PACKETS // only do this if we are rebroadcasting them, nobody likes a stalker
                       "<p>It will rebroadcast and display Disaster Radio packets."
#else
#ifdef REBROADCAST_DISASTER_RADIO_PACKETS // what it says on the tin: do we also want to display it?
                       "<p>It will rebroadcast Disaster Radio packets."
#endif
#endif


#ifdef REQUIRE_TAG_FOR_REBROADCAST_STRICT
#else
#ifdef REQUIRE_TAG_FOR_REBROADCAST
                       "<p>It will honor other mesh network systems by repeating their packets too, as long as they start with a xxxx: tag in front."
#else
                       "<p>It will honor other mesh network systems by repeating their packets too!"
#endif
#endif
                       "<p>If you are on a Bluetooth or serial CellSol pylon, typing ,,, on a line by itself will dump the pylon's status and last received strings (in case your phone loses them). Our app does this automatically on reconnect. This lets you use any terminal app."
#ifdef MODEFLIP
                       "<p>This pylon will turn on its WiFi once a minute to check for clients, and be a repeater the rest of the time, in order to save power."
#else
                       "<p>Local connection and IRC gateway options can be configured by the pylon owner."
#endif
                       "<p>The CellSol pylons (any version of them) will work on any 3.6 or 3.7v batteries, or any USB power banks, or anything that delivers 3.5 to 6 volts, really."
                       "<h2> About the Project</h2>"
#ifdef PROVIDE_SOURCE_CODE
                       "<p>The project is open source and open schematic. You can get a copy of the source right from here! <a href=\"/src.zip\"> Source code </a>."
#else
                       "<p>The project is open source and open schematic. You can download everything from <a href=\"http://f3.to/cellsol/\">http://f3.to/cellsol/</a> or from a pylon that carries it (This one doesn't have enough memory to)."
#endif
                       "<p><a href=\"/\">Go back</a></body></html>");
  PetTheWatchdog();
  wificlient1.println();
}
#endif

void sendstrings()
{
  for (byte i = 10; i > 0; i--)
  {
    PetTheWatchdog();
    wificlient1.println("<small><small>RX(" + TimeAgoString(pseudoseconds - timestamp_rx[i - 1]) + "):</small></small>" + (string_rx[i - 1]) + "<br>");
    //    wificlient1.println("<small><small>RX(" + TimeAgoString(pseudoseconds - timestamp_rx[i - 1]) + "):</small></small>" + encodeHtml(string_rx[i - 1]) + "<br>");
  }
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
  if (HasLocalAP()) dnsServer.processNextRequest();

  // Do wifi things here
  wificlient1 = server.accept();   // NetworkServer 3.x API

  if (UsesUpstreamWifi() && !wificlient1 && WiFi.status() != WL_CONNECTED)
  {
    if ((uint32_t)(millis() - LastReconnect) > RECONNECT_EVERY)
    {
      LastReconnect = millis();
      ConnectToUpstreamWifi(100);
    }
  }
  if (wificlient1) {
    //  Serial.print("2");
    currentlyserving = true;
    //      Serial.println(String(client)); // says 1
    if (UsesUpstreamWifi()) LastReconnect = millis();
    unsigned long connect_timeout = millis() + 3000;
    char c = 0, c1 = 0, c2 = 0;
    while (wificlient1.connected()) {            // loop while the client's connected
      // does not get this far outside lan for some reason
      if (wificlient1.available() == false)
      {
        //  Serial.print("3");
        DoBasicSteps();
        if (HasLocalAP()) dnsServer.processNextRequest();
        if ((int32_t)(millis() - connect_timeout) >= 0)
        {
          wificlient1.stop();
          break;
        }

      }
      if (wificlient1.available()) // if there's bytes to read from the client,
      {
        //  Serial.print("4");
        PetTheWatchdog();
        c2 = c1;
        c1 = c;
        c = wificlient1.read();

        if (header.length() < MAX_HTTP_REQUEST_SIZE)
          header += c;

        //        Serial.print(c1);

        // detect two newlines
        if ((c == 10 and c1 == 10) or (c == 13 and c1 == 13) or (c == 10 and c1 == 13 and c2 == 10)) {              // if the byte is a newline character
          PetTheWatchdog();

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:

          //Serial.print(header);

          int hostStart = header.indexOf("Host: ");
          if (hostStart >= 0)
          {
            int hostEnd = header.indexOf('\n', hostStart);
            hoststring = header.substring(hostStart + 6,
                                          hostEnd >= 0 ? hostEnd : header.length());
            hoststring.trim();
            hoststring += "/";
          }
          else
            hoststring = ipstring_a + "/";

          const int firstSpace = header.indexOf(' ');
          const int secondSpace = header.indexOf(' ', firstSpace + 1);
          String whattoget = (firstSpace >= 0 && secondSpace > firstSpace)
            ? header.substring(firstSpace + 1, secondSpace) : "";

          //Serial.print(hoststring);

          lwc = wificlient1.remoteIP();
          if (lwc[2] == AP_IP[2] && lwc[1] == AP_IP[1] && lwc[0] == AP_IP[0])
          {
            IP = AP_IP;
            client_on_lan = true;
          }
          else
          {
            IP = Client_IP;
            client_on_lan = false;
          }
          //              Serial.println(":SYS:IP " + ipstring_a + " " + ipstring_c + " " + ipstring + " " + lwc.toString());

          ipstring = IP.toString();

          LastReconnect = millis(); // we're talking, so obviously there's no need to reconnect!

          //Serial.print(hextag);
          //Serial.print(String(lwc));
          //Serial.println("Header:" + header);



          // naive, and cheap, method for serving multiple pages. if someone better at HTML than me can do autorefresh of the chat contents, that'd be awesome.
          if (whattoget.equals("/favicon.ico"))
          {
            wificlient1.println("HTTP/1.1 204 No Content\r\n");
            //                wificlient1.println();// we're not actually sending anything
          }
          else if (whattoget == String("/") + SETTINGS_PAGE_PATH && header.startsWith("GET "))
          {
            sendSettingsPage();
          }
          else if (whattoget == String("/") + SETTINGS_PAGE_PATH + "/save" && header.startsWith("POST "))
          {
            String body;
            if (!readSettingsPostBody(header, body)) {
              sendSettingsPage("Invalid or oversized settings request; nothing was saved");
            } else {
              String error = updateSettingsFromForm(body);
              if (error.length()) {
                sendSettingsPage(error + "; nothing was saved");
              } else {
                send_ok_response();
                send_html_header();
                wificlient1.println("<body><h2>Saved</h2><p>Restarting CellSol. "
                                    "Reconnect to the configured WiFi address. "
                                    "If upstream WiFi is unavailable, the local CellSol AP will return for recovery.</p></body></html>");
                wificlient1.flush();
                delay(1200);
                ESP.restart();
              }
            }
          }
#ifdef SERVE_FAQ_PAGE
          else if (whattoget.equals("/faq.html")) // can  be done for other emergency files here
          {
            send_faq_page();
          }
#endif
          else if (whattoget.startsWith("/last") and (whattoget.endsWith(".txt")))
          {
            int i = whattoget.charAt(5) - '0';
            if (i > -1 and i < 10)
              send_string_by_itself(string_rx[i]);
            else
              send_string_by_itself("");
          }
#ifdef COMMUNITY_MEMORY_SIZE
          else if (whattoget.equals("/mem.html")) // can  be done for other emergency files here
          {
            //Serial.println("Regular page (default)");
            send_ok_response();

            // Display the HTML web page
            // General purpose html header

            send_html_header();
            wificlient1.println( "<body>CellSol WiFi Pylon " VERSIONSTRING " at ");
            wificlient1.print(ipstring_a);
            wificlient1.println("<div style=\"max-width:100%;\"><p><small>");
            for (int i = COMMUNITY_MEMORY_SIZE - 1; i > -1; i--)
            {
              PetTheWatchdog();
              wificlient1.print("<br><small><small>RX");
              //              wificlient1.print(i + 9);
              wificlient1.print(":</small></small>");
              //              wificlient1.println(encodeHtml(communitymemory[i]));
              wificlient1.println(communitymemory[i]);
            }
            wificlient1.print("<br>");
            sendstrings();
            wificlient1.println("</small></p><a href=\"http://" + hoststring + "\">Return to main</a></div></body></html>");
            wificlient1.println();
          }
#endif
#ifdef PROVIDE_CAT_PICTURE
          else if (whattoget.equals("/cat.jpg")) // can  be done for other emergency files here
          {
            SendBinaryFile("image/jpeg", cat_jpg, cat_jpg_size);
          }
#endif
#ifdef PROVIDE_SOURCE_CODE
          else if (whattoget.equals("/src.zip")) // can  be done for other emergency files here
          {
            SendBinaryFile("application/octet-stream", src_zip, src_zip_size);
          }
#endif
#ifdef PROVIDE_APK
          else if (whattoget.equals("/btt.apk")) // can  be done for other emergency files here
          {
            SendBinaryFile("application/octet-stream", btt_apk, btt_apk_size);
          }
#endif
          else if (whattoget.equals("/chat.html")) // chat iframe
          {
            send_ok_response();
            send_html_header(REFRESH_CHAT_EVERY);
            if (UTC_Seconds)
              wificlient1.println("<body><small><small><small>UTC Time Update:" + UTC_String(UTC_Seconds) + "<br></small></small>");
            else
              wificlient1.print("<body><small>");
            sendstrings();
            wificlient1.println("</small></body></html>");
          }
#ifdef TX_IFRAME
          else if (whattoget.equals("/answerform.html")) // answer form iframe
          {
            send_form_iframe();
          }
#endif
          else if (whattoget.startsWith("/get"))
          {
            // Parse the URL's query, not the whole HTTP header. The old code
            // expected &refresh= even though the JavaScript never sent it.
            const int inputStart = whattoget.indexOf("input1=");
            gotstring = (inputStart >= 0) ? whattoget.substring(inputStart + 7) : "";
            const int ampersand = gotstring.indexOf('&');
            if (ampersand >= 0)
              gotstring = gotstring.substring(0, ampersand);
            char tempme[MAX_HTTP_REQUEST_SIZE + 1];
            gotstring.toCharArray(tempme, sizeof(tempme));
            decode_in_place(tempme);
            gotstring = String(tempme);
            gotstring.trim();
            if (gotstring.length() > MAXPKTSIZEM)
              gotstring = "(>maxsize)" + gotstring.substring(0, 20) + "...";
            if (gotstring.length() > 1) // don't send empty strings or strings with just
            {
              String hextag_temp;
              last_web_caller = wificlient1.remoteIP()[3];
              if (UsesUpstreamWifi() && !client_on_lan) {
                lwc = wificlient1.remoteIP();
                hextag_temp = fourhex(lwc[0] ^ lwc[3], lwc[1] ^ lwc[2]);
              } else {
                if (last_web_caller < 16)
                  last_web_caller += spare_id_nibble;
                hextag_temp = fourhex(derpme, last_web_caller);
              }
              if (hextag_temp.charAt(3) == ('0'))
                hextag_temp.setCharAt(3, '1'); // only actual nodes are allowed to end in 0
              lasttagimade = hextag_temp;

#ifdef GPS_SERIAL_1
              if (gotstring.startsWith("UTC:"))
                gotstring = fourhex(derpme, spare_id_nibble + 0) + TAG_END_SYMBOL + "UTC:" + String(UTC_Seconds);
#endif
              gotstring = hextag_temp + TAG_END_SYMBOL + gotstring;

              if (string_rx[0].equals(gotstring) == false) // prevents sending it out twice if it's the last thing that went out
              {
                Serial.println(gotstring);
                if (has_bluetooth_been_initialized) ESP_BT_println(gotstring);


                //Send LoRa packet to receiver. Will need to make sure we don't send duplicates.

                LoraSendAndUpdate(gotstring);
                cyclestrings(gotstring);
#ifdef IRC_SERVER
                irc_broadcast( gotstring);
#endif
              }

              gotstring = "";

            }
#ifdef TX_IFRAME
            send_form_iframe();
#else
            send_redirect_to_main(true);
#endif
          }
          else if (whattoget.equals("/refresh")) // exception is needed to make sure that the refresh button works properly
          {
            send_redirect_to_main(true);
          }
#ifdef DEBUG_OPTIONS_PAGE
          else if (whattoget.startsWith("/option!")) // semi hidden option stuff! Yay! Any more that we need? I don't want to make it possible to turn this off or switch modes remotely because that's easy to abuse.
          {
            char option = whattoget.charAt(8);
            if (option == 'C') // center text
            {
              centertext = true;
              TEXT_ALIGN_STRING = TEXT_ALIGN_STRING_A;
            }
            if (option == 'J') // justify text
            {
              centertext = false;
              TEXT_ALIGN_STRING = TEXT_ALIGN_STRING_B;
            }
            if (option == 'S') // turn serial port off (in case of spam or errors coming from it)(turn back on with the enable string)
            {
              enablecomport = false;
            }

            if (option == 'T') // reset pseudoseconds timer
            {
              pseudoseconds = 0;
            }
#ifdef IRC_SERVER
            if (option == 'I') // reset failed irc connections / try to connect to irc again
            {
              failed_irc_connections = 0;
              pongs_joins = 0;
              has_irc_been_initialized = false;
            }
#endif
            if (option == 'D') // force display on
            {
              display_on_ping = true;
            }
            if (option == 'd') // force display off
            {
              display_on_ping = false;
            }
            if (option == 'B') // force sending (broadcasting) twice on
            {
              broadcast_twice = true;
            }
            if (option == 'b') // force sending (broadcasting) twice off
            {
              broadcast_twice = false;
            }
            send_ok_response();
            send_html_header(0);
            wificlient1.print("<body>CellSol WiFi Pylon " VERSIONSTRING " Option: <b>");
            wificlient1.print(option);
            wificlient1.println("</b><br>Redirecting to main</body></html>");
            wificlient1.println();
          }
#endif
          else if (whattoget.equals("/") or whattoget.startsWith("/index.html"))
          {
            //Serial.println("Regular page (default)");
            send_ok_response();
            // Display the HTML web page
            // General purpose html header

            send_html_header();
            wificlient1.println("<script>"
                                "window.addEventListener(\"load\", function(){"
                                "var input = document.getElementById('msgfield');"
                                "input.addEventListener(\"keyup\", function(event) {"
                                "if (event.keyCode === 13) {"
                                "event.preventDefault();"
                                "document.getElementById(\"msgBtn\").click();}});});"
                                "function resizeIframe(obj) {"
                                "    obj.style.height = obj.contentWindow.document.documentElement.scrollHeight + 'px';"
                                //"    obj.style.width = obj.contentWindow.document.documentElement.scrollWidth + 'px';"
                                "  }\r\n</script>"

                                "<script>function sendMsg(){\r\n"
                                "getSend('/get',document.getElementById('msgfield').value); document.getElementById('msgfield').value=\"\";\r\n"
                                "}\r\n"
                                "function getSend(url, input1=null) {\r\n" // we default undefined so that we don't have to explicitly specify it; it just gets passed to the next function
                                "var xhttp = new XMLHttpRequest();\r\n"
                                "xhttp.open('GET', url+\"?input1=\" + encodeURIComponent(input1 || ''), true); xhttp.send();"
                                "}"
                                "</script>"
                                "<body>CellSol WiFi Pylon " VERSIONSTRING " at ");
            wificlient1.print(ipstring_a);
            wificlient1.println("<br>"
#ifdef COMMUNITY_MEMORY_SIZE
                                "<a href=\"http://" + hoststring + "mem.html\">Older messages</a> "
#endif
#ifdef PROVIDE_CAT_PICTURE
                                "<a href=\"http://" + hoststring + "cat.jpg\">Cat picture</a> "
#endif
#ifdef SERVE_FAQ_PAGE
                                "<a href=\"http://" + hoststring + "faq.html\">FAQ page</a> "
#endif
                                "<br>"
                                "<div style=\"max-width:100%;\">"
                                "<iframe id=\"chatin\" src=\"/chat.html\" frameborder=\"0\" scrolling=\"no\" onload=\"resizeIframe(this);\"style=\"width:100%;\" /></iframe>"
#ifdef TX_IFRAME
                                "<iframe id=\"chatout\" src=\"/answerform.html\" frameborder=\"0\" scrolling=\"no\" style=\"width:100%; height:3em;\" /></iframe></div>"
#else
                                "<small><small>TX&gt;</small></small> <input id=\"msgfield\" type=\"text\" maxlength=\"160\" name=\"input1\"><button id=\"msgBtn\" onClick=sendMsg() value=\"Send\">Send</button><br>"
#endif
                               );
            /*
              "<iframe id=\"chatin\" src=\"/chat.html\" frameborder=\"0\" scrolling=\"no\" onload=\"resizeIframe(this);\"style=\"width:100%;\" />");
              // in case iframe doesn't work
              sendstrings();
              wificlient1.println("</iframe>"
              "<br>"
              "<iframe id=\"chatout\" src=\"/answerform.html\" frameborder=\"0\" scrolling=\"no\" style=\"width:100%; height:5em;\" />"
              // in case iframe doesn't work, it displays this
              "<form action=\"/get\"> Your message: <input type=\"text\" maxlength=\"50\" name=\"input1\"><input type=\"submit\" value=\"Submit\"><form>"
              "</iframe>");
            */
            wificlient1.println("<button value=\"Refresh this page (in case of errors, etc.)\" onClick=getSend('/refresh')>Refresh this page (in case of errors, etc.)</button>");
            wificlient1.print  ("<br><small>CellSol is a serverless relay chat between LoRa pylons. It is intended for enabling communication in case of cell phone network disruption.<br>"
                                "Bluetooth terminal APK download (you may have to enter URL in browser manually): "
#ifdef PROVIDE_APK
                                "<a href=\"http://" + hoststring + "btt.apk\" target=\"_blank\">http://" + hoststring + "btt.apk</a> , <a href=\"http://f3.to/btt.apk\" target=\"_blank\">http://f3.to/btt.apk</a><br>To help this project grow, you must construct additional pylons. <br>Sysinfo:(" PYLONMODEL ";");
#else
                                "<a href=\"http://f3.to/btt.apk\" target=\"_blank\">http://f3.to/btt.apk</a><br>To help this project grow, you must construct additional pylons. <br>Sysinfo:(" PYLONMODEL ";");
#endif
            wificlient1.print(PylonTypeLabel());
            wificlient1.print(" ");
            wificlient1.print(status_string());
#ifdef PROVIDE_SOURCE_CODE
            wificlient1.print("The source code for all platforms is available <a href=\"http://" + hoststring + "src.zip\" target=\"_blank\">here.</a>");
#endif

            if (pseudoseconds % 2)
              wificlient1.println("<small> Deus Nolens Exitus</body></html>");
            else
              wificlient1.println("<small> Vigilo Confido</body></html>");


            // The HTTP response ends with another blank line
            wificlient1.println();
          }
          else // we were asked a page that we don't have. redirect to main
          {
            if (LoginPageRequested(whattoget))
            {
              send_redirect_to_main(true);
            }
            else
            {
              send_ok_response();
              send_html_header(0);
              wificlient1.println(String("<body>CellSol WiFi Pylon " VERSIONSTRING " (" PYLONMODEL ";") + PylonTypeLabel() + " does not hold " + htmlAttribute(whattoget) + "<p>Redirecting to main</p></body></html>");
              wificlient1.println();
            }
          }
          header = "";
          break;
        }
      }
    }
    // Clear the header variable
    header = "";
    wificlient1.stop();
    DoBasicSteps();
    currentlyserving = false;
  }
}


#ifdef IRC_SERVER
String IRC_NICK;
String IRC_CHAN;
String hextag_irc;
int prefixlength = -1;
byte pongs_joins = 0;

WiFiClient wificlient2;
bool has_irc_been_initialized = false;
IRCClient *ircclient = nullptr;

void DoIRCStuff()
{
  if (!settings.ircEnabled || !UsesUpstreamWifi() || WiFi.status() != WL_CONNECTED)
    return;
  if (failed_irc_connections > IRC_FAILED_TRE)
    return;

  if (has_irc_been_initialized == false)
  {
    hextag_irc = fourhex(derpme, spare_id_nibble + 0);
    IRC_NICK = settings.ircNick + hextag_irc;
    IRC_CHAN = String("#") + settings.ircChannel;
    if (settings.ircChannelTag) IRC_CHAN += hextag_irc;
    prefixlength = settings.ircPrefix.length();
    if (ircclient == nullptr)
      ircclient = new IRCClient(settings.ircServer.c_str(), settings.ircPort, wificlient2);
    if (ircclient == nullptr) return;
    ircclient->setCallback(irc_callback);
    ircclient->setSentCallback(debugSentCallback);
    has_irc_been_initialized = true;
  }
  if (!ircclient->connected())
  {
    Serial.println(":SYS:IRC_CONN");
    // Attempt to connect
    if (ircclient->connect(IRC_NICK, IRC_NICK))
    {
      Serial.println(":SYS:IRC_CONN_OK");
      failed_irc_connections = 0;
      pongs_joins = 0;
    }
    else
    {
      Serial.println(":SYS:IRC_CONN_FAIL");
      // Wait 1 second before retrying
      BasicWhileDelay(100); // needs smart delay later
      failed_irc_connections++;
    }
    return;
  }

  ircclient->loop();

}
void irc_broadcast (String s)
{
  if (settings.ircEnabled && has_irc_been_initialized && ircclient != nullptr)
    if (ircclient->connected())
      ircclient->sendMessage(IRC_CHAN, s);
}

void irc_callback(IRCMessage ircMessage)
{
  PetTheWatchdog();
  // PRIVMSG ignoring CTCP messages
  if (ircMessage.command == "PRIVMSG" && ircMessage.text.length() && ircMessage.text[0] != '\001')
  {
    bool validforward = ircMessage.text.startsWith(settings.ircPrefix);
    bool isotherpylon = false;
    if (validforward == false && settings.ircForwardPylons)
      isotherpylon = ircMessage.nick.startsWith(settings.ircNick) &&
                      ircMessage.nick.length() >= settings.ircNick.length() + 4 &&
                      ircMessage.nick.charAt(ircMessage.nick.length() - 1) == '0' &&
                      IsHex(ircMessage.nick.charAt(ircMessage.nick.length() - 2)) &&
                      IsHex(ircMessage.nick.charAt(ircMessage.nick.length() - 3)) &&
                      IsHex(ircMessage.nick.charAt(ircMessage.nick.length() - 4));
    validforward = validforward or isotherpylon;
    if (validforward)
    {
      String gotstring;
      if (isotherpylon)
      {
        //        Serial.println("ADDITIONAL PYLONS");
        gotstring = hextag_irc + TAG_END_SYMBOL + ircMessage.nick.substring(ircMessage.nick.length() - 4) + ">" + ircMessage.text.substring(0, MAXPKTSIZEM);
      }
      else
      {
        gotstring = hextag_irc + TAG_END_SYMBOL + ircMessage.nick.substring(0, 6) + ">" + ircMessage.text.substring(prefixlength, MAXPKTSIZEM);
      }
      gotstring = gotstring.substring(0, MAXPKTSIZEP);
      Serial.println(gotstring);

      if (has_bluetooth_been_initialized)
        ESP_BT_println(gotstring);

      LoraSendAndUpdate(gotstring);
      cyclestrings(gotstring);
    }
    return;
  }
  //Serial.print("irc_callback");
  if (ircMessage.original.endsWith("already in use."))
  {
    PetTheWatchdog();
    IRC_NICK = IRC_NICK + "_";
    if (IRC_NICK.endsWith("___"))
      IRC_NICK.replace("___", "");
    if (ircclient) ircclient->sendRaw("NICK " + IRC_NICK);
    //ircclient.sendRaw("USER " + IRC_USER + " 8 * :Arduino IRC Client");
    //Serial.println(IRC_NICK);
  }
  // Serial.println(ircMessage.original);
}

void debugSentCallback(String data)
{
  PetTheWatchdog();
  if (data.startsWith("SENT: PONG") and (++pongs_joins < 3)) // if we are getting pings, we are in good shape as far as the irc server is concerned, so join the channel and start doing work.
  {
    if (ircclient) {
      ircclient->sendRaw("JOIN " + IRC_CHAN); // if already joined, rejoin, not a problem
      ircclient->sendRaw("TOPIC " + IRC_CHAN + " :CellSol relay channel. Prefix radio messages with " + settings.ircPrefix);
    }

  }
  //Serial.print("debugSentCallback");
  //Serial.println(data);
}

#endif


void loop() {

//display_on_ping = true; // temporary, for testing

#ifdef MINDISPLAY
  DoMinDisplay();
#endif

  if (wifimode)
  {
    DoWifiSteps();
    ServeWebPagesAsNecessary();
#ifdef IRC_SERVER
    DoIRCStuff();
#endif
  }
  else
  {
    DoBtSteps();
  }
}
