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
// #include <LoRa.h>
#include <SPI.h>


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
// Set web server port number to 80
WiFiServer server(80);

DNSServer dnsServer;

WiFiClient client;

int batt_delta;
int battimeout = 0;
bool low_batt_announce = false;
int lploops = LPLOOP_BLINK - 10; // give me a blink early so i know we're alive
//bool has_serial_been_initialized = false; TODO: refactor
//bool has_bluetooth_been_initialized = false;
bool has_wifi_been_initialized = false;
bool has_display_been_initialized = false;

byte derpme = 0; // third number of ip address
//byte spare_id_nibble = 0; // upper nibble; lower nibble always 0

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
//byte charcounter = 0;
//boolean readytosend = false;
//byte charcounte2 = 0;
//boolean readytosen2 = false;
//boolean dodisplay = false;
//boolean dodisplaybuf = false;
boolean displayexists = false;
bool displayenabled = true;





// Variable to store the HTTP request
String header;

#ifdef COMMUNITY_MEMORY_SIZE
String communitymemory[COMMUNITY_MEMORY_SIZE];
#endif
String string_rx[10];

bool wifimode = true; // bluetooth or wifi


#ifdef USER_BUTTON_PIN
#else
#undef MODEFLIP_BUTTON
#endif

// if this exists, only show the display when we are pushing the button that way we know someone is actually looking at it (saves power)
#define user_button_display


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

// important: this should be copy/pasted exactly between hardware types.

void SleepLowBatt()
{
  String tempstring = ":SYS:SLEEP " + String(derpme) + ":" + status_string();
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
  String tempstring = "";
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


  String tempstring = ":SYS:LOWPWR " + String(derpme) + ":" + status_string();
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
  String tempstring = ":SYS:" PYLONTYPE " " + String(derpme) + ":" + status_string();
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
        break;
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
