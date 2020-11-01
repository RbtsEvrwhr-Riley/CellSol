/*********
  Cellular-Solar (CellSol) is a simple interconnect between lora, wifi and com port(s) by spiritplumber. Intended to be used to allow for comms after a natural disaster.
  (c) 2020 Robots Everywhere, LLC until we are ready to release it under copyleft
  Written by Riley August (HTML, CSS), and spiritplumber (skeleton). Thanks to Rui Santos for the tutorials. Thanks to Jerry Jenkins for the inspiration
*********/

// Firmware version.
#define VERSIONSTRING "0.23"

// hardware platform types; pick one and only one. Default is LORA32 V2. If yours is not in here, you'll have to adjust stuff manually. They will override some settings (for example, selecting TTGO will turn off the display since there isn't one)
#define LORA32V2 // Heltec LORA32 V2 (White board)
//#define TBEAM // TTGO T-Beam (Black board with gps)
//#define LORA32V1 // Heltec clone (Black board)

// Physical radio band. Choosing the wrong range WILL DAMAGE YOUR RADIO MODULE. Currently set up to stay a little away from the main LoRa and LoRaWAN radio ranges out of politeness.
//4331E5 for worldwide (restrictions apply)

//8661E5 for Europe
//9151E5 for North America
#define BAND 9151E5

// compile time setup stuff. if you aren't sure what these do, leave them alone.
// the default is that these four are commented out. This gives you the standard wifi access point setup.
// The first mode saves power but makes the AP invisible 45 seconds out of every minute.
// The last three modes use more power than standard mode!
// switching between wifi and bluetooth is done at startup with the normal mode, not here; push the button immediately after a reset.
// if more than one is uncommented, lowest line wins.
//#define MODEFLIP 15000 // minimum suggested: 12000. powershare mode operates the pylon on full AP for this many milliseconds every minute, and as a repeater the rest of the time. It is exclusive with the other options. The mode flip will pause if there are clients connected.
//#define BT_ENABLE_FOR_AP // Uncomment this to also enable Bluetooth when the pylon is running as an access point. THIS WILL EAT UP A LOT OF POWER! Only enable if you're using a big battery and a big panel, or have line power.

//#define WIFI_IS_CLIENT // Uncomment this to enable use as a gateway. Bluetooth will be on. Note that gateway mode must be configured manually and will need an open port on the router. If you don't know what this does, leave it alone!
//#define WIFI_IS_HYBRID // Uncomment this to enable use as BOTH a gateway and an AP. Bluetooth will be on. Performance will be slower than either. Note that this won't allow people to get on the internet through the AP, so it's ideal if you want to let people use cellsol without a password, but not use the internet.
#define DHCP // DHCP on if defined; will ignore IP address setting fields if DHCP is on. ON BY DEFAULT.

//#define REPEATER_ONLY // Uncomment this to bypass everything else and runs as repeater (and serial) only. useful if we are out of arduinos. not useful otherwise.

//#define NODISPLAY // display is absent or should be kept turned off at all times. Saves some power, naturally.

#define SEND_TWICE // if defined, allow sending a packet twice after a pseudorandom delay, in case the first one got lost
#define RSSI_TRE_LO -130 // if sendtwice is defined, below this (for the last 4 received packets), turn on sendtwice
#define RSSI_TRE_HI -100 // if sendtwice is defined, above this (for the last 4 received packets), turn off sendtwice

#define TUTORIALSTRINGS // On first activation after a poweroff/reset, should we display a mini help on the screen?

#define PROVIDE_APK // make bluetooth terminal apk available?
#define THE_INTERNET_IS_MADE_OF_CATS // load cat picture as an example of embedded file?
//#define YOU_ARE_EATING_RECURSION // embed source zip in source for the glory of recursion?
#define HELPLINK  // if undefined, do not add help page

// these only have an effect in client and hybrid mode; the IP address for AP mode is always 192.168.(autocalculated).1
// the upstream router will have to either open port 80 to this, or do a redirect.
// These should be ignored in dhcp mode
#define CLIENT_IP_ADDR 192,168,2,55 // client IP address for client or hybrid mode. needs commas instead of periods
#define GATEWAY_IP_ADDR 192,168,2,1 // router IP address for client or hybrid mode. needs commas instead of periods
#define GATEWAY_SUBNET 255,255,255,0 // subnet mask for client or hybrid mode. needs commas instead of periods
#define WIFI_UPSTREAM_AP "RobotsEverywhere_24" // SSID of the router we're trying to connect to. 
#define WIFI_UPSTREAM_PWD "derpderp"// password for the router we're trying to connect to. Use "" for none/open.

#define USE_BATTERY_NOISE_FOR_ID // if undefined, same id across power cycles. if not, use battery level to get a bit of noise in the ID (mostly to avoid creepy people hashing it to figure out where you are).
#define DO_NOT_LOG_SYSTEM_PACKETS // Don't display system packets to avoid spamming out human messages (example: UTC fix)



// this affects wifi range. obviously it will affect power consumption. it does not affect lora range. for line powered stuff, it'll be turned up to 11. For default, keep it commented out.
#define WIFI_POWER_LEVEL 4 // 0 to 11, higher = stronger. lora power level is fixed at 19/20 because 20/20 can mess up some radios.

// more timing stuff
#define SLEEP_TIME 60 // this is in seconds - how long to sleep before waking up and checking power level again? NOT ACCURATE.
#define DISPLAY_INTERVAL 60 // In milliseconds, how long to keep the display on after button release, if it's there? NOT ACCURATE.

#define TX_IFRAME // use an iframe for the tx form on the page. someone with web knowledge tell me which is nicer please. i think that not using it is actually slightly faster overall. nested iframes (main(tx(rx))) maybe?

// consequences of the setup above
#define PYLONTYPE "(AP)"// identifier for how we are running
#ifdef MODEFLIP
#define PYLONTYPE "(~AP~)"// identifier for how we are running
#endif
#ifdef BT_ENABLE_FOR_AP // if both are uncommented, it'll go to HYBRID Mode.
#undef MODEFLIP
#define WIFI_POWER_LEVEL 11 // 0 to 11, higher = stronger. lora power level is fixed at 19/20 because 20/20 can mess up some radios.
#define PYLONTYPE "(AP+BT)" // identifier for how we are running
#endif
#ifdef WIFI_IS_HYBRID // if both are uncommented, it'll go to HYBRID Mode.
#define WIFI_IS_CLIENT // if both are uncommented, it'll go to HYBRID Mode.
#endif
#ifdef WIFI_IS_CLIENT // if defined, wifi will attach to an existing AP and act as a web gateway. it may be useful to have a way to get OUT of this mode, but that's for another day
#undef MODEFLIP
#define DISPLAY_INTERVAL 2000000000// basically leave the screen on, we're on line power anyway. button is now used to force wifi reconnect.
#define RECONNECT_EVERY 60000 // every this many milliseconds, reconnect to the upstream wifi (useful in case your router is prone to crapping out, or does load balancing)
#define WIFI_RESET_CYCLES 1000 // after this many cycles, reset upon wifi disconnect/reconnect
#define SLEEP_TIME 20  // gateway is line powered, so it stays on
#define BT_ENABLE_FOR_AP // one implies the other
#define PYLONTYPE "(STA+BT)" // identifier for how we are running
#endif
#ifdef WIFI_IS_HYBRID // if both are uncommented, it'll go to HYBRID Mode.
#define RECONNECT_EVERY 2000000000 // every this many milliseconds, reconnect to the upstream wifi (useful in case your router is prone to crapping out, or does load balancing)
#define PYLONTYPE "(AP+STA+BT)"// identifier for how we are running
#endif


// battery stuff
#define MODEFLIP_BATTERY_FULL 1304 // if modeflip, battery level above this will leave the module running on full. Useful if we are getting good sunlight and the battery is full anyway.
#define MODEFLIP_BUTTON // user button gets you back into high power mode

#define LPLOOP_BLINK 10000 // eversoly this many cycles (not milliseconds!), blink the led
#define ADC_INTERVAL 5000 // In milliseconds, how often to read the battery level? NOT ACCURATE.
#define FULL_BATT 1450 // unused but a good reference. more than this = there probably is no battery
#define BATT_HIGH_ENOUGH_FOR_FULL_POWER 1200 // above this, allow wifi/bt
#define BATT_TOO_LOW_FOR_ANYTHING 1065 // below this, don't try to turn on, go back to sleep and wait for better times
#define BATT_HYSTERESIS_POWER 15 // hysteresis between power state changes
#define POWER_STATE_CHANGE_ANNOUNCE false //  should the module announce it when it's coming online or going offline? (probably only for debugging)
#define REFRESH_CHAT_EVERY 3 // seconds

#ifdef REPEATER_ONLY
#define LPLOOP_BLINK 5000 // every this many cycles (not milliseconds!), blink the led
#define ADC_INTERVAL 10000 // In milliseconds, how often to read the battery level? NOT ACCURATE.
#define FULL_BATT 1450 // unused but a good reference. more than this = there probably is no battery
#define BATT_HIGH_ENOUGH_FOR_FULL_POWER 1500 // above this, allow wifi/bt
#define BATT_TOO_LOW_FOR_ANYTHING 1065 // below this, don't try to turn on, go back to sleep and wait for better times
#define BATT_HYSTERESIS_POWER 15 // hysteresis between power state changes
#define POWER_STATE_CHANGE_ANNOUNCE false //  should the module announce it when it's coming online or going offline? (probably only for debugging)
#define REFRESH_CHAT_EVERY 42 // doesn't really matter since it neveer gets called
#define PYLONTYPE "L0WPWR"// identifier for how we are running
#undef TUTORIALSTRINGS
#endif

#define TAG_END_SYMBOL ':'

#ifdef PROVIDE_APK
#include "btt/btt.h"
#endif

#ifdef THE_INTERNET_IS_MADE_OF_CATS
#include "btt/cat.h"
#endif

#ifdef YOU_ARE_EATING_RECURSION // TODO: add to design document
#include "btt/src.h"
#endif


#define REQUIRE_TAG_FOR_REBROADCAST // if defined, require xxxx: tag for rebroadcast.
//#define REQUIRE_TAG_FOR_REBROADCAST_STRICT // on top of that, the first four characters must be hex digits.
#define BAD_CHARACTERS_MAX_DIVIDER 10 // 1/x of bad characters before we discard the string
//#define SHOW_RSSI // if enabled, show RSSI for wireless packets coming in.

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

//Libraries for LoRa - TODO: where do we get them?
#include <SPI.h>
#include <LoRa.h>

// watchdog - TODO: where do we get them?
#include <esp_task_wdt.h>
#define WDT_TIMEOUT 4 // in seconds

//define the pins used by the LoRa transceiver module. note that these change between ttgo and lora32!
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

#include "esp32-hal-cpu.h"
#define CLKFREQ_HI 240 // megahertz when going as fast as possible
#define CLKFREQ 80 // megahertz when in full mode
#define CLKFREQ_LOW 20 // megahertz when in repeater mode; can't go any lower, annoyingly

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


// Assign output variables to GPIO pins
#define USER_BUTTON_PIN 0
#define USER_LED_PIN 25
#define enablecomstring "COMCHIAVE"

#define EXT_PWR_PIN 21
#define BATT_ADC 37

#ifdef LORA32V2
#undef GPS_SERIAL_1 // no gps on the 
#define EXT_PWR_PIN 21
#define BATT_ADC 37
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26
#define USER_BUTTON_PIN 0
#define USER_LED_PIN 25
#endif

#ifdef TBEAM
#define GPS_SERIAL_1 3600 // every this many seconds, send out UTC seconds. Internal is updated as often as they come in
#define NODISPLAY // force no display since the system doesn't have it
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DIO0 26
#undef YOU_ARE_EATING_RECURSION
#undef THE_INTERNET_IS_MADE_OF_CATS
#undef PROVIDE_APK
#undef TUTORIALSTRINGS
#undef HELPLINK
#define USER_BUTTON_PIN 38
#undef USER_LED_PIN
#undef EXT_PWR_PIN
#undef BATT_ADC
#endif


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

#define TEXT_ALIGN_STRING_A "center"
#define TEXT_ALIGN_STRING_B "justify"
String TEXT_ALIGN_STRING = TEXT_ALIGN_STRING_A;
static RTC_NOINIT_ATTR bool centertext = true;// if this, center, otherwise, justify

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

#ifdef PROVIDE_APK
extern const long int btt_apk_size;
extern const unsigned char btt_apk[];
#endif
#ifdef THE_INTERNET_IS_MADE_OF_CATS
extern const long int cat_jpg_size;
extern const unsigned char cat_jpg[];
#endif
#ifdef YOU_ARE_EATING_RECURSION
extern const long int src_zip_size;
extern const unsigned char src_zip[];
#endif






/**
   BATTERY ADC BLOCK: DEFINE THESE TWO PINS TO ENABLE BATTERY LEVEL SENSOR
   SOME VERSIONS OF THE LORA32 WILL REQUIRE EXT_PWR_PIN 12 HIGH AND BATT_ADC 13
   DO NOT SET EXT_PWR_PIN LOW ON THESE YOU MAY FRY YOUR ADC CHECK HELTEC DOCUMENTATION
*/

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


unsigned long pseudoseconds = 0;
unsigned long psm = 0;
long UTC_Seconds = 0;
void PetTheWatchdog() {
  if (millis() > psm)
  {
    esp_task_wdt_reset();
    // apparently the esp32 is smart enough to update tick size
    /*
      if (getCpuFrequencyMhz() < CLKFREQ)
        psm = psm+1000;
      else
        psm=psm+1000;
    */
    psm = psm + 999;
    pseudoseconds++;

    DoUTC(true);
  }
#ifdef GPS_SERIAL_1
  else
    DoUTC(false);
#endif
}


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
IPAddress Client_IP(CLIENT_IP_ADDR);// this is the IP address we will be using IF DHCP IS NOT CONFIGURED.
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
void Watchdog(bool onoff) // gets turned back on by the adc reading, assumtion is that we can survive for 5 seconds we hope
{
  if (onoff)
  {
    esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL); //add current thread to WDT watch
  }
  else
  {
    esp_task_wdt_delete(NULL);
    esp_task_wdt_deinit();
    esp_task_wdt_delete(NULL);
    esp_task_wdt_deinit();
  }
  is_watchdog_on = onoff;
  battimeout = millis() + ADC_INTERVAL; // wait to turn the watchdog back on please
}

String status_string()
{
  return (fourhex(derpme, spare_id_nibble) + TAG_END_SYMBOL + "(" + String(currbatterylevel) + "/" + String(batt_delta) + ") " + (is_watchdog_on ? "`" : ",") + String(pseudoseconds) + ",");
}

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

#define COMMUNITY_MEMORY_SIZE 50
#ifdef COMMUNITY_MEMORY_SIZE
String communitymemory[COMMUNITY_MEMORY_SIZE];
#endif
String string_rx[10];
long timestamp_rx[10];
String TimeAgoString(long secs) // keep it simple for display
{
  if (secs < 0)
    return "";
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

bool wifimode = true; // bluetooth or wifi


#ifdef USER_BUTTON_PIN
#else
#undef MODEFLIP_BUTTON
#endif

// if this exists, only show the display when we are pushing the button that way we know someone is actually looking at it (saves power)
#define user_button_display

#define MAXPKTSIZE  200 // lora packet is 255 bytes and we will need some for the header
#define MAXPKTSIZEP 201 // lora packet is 255 bytes and we will need some for the header
#define MAXPKTSIZEM 199 // lora packet is 255 bytes and we will need some for the header
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
      display.setCursor(0, 9);
      display.println(string_rx[2].substring(0, 42));
      display.setCursor(0, 27);
      display.println(string_rx[1].substring(0, 42));
      display.setCursor(0, 45);
      display.println(string_rx[0].substring(0, 42));
      //display.print(LoRaData);
      display.display();
    }
    dodisplaybuf = false;
    dodisplay = false;
  }
#endif
}

// moves old strings out
void cyclestrings(String newone)
{
  newone.replace('\r', ' ');
  newone.replace('\n', ' ');
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

            if (string_rx[i - 1].length() > 0)
              st.println(string_rx[i - 1]);
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

#ifndef GPS_SERIAL_1 // if we already have our own gps, use it.
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
        cyclestrings(utcstring);
        next_utc_to_send = pseudoseconds + (GPS_SERIAL_1 - 1); // one hour should do it
      }
    }
  }
}
#else
#ifdef WIFI_IS_CLIENT
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
    serstr.replace('\n', ' ');
    serstr.replace('\r', ' ');
    serstr.replace("  ", " ");
    serstr.trim();
    charcounter = 0;
#ifdef GPS_SERIAL_1
    if (serstr == "UTC")
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
    serstr.replace('\n', ' ');
    serstr.replace('\r', ' ');
    serstr.replace("  ", " ");
    serstr.trim();
    charcounte2 = 0;
#ifdef GPS_SERIAL_1
    if (serstr == "UTC")
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

// the binary file must be imported as a header using bin2c because I don't want a filesystem on this build, it's too much overhead rn
void SendBinaryFile(String contenttype, const unsigned char file[], const long int filesize)
{
  // this obviously should be its own function since we are using it more than once.
  client.print("HTTP/1.1 200 OK\r\n"
               "Content-Description: File Transfer\r\n"
               "Content-type:");
  client.print(contenttype);
  client.print("\r\nConnection: close\r\n"
               "Content-Transfer-Encoding: binary\r\n"
               "Content-Length: ");
  client.println(filesize);
  client.println();
  char ch;
  byte stuff[32]; // 32 bytes is a surprisingly good compromise between speed and memory overhead
  byte j = 0;
  int i;
  for (i = 0; i < filesize; i++)
  {
    stuff[j] = (byte)file[i];
    if (++j == 32)
    {
      client.write(stuff, 32);
      j = 0;
    }
    if (i == (filesize - 1))
    {
      client.write(stuff, j);
    }

    // attempt slow operation while the data gets sent; other wifi clients will just have to wait and that's all there is to it
    if (i % 64 == 0)
    {
      PetTheWatchdog();

      ReadFromStream(Serial, receivedChars, charcounter, readytosend, has_serial_been_initialized, 15);
#ifdef BT_ENABLE_FOR_AP
      ReadFromStream(ESP_BT, receivedChar2, charcounte2, readytosen2, has_bluetooth_been_initialized, 14);
#endif
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
  client.flush();
  client.println();


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
#ifndef DHCP // WIFI_IS_HYBRID
  WiFi.config(Client_IP, Client_Gateway, Client_Subnet, Client_Gateway, Client_Gateway);
#endif // WIFI_IS_HYBRID
  BasicWhileDelay(pausetime);
  WiFi.softAPConfig(AP_IP, AP_IP, IPAddress(255, 255, 255, 0));
  BasicWhileDelay(pausetime);
  if (String(WIFI_UPSTREAM_PWD).length() == 0)
    WiFi.begin(WIFI_UPSTREAM_AP);
  else
    WiFi.begin(WIFI_UPSTREAM_AP, WIFI_UPSTREAM_PWD);
  BasicWhileDelay(pausetime);
  WiFi.softAP(string2char(ssid), NULL, (1 + (derpme % 12)), false, 8); // the derpme thing is to set a channel
#else // !WIFI_IS_HYBRID
  WiFi.mode(WIFI_MODE_STA);
  BasicWhileDelay(pausetime);
#endif // END WIFI_IS_HYBRID

#ifndef DHCP // WIFI_IS_CLIENT
  WiFi.config(Client_IP, Client_Gateway, Client_Subnet, Client_Gateway, Client_Gateway);
#endif
  BasicWhileDelay(pausetime);
  WiFi.begin(WIFI_UPSTREAM_AP, WIFI_UPSTREAM_PWD);
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
  LastReconnect = millis();
  ipstring = ipstring_a;//AP_IP.toString();//"Gateway";//Client_IP.toString()+"G";
  dodisplay = true;
  BasicWhileDelay(pausetime);
  server.begin();
  CurrentlyConnecting = false;  
  Client_Gateway = WiFi.gatewayIP();
  Client_Subnet = WiFi.subnetMask();  
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
  Client_IP = WiFi.localIP();
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
      string_rx[1] = "Client IP address:   " + Client_IP.toString();
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

void send_html_header(int redir = -1) // 0: redirect to root. positive: refresh every x seconds
{
  PetTheWatchdog();
  client.print("<!DOCTYPE html><html><head>"
               "<title>CellSol WiFi Pylon " VERSIONSTRING "</title>");
  if (redir == 0)
    client.print("<meta http-equiv=\"refresh\" content=\"0;URL='/'\" />");
  else if (redir > -1 && redir < 99)
    client.print("<meta http-equiv=\"refresh\" content=\"" + String(redir) + "\">");
  /*
    client.println("<meta purpose=\"Deus Nolens Exitus\" name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"icon\" href=\"data:,\">"
                   "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: " + TEXT_ALIGN_STRING + ";}.button { background-color: #4CAF50; border: none; color: white; padding: 0px 0px; text-decoration: none; font-size: 20px; margin: 0px; cursor: pointer;}.button2 {background-color: #555555;}</style>"
                   "</head>");
  */
  client.print("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"icon\" href=\"data:,\">"
               "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: ");
  client.print(TEXT_ALIGN_STRING);
  client.println(";}.button {border: none; color: white; padding: 0px 0px; text-decoration: none; font-size: 1.2em; margin: 0px; cursor: pointer;}.button2 {}</style></head>");
}
bool client_on_lan = true;
String hoststring = "/";

void send_string_by_itself (String st)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/plain");
  client.println("Connection: close");
  client.println();
  client.println(encodeHtml(st));
  client.println();
}
void send_redirect_to_main(bool doip)
{
  PetTheWatchdog();
  client.println("HTTP/1.1 302 Found");
  if (doip)
  {
    if (client_on_lan)
      client.println("Location: http://" + ipstring); //ipstring
    else
      client.println("Location: http://" + hoststring); //ipstring
  }
  else
    client.println("Location: /");// http://" + ipstring + "/");
  client.println("Connection: close");
  client.println();
}

#ifdef TX_IFRAME
void send_form_iframe()
{
  send_ok_response();
  send_html_header();
  client.println("<body>"
                 "<form action=\"/get\"> <small><small>TX&gt;</small></small> <input type=\"text\" maxlength=\"160\" name=\"input1\"><input type=\"submit\" value=\"Submit\"><form>"
                 "</body></html>");
}
#endif
void send_ok_response()
{
  PetTheWatchdog();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
}

#ifdef HELPLINK
void send_help_page() // TODO: change this to a properly compressed HTML include and actually finish it.
{
  PetTheWatchdog();
  send_ok_response();
  send_html_header();
  client.println( "<body>CellSol WiFi Pylon " VERSIONSTRING " Help<p>The full help/howto is available, when the internet works, at <a href=\"http://f3.to/cellsol/\">http://f3.to/cellsol/</a></p><p>"
                  "<br>By using the chat in the main page, you will be able to communicate with people who are in range of the CellSol network, or people who are using CellSol gateways."
                  "<br>Each message is sent out to neighboring pylons, which can be a few kilometers apart, and rebroadcast. There is no routing; every pylon gets everything. You can consider this system akin to a single IRC/Discord/Twitch chat channel, except it will work when nothing else will."
                  "<br>The tag in front of your message is a pseudonymous identifier: it is used to tell people apart. It is four hex digits. (Sorry, no nicknames)."
                  "<br>It is free to use and does not depend on any infrastructure, each pylon is self-contained: just deploy a few of them in an area and you are good to go."
                  "<br>Any phone with a Bluetooth chat app (Android APK can be downloaded from this very pylon, iPhone or others will have to use their own) can also use the Bluetooth pylons or pocket nodes. They require very little power to operate."
                  "<br>This is a lot like existing LoRa mesh chat systems, except that it's intended to leave repeaters in place."
#ifdef REQUIRE_TAG_FOR_REBROADCAST
                  "<br>It will honor other mesh network systems by repeating their packets too, as long as they start with a xxxx: tag in front."
#else
                  "<br>It will honor other mesh network systems by repeating their packets too!"
#endif
                  "<br>The mesh topology prioritizes redundancy over speed or cleanliness, so you may occasionally get a garbled message: we try to display those in case their meaning can be understood despite the garbling."
                  "<br>The best use for a standalone repeater is somewhere with no traffic. The best use for a Bluetooth pylon is in someone's pocket or backpack, connected to their phone. The best use for a WiFi pylon such as this one is somewhere that has a hardened internet connection (satellite, etc.) or generally somewhere where people go (a waypoint, base camp, etc.)"
                  "<br>If you are on Bluetooth or serial, typing ,,, on a line by itself will dump the pylon's status and last received strings (in case your phone loses them). Our app does this automatically on reconnect. This lets you use any Bluetooth (or serial) terminal, nice if your phone is from 2006."
                  "<br>The default configuration lets you switch this device from wifi to bluetooth by pushing the PROG button right after a reset. Other configurations have bluetooth on by default so it's not an issue."
                  "<br>The modeflip configuration will only advertise itself as an access point for a few seconds each minute, but will stay on if you connect to it. This is done to save power."
                  "<br>The cellsol pylons (either flavor) will work on any 3.6 or 3.7v batteries, or any USB power banks, or anything that delivers 3.5 to 6 volts, really."
                  "<br>Geolocation services are currently not available by default but if you have gps nothing prevents you from copying/pasting coordinates of course."
                  "<br>The project is open source and open schematic, but we can't host those on the device itself. Although that would be cool. Should we do that?"
                  "<br>There is no encryption for these messages (to make it easier for other systems to read). This is intended to be used in emergencies so that's really not a concern."
                  "<br>There is a large orange cat in my shop and I have no idea why."

                  "<p>Someone whose first language is English, please write a better help file that can be understood by regular people, I am too familiar with the architecture.</p><a href=\"/\">Go back</a></body></html>");
  PetTheWatchdog();
  client.println();
}
#endif

void sendstrings()
{
  for (byte i = 10; i > 0; i--)
  {
    PetTheWatchdog();
    //    client.println("<small><small>RX(" + String(i - 1) + "):</small></small>" + encodeHtml(string_rx[i - 1]) + "<br>");
    client.println("<small><small>RX(" + TimeAgoString(pseudoseconds - timestamp_rx[i - 1]) + "):</small></small>" + encodeHtml(string_rx[i - 1]) + "<br>");
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
  dnsServer.processNextRequest();

  // Do wifi things here
  client = server.available();   // Listen for incoming clients

#ifdef WIFI_IS_CLIENT
  if (client == false)
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
    unsigned long connect_timeout = millis()+3000;
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
      char c, c1, c2;
      if (client.available()) // if there's bytes to read from the client,
      {
        //  Serial.print("4");
        PetTheWatchdog();
        c2 = c1;
        c1 = c;
        c = client.read();

        if (header.length() < (MAXPKTSIZEP + 30)) // prevent buffer overflows, and keeps strings a reasonable size.
          header += c;

        //        Serial.print(c1);

        // detect two newlines
        if ((c == 10 and c1 == 10) or (c == 13 and c1 == 13) or (c == 10 and c1 == 13 and c2 == 10)) {              // if the byte is a newline character
          PetTheWatchdog();

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:

          //Serial.print(header);

          long derp = header.indexOf("Host: ");
          hoststring = header.substring(derp + 6);
          hoststring = hoststring.substring(0, hoststring.indexOf('\n') - 1) + "/";

          String whattoget = header.substring(4, header.indexOf(" HTTP/"));

          //Serial.print(hoststring);

          lwc = client.remoteIP();
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
            client.println("HTTP/1.1 204 No Content\r\n");
            //                client.println();// we're not actually sending anything
          }
#ifdef HELPLINK
          else if (whattoget.equals("/help.html")) // can  be done for other emergency files here
          {
            send_help_page();
          }
#endif
#ifdef COMMUNITY_MEMORY_SIZE
          else if (whattoget.startsWith("/last") and (whattoget.endsWith(".txt")))
          {
            int i = whattoget.charAt(5) - '0';
            if (i > -1 and i < 10)
              send_string_by_itself(string_rx[i]);
            else
              send_string_by_itself("");
          }
          else if (whattoget.equals("/mem.html")) // can  be done for other emergency files here
          {
            //Serial.println("Regular page (default)");
            send_ok_response();

            // Display the HTML web page
            // General purpose html header

            send_html_header();
            client.println( "<body>CellSol WiFi Pylon " VERSIONSTRING " at ");
            client.print(ipstring_a);
            client.println("<div style=\"max-width:100%;\"><p><small>");
            for (int i = COMMUNITY_MEMORY_SIZE - 1; i > -1; i--)
            {
              PetTheWatchdog();
              client.print("<br><small><small>RX(");
              client.print(i + 9);
              client.print("):</small></small>");
              client.println(communitymemory[i]);
            }
            client.print("<br>");
            sendstrings();
            client.println("</small></p><a href=\"http://" + hoststring + "\">Return to main</a></div></body></html>");
            client.println();
          }
#endif
#ifdef THE_INTERNET_IS_MADE_OF_CATS
          else if (whattoget.equals("/cat.jpg")) // can  be done for other emergency files here
          {
            SendBinaryFile("image/jpeg", cat_jpg, cat_jpg_size);
          }
#endif
#ifdef YOU_ARE_EATING_RECURSION
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
              client.println("<body><small><small><small>UTC Time Update:" + UTC_String(UTC_Seconds) + "<br></small></small>");
            else
              client.print("<body><small>");
            sendstrings();
            client.println("</small></body></html>");
          }
#ifdef TX_IFRAME
          else if (whattoget.equals("/answerform.html")) // answer form iframe
          {
            send_form_iframe();
          }
#endif
          else if (whattoget.startsWith("/get"))
          {

            if (whattoget.equals("/get?refresh=")) // exception is needed to make sure that the refresh button works properly
            {
              send_redirect_to_main(true);
            }
            else
            {

              // do external outputs
              gotstring = header.substring(header.indexOf("input1="));
              gotstring = gotstring.substring(0, gotstring.indexOf("HTTP/1.1"));
              gotstring = gotstring.substring(7, gotstring.indexOf("&refresh="));//HTTP/1.1")); //input1= is 7 characters
              if (gotstring.length() > (MAXPKTSIZE + 5)) // prevent intentional spam on arrival. the other input methods already do this
              {
                gotstring = "(>maxsize)" + gotstring.substring(0, 20) + "..."; // give a preview of what was sent, in case it's important and got thru even if it should not have
              }
              if (gotstring.length() > MAXPKTSIZEM) // prevent accidental spam // do not flood the lora
              {
                gotstring = gotstring.substring(0, MAXPKTSIZEM);
              }
              gotstring.replace('\r', ' ');
              gotstring.replace('\n', ' ');
              gotstring.trim();
              if (gotstring.length() > 1) // don't send empty strings or strings with just
              {
                char tempme[MAXPKTSIZE]; // slightly more efficient
                gotstring.toCharArray(tempme, MAXPKTSIZE);
                decode_in_place(tempme);
                gotstring = String(tempme);
                gotstring.trim();
                String hextag_temp;
                last_web_caller = client.remoteIP()[3];
#ifdef WIFI_IS_CLIENT
                lwc = client.remoteIP();
                hextag_temp = fourhex(lwc[0] ^ lwc[3], lwc[1] ^ lwc[2]); // should be unique enough, but mix it up so that we don't accidentally reveal someone's ip
#else
                if (last_web_caller < 16)
                  last_web_caller = last_web_caller + spare_id_nibble; // do SOMETHING with the third digit
                hextag_temp = fourhex(derpme, last_web_caller);
#endif
                lasttagimade = hextag_temp;

#ifdef GPS_SERIAL_1
                if (gotstring == "UTC")
                  gotstring = fourhex(derpme, spare_id_nibble + 0) + TAG_END_SYMBOL + "UTC:" + String(UTC_Seconds);
#endif

                gotstring = hextag_temp + TAG_END_SYMBOL + gotstring;

                if (string_rx[0].equals(gotstring) == false) // prevents sending it out twice if it's the last thing that went out
                {
                  Serial.println(gotstring);
#ifdef BT_ENABLE_FOR_AP
                  ESP_BT.println(gotstring);
#endif


                  //Send LoRa packet to receiver. Will need to make sure we don't send duplicates.

                  LoraSendAndUpdate(gotstring);
                  cyclestrings(gotstring);
                }

                gotstring = "";

              }
#ifdef TX_IFRAME
              send_form_iframe();
#else
              send_redirect_to_main(true);
#endif
            }
          }
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
            client.print("<body>CellSol WiFi Pylon " VERSIONSTRING " Option: <b>");
            client.print(option);
            client.println("</b><br>Redirecting to main</body></html>");
            client.println();
          }
          else if (whattoget.equals("/") or whattoget.startsWith("/index.html"))
          {
            //Serial.println("Regular page (default)");
            send_ok_response();
            // Display the HTML web page
            // General purpose html header

            send_html_header();
            client.println("<script>"
                           "function resizeIframe(obj) {"
                           "    obj.style.height = obj.contentWindow.document.documentElement.scrollHeight + 'px';"
                           //"    obj.style.width = obj.contentWindow.document.documentElement.scrollWidth + 'px';"
                           "  }"
                           "</script>"
                           "<body>CellSol WiFi Pylon " VERSIONSTRING " at ");
            client.print(ipstring_a);
            client.println("<br>"
#ifdef COMMUNITY_MEMORY_SIZE
                           "<a href=\"http://" + hoststring + "mem.html\">Older messages</a> "
#endif
#ifdef THE_INTERNET_IS_MADE_OF_CATS
                           "<a href=\"http://" + hoststring + "cat.jpg\">Cat picture</a> "
#endif
#ifdef HELPLINK
                           "<a href=\"http://" + hoststring + "help.html\">Help page</a> "
#endif
                           "<br>"
                           "<div style=\"max-width:100%;\">"
                           "<iframe id=\"chatin\" src=\"/chat.html\" frameborder=\"0\" scrolling=\"no\" onload=\"resizeIframe(this);\"style=\"width:100%;\" /></iframe>"
#ifdef TX_IFRAME
                           "<iframe id=\"chatout\" src=\"/answerform.html\" frameborder=\"0\" scrolling=\"no\" style=\"width:100%; height:3em;\" /></iframe></div>"
#else
                           "<form action=\"/get\"> <small><small>TX&gt;</small></small> <input type=\"text\" maxlength=\"160\" name=\"input1\"><input type=\"submit\" value=\"Submit\"><form><br>"
#endif
                          );
            /*
              "<iframe id=\"chatin\" src=\"/chat.html\" frameborder=\"0\" scrolling=\"no\" onload=\"resizeIframe(this);\"style=\"width:100%;\" />");
              // in case iframe doesn't work
              sendstrings();
              client.println("</iframe>"
              "<br>"
              "<iframe id=\"chatout\" src=\"/answerform.html\" frameborder=\"0\" scrolling=\"no\" style=\"width:100%; height:5em;\" />"
              // in case iframe doesn't work, it displays this
              "<form action=\"/get\"> Your message: <input type=\"text\" maxlength=\"50\" name=\"input1\"><input type=\"submit\" value=\"Submit\"><form>"
              "</iframe>");
            */
            client.println("<form action=\"/get\"><input type=\"submit\" value=\"Refresh this page (in case of errors, etc.)\"><input type=\"hidden\" size=\"1\" maxlength=\"1\" name=\"refresh\"><form>");
            client.print  ("<br><small>CellSol is a serverless relay chat for enabling communication between pylons in case of cell phone network disruption.<br>"
                           "Bluetooth terminal APK download (you may have to enter URL in browser manually): "
#ifdef PROVIDE_APK
                           "<a href=\"http://" + hoststring + "btt.apk\" target=\"_blank\">http://" + hoststring + "btt.apk</a> , <a href=\"http://f3.to/btt.apk\" target=\"_blank\">http://f3.to/btt.apk</a><br>To help this project grow, you must construct additional pylons. <br>Sysinfo: " PYLONTYPE " ");
#else
                           "<a href=\"http://f3.to/btt.apk\" target=\"_blank\">http://f3.to/btt.apk</a><br>To help this project grow, you must construct additional pylons. <br>Sysinfo: " PYLONTYPE " ");
#endif
            client.print(status_string());
#ifdef YOU_ARE_EATING_RECURSION
            client.print("<a href=\"http://" + hoststring + "src.zip\" target=\"_blank\">Source code</a> ");
#endif

            if (pseudoseconds % 2)
              client.println("<small> Deus Nolens Exitus</body></html>");
            else
              client.println("<small> Vigilo Confido</body></html>");


            // The HTTP response ends with another blank line
            client.println();
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
              client.println("<body>CellSol WiFi Pylon " VERSIONSTRING " " PYLONTYPE" does not hold " + whattoget + "<br>Redirecting to main</body></html> ");
              client.println();
            }
          }
          header = "";
          break;
        }
      }
    }
    // Clear the header variable
    header = "";
    client.stop();
    DoBasicSteps();
    currentlyserving = false;
  }
}

void loop() {


  if (wifimode)
  {
    DoWifiSteps();
    ServeWebPagesAsNecessary();
  }
  else
  {
    DoBtSteps();
  }
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
