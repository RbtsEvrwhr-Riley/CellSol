// test
#include "BluetoothSerial.h"
#include "config.h"
#include "watchdog.cpp"

extern String hextag; // move this here?
extern byte derpme;
byte spare_id_nibble;

// decide whether to actually deal with this packet or not
inline bool IsValidChar(char i) {
  return (i == 13 || i == 10 || (i > 31 && i < 128));
}
inline bool IsHex(char i) {
  return ((i > 64 && i < 71) || (i > 96 && i < 103) || (i > 47 && i < 58)); // AF, af, 09
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

extern int currbatterylevel;
extern bool broadcast_twice;
extern String string_rx[10];

/**
 * This method is highly coupled to display, battery, etc. It would be ideal to break that stuff out, but it would likely slow down the system significantly.
 */
void ReadFromStream(Stream &st, char buf[], byte &cnt, bool &sendout, bool streamexists, int whichtag)
{
  String tempstring = "";
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

/**
 * I want to centralize all of this but right now we need to extern them.
 */
#ifdef COMMUNITY_MEMORY_SIZE
extern String communitymemory[COMMUNITY_MEMORY_SIZE];
#endif
extern String string_rx[10];
long timestamp_rx[10];
bool display_on_ping = false;

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
