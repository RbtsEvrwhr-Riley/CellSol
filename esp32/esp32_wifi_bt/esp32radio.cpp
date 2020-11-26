#include <LoRa.h>
#include "config.h"

bool has_lora_been_initialized = false;
long LastThingISentViaLora_3 = 0;
long LastThingISentViaLora_2 = 0;
long LastThingISentViaLora_1 = 0;
long LastThingISentViaLora_0 = 0;

long LTISVL_3_time = 0;
long LTISVL_2_time = 0;
long LTISVL_1_time = 0;
long LTISVL_0_time = 0;
#define ANTISPAM_TIME_SERIAL 1 // for serial
bool has_serial_been_initialized;
bool has_bluetooth_been_initialized = false;
byte charcounter = 0;
boolean readytosend = false;
byte charcounte2 = 0;
boolean readytosen2 = false;

//TODO: These should probably not be here.
boolean dodisplay = false;
boolean dodisplaybuf = false;
boolean enablecomport = false; // set to false for a solderless fix for UART buffer crosstalk hardware issue

#include "BluetoothSerial.h"

BluetoothSerial ESP_BT;

// TODO: these are used for web, not radio, unsure why they're hard coupled in here.

String hextag  = "XXXX"; // usually the last two IP octets; gives pseudonimity to sender
String lasttagimade = "XXXX"; // last one we made for use in checking


int rssi; // last packat received rssi
String LoRaData;

char receivedChars[MAXPKTSIZE]; // an array to store the received data
char receivedChar2[MAXPKTSIZE]; // an array to store the received data

// Used as a utility
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


#define ANTISPAMTIME 3 // in (pseudo) seconds. for radio

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