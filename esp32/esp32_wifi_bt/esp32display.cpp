#include "datastreams.cpp" // TODO: merge data and display sort of?
int countme = 0;


extern bool dodisplay; // TODO: definitely centralize display
extern bool dodisplaybuf;
extern String string_rx[10];

extern String ipstring;
extern String ipstring_c;
extern String ipstring_a;
extern String ipstring_b; // used in bluetooth mode

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