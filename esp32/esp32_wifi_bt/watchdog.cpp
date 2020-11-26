// watchdog
#include <esp_task_wdt.h>
#define WDT_TIMEOUT 4 // in seconds
#include "Arduino.h"
#include "config.h"

unsigned long pseudoseconds = 0;
unsigned long psm = 0;
long UTC_Seconds = 0;
extern int battimeout;


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

extern bool is_watchdog_on;

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
