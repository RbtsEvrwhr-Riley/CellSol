// honk

#include "watchdog.cpp"
#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
extern WiFiClient client;
extern bool dodisplay;
long timestamp_rx[10];
extern String string_rx[10];
extern String communitymemory[COMMUNITY_MEMORY_SIZE];

// Web constants for text align
#define TEXT_ALIGN_STRING_A "center"
#define TEXT_ALIGN_STRING_B "justify"
String TEXT_ALIGN_STRING = TEXT_ALIGN_STRING_A;
static RTC_NOINIT_ATTR bool centertext = true;// if this, center, otherwise, justify


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
extern String ipstring; // I don't like having to extern this but we have no choice.

extern String ipstring_a;
String hoststring = "/";

void send_string_by_itself (String st)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/plain");
  client.println("Connection: close");
  client.println();
  //  client.println(encodeHtml(st));
  client.println(st);
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

void send_ok_response()
{
  PetTheWatchdog();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
}


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


// sends the chat strings over the wireless UART
void sendstrings()
{
  for (byte i = 10; i > 0; i--)
  {
    PetTheWatchdog();
    client.println("<small><small>RX(" + TimeAgoString(pseudoseconds - timestamp_rx[i - 1]) + "):</small></small>" + (string_rx[i - 1]) + "<br>");
  }
}

void send_chat_iframe()
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

#ifdef SERVE_FAQ_PAGE
void send_faq_page()
{
  send_ok_response();
  send_html_header();
  client.println( "<style> html{text-align: start;}</style>"
                  "<body><h1>CellSol WiFi Pylon " VERSIONSTRING " Help</h1>"
                  "<p>The full help/howto is available, when the internet works, at <a href=\"http://f3.to/cellsol/\">http://f3.to/cellsol/</a></p><p>"
                  "<h2>About the Chat<h2>"
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
                  "<p>The best use for a WiFi pylonis somewhere that has a stable internet connection (satellite, etc.) or generally somewhere where people go (a waypoint, base camp, etc.)"
                  "<h2>About This Pylon</h2>"
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
#ifdef BT_ENABLE_FOR_AP
                  "<p>This pylon can be accessed both from WiFi and Bluetooth. Please note that this drains the battery fairly quickly."
#else
                  "<p>This pylon is in WiFi mode right now. You can switch to Bluetooth mode by pushing the PROG button right after a reset and keeping it pushed until the screen says Bluetooth."
#endif
#endif
                  "<p>The CellSol pylons (any version of them) will work on any 3.6 or 3.7v batteries, or any USB power banks, or anything that delivers 3.5 to 6 volts, really."
                  "<h2> About the Project</h2>"
#ifdef YOU_ARE_EATING_RECURSION
                  "<p>The project is open source and open schematic. You can get a copy of the source right from here! <a href=\"/src.zip\"> Source code </a>."
#else
                  "<p>The project is open source and open schematic. You can download everything from <a href=\"http://f3.to/cellsol/\">http://f3.to/cellsol/</a> or from a pylon that carries it (This one doesn't have enough memory to)."
#endif
                  "<p><a href=\"/\">Go back</a></body></html>");
  PetTheWatchdog();
  client.println();
}
#endif


void send_mem_page()
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
    client.print("<br><small><small>RX");
    //              client.print(i + 9);
    client.print(":</small></small>");
    //              client.println(encodeHtml(communitymemory[i]));
    client.println(communitymemory[i]);
  }
  client.print("<br>");
  sendstrings();
  client.println("</small></p><a href=\"http://" + hoststring + "\">Return to main</a></div></body></html>");
  client.println();
}


char c1 = 0;
char c2 = 0;
char c = 0;
extern String header; // necessary because it's a magic extern in the driver
extern IPAddress lwc; // these need to talk to each other so the display works
extern IPAddress AP_IP;
extern IPAddress IP;
extern IPAddress Client_IP;
extern long LastReconnect;

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


extern char receivedChars[MAXPKTSIZE]; // an array to store the received data
extern char receivedChar2[MAXPKTSIZE]; // an array to store the received data
extern bool has_serial_been_initialized;
extern bool has_bluetooth_been_initialized = false;
extern byte charcounter = 0;
extern boolean readytosend = false;
extern byte charcounte2 = 0;
extern boolean readytosen2 = false;
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


void serveWebSiteRequests(char c)
{
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
		send_mem_page();
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
		send_chat_iframe();
	  }
	  else if (whattoget.startsWith("/get"))
	  {
		// get header value for input1 which should be message
		gotstring = header.substring(header.indexOf("input1="));
		gotstring = gotstring.substring(0, gotstring.indexOf("HTTP/1.1"));
		// We no longer need to pull the refresh part, since we're not refreshing from /get anymore.
		
		if (gotstring.length() > (MAXPKTSIZE + 5)) // prevent intentional spam on arrival. the other input methods already do this
		{
		  gotstring = "(>maxsize)" + gotstring.substring(0, 20) + "..."; // give a preview of what was sent, in case it's important and got thru even if it should not have
		}
		if (gotstring.length() > MAXPKTSIZEM) // prevent accidental spam // do not flood the lora
		{
		  gotstring = gotstring.substring(0, MAXPKTSIZEM);
		}
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
		  if (gotstring.startsWith("UTC:"))
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
#endif
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
					   "  }\r\n</script>"

					   "<script>function sendMsg(){\r\n"
					   "getSend('\\get',document.getElementById('msgfield').value); document.getElementById('msgfield').value=\"\";\r\n"
					   "}\r\n"
					   "function getSend(url, input1=null) {\r\n" // we default undefined so that we don't have to explicitly specify it; it just gets passed to the next function
					   "var xhttp = new XMLHttpRequest();\r\n"
					   "xhttp.open('GET', url+\"?input1=\" + input1, true); xhttp.send();" // we don't care about the response, we are just dumb sending.
					   "}"
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
#ifdef SERVE_FAQ_PAGE
					   "<a href=\"http://" + hoststring + "faq.html\">FAQ page</a> "
#endif
					   "<br>"
					   "<div style=\"max-width:100%;\">"
					   "<iframe id=\"chatin\" src=\"/chat.html\" frameborder=\"0\" scrolling=\"no\" onload=\"resizeIframe(this);\"style=\"width:100%;\" /></iframe>"
#ifdef TX_IFRAME
					   "<iframe id=\"chatout\" src=\"/answerform.html\" frameborder=\"0\" scrolling=\"no\" style=\"width:100%; height:3em;\" /></iframe></div>"
#else
					   "<small><small>TX&gt;</small></small> <input id=\"msgfield\" type=\"text\" maxlength=\"160\" name=\"input1\"><button onClick=sendMsg() value=\"Send\">Send</button><br>"
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
		client.println("<button value=\"Refresh this page (in case of errors, etc.)\" onClick=getSend('/refresh')>Refresh this page (in case of errors, etc.)</button>");
		client.print  ("<br><small>CellSol is a serverless relay chat between LoRa pylons. It is intended for enabling communication in case of cell phone network disruption.<br>"
					   "Bluetooth terminal APK download (you may have to enter URL in browser manually): "
#ifdef PROVIDE_APK
					   "<a href=\"http://" + hoststring + "btt.apk\" target=\"_blank\">http://" + hoststring + "btt.apk</a> , <a href=\"http://f3.to/btt.apk\" target=\"_blank\">http://f3.to/btt.apk</a><br>To help this project grow, you must construct additional pylons. <br>Sysinfo: " PYLONTYPE " ");
#else
					   "<a href=\"http://f3.to/btt.apk\" target=\"_blank\">http://f3.to/btt.apk</a><br>To help this project grow, you must construct additional pylons. <br>Sysinfo: " PYLONTYPE " ");
#endif
		client.print(status_string());
#ifdef YOU_ARE_EATING_RECURSION
		client.print("The source code for all platforms is available <a href=\"http://" + hoststring + "src.zip\" target=\"_blank\">here.</a>");
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
