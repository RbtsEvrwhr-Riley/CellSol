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
// switching between wifi and bluetooth is done at startup with the normal mode, not here; push the button immediately after a reset.
// if more than one is uncommented, lowest line wins.

// The first mode saves power but makes the AP invisible 45 seconds out of every minute.
// #define MODEFLIP 15000 // minimum suggested: 12000. powershare mode operates the pylon on full AP for this many milliseconds every minute, and as a repeater the rest of the time. It is exclusive with the other options. The mode flip will pause if there are clients connected or if the battery is full enough.

// The last three modes use more power than standard mode!
//#define BT_ENABLE_FOR_AP // Uncomment this to also enable Bluetooth when the pylon is running as an access point. THIS WILL EAT UP A LOT OF POWER! Only enable if you're using a big battery and a big panel, or have line power.
//#define WIFI_IS_CLIENT // Uncomment this to enable use as a gateway. Bluetooth will be on. Note that gateway mode must be configured manually and will need an open port on the router. If you don't know what this does, leave it alone!
//#define WIFI_IS_HYBRID // Uncomment this to enable use as BOTH a gateway and an AP. Bluetooth will be on. Performance will be slower than either. Note that this won't allow people to get on the internet through the AP, so it's ideal if you want to let people use cellsol without a password, but not use the internet.

//#define REPEATER_ONLY // Uncomment this to bypass everything else and runs as repeater (and serial) only. useful if we are out of arduinos. not useful otherwise.

//#define NODISPLAY // display is either absent or should be kept off at all times. Saves some power, naturally.

#define SEND_TWICE // if defined, allow sending a packet twice after a pseudorandom delay, in case the first one got lost
#define RSSI_TRE_LO -120 // if sendtwice is defined, below this (for the last 4 received packets), turn on sendtwice
#define RSSI_TRE_HI -100 // if sendtwice is defined, above this (for the last 4 received packets), turn off sendtwice

/**
 * WEBPAGE CONFIG
 */
#define TUTORIALSTRINGS // On first activation after a poweroff/reset, should we display a mini help on the screen?

#define PROVIDE_APK // make bluetooth terminal apk available?
#define PROVIDE_CAT_PICTURE // load cat picture as an example of embedded file?
#define PROVIDE_SOURCE_CODE // embed source zip in source for the glory of recursion?
#define SERVE_FAQ_PAGE  // if undefined, do not add help page
#define REFRESH_CHAT_EVERY 3 // seconds

#define REQUIRE_TAG_FOR_REBROADCAST // if defined, require xxxx: tag for rebroadcast.
//#define REQUIRE_TAG_FOR_REBROADCAST_STRICT // on top of that, the first four characters must be hex digits.
#define BAD_CHARACTERS_MAX_DIVIDER 10 // 1/x of bad characters before we discard the string
//#define SHOW_RSSI // if enabled, show RSSI for wireless packets coming in.

#define TAG_END_SYMBOL ':'

/**
 * NETWORK CONFIGURATION
 */
//This is the root name for the AP. It will be used when in AP mode.
#define SSIDROOT "CellSol "

// these only have an effect in client and hybrid mode; the IP address for AP mode is always 192.168.(autocalculated).1
// the upstream router will have to either open port 80 to this, or do a redirect.
#define CLIENT_IP_ADDR 192,168,2,55 // client IP address for client or hybrid mode. needs commas instead of periods
#define GATEWAY_IP_ADDR 192,168,2,1 // router IP address for client or hybrid mode. needs commas instead of periods
#define GATEWAY_SUBNET 255,255,255,0 // subnet mask for client or hybrid mode. needs commas instead of periods
#define WIFI_UPSTREAM_AP "RobotsEverywhere_24" // SSID of the router we're trying to connect to. 
#define WIFI_UPSTREAM_PWD "derpderp"// password for the router we're trying to connect to. Use "" for none/open.

#define DHCP // DHCP on if defined; will ignore IP address setting fields above in that case.

// irc relay stuff (only used if wifi is client or hybrid)

#define IRC_SERVER   "irc.rizon.net" // comment this line out to avoid using irc subsystem
#define IRC_PORT     6667
#define IRC_NICK_ROOT "CellSol" // will add hextag
#define IRC_CHAN_ROOT "CellSol" // will add hextag if the line below is uncommented
//#define IRC_CHAN_HEXTAG
#define FWD_PREFIX "~~" // if there's a prefix, forward it to the radio stuff, otherwise don't.
#define IRC_FAILED_TRE 9 // if more than this many fails, stop trying
#define IRC_TOPIC "CellSol relay channel! Start your message with " FWD_PREFIX " to forward to radio. Example: " FWD_PREFIX "Please report!" // Edit this to fit, specifically if you remove FWD_PREFIX.

#define USE_BATTERY_NOISE_FOR_ID // if undefined, same id across power cycles. if not, use battery level to get a bit of noise in the ID (mostly to avoid creepy people hashing it to figure out where you are).
#define DO_NOT_LOG_SYSTEM_PACKETS // Don't display system packets to avoid spamming out human messages (example: UTC fix)

//#define DEBUG_OPTION_PAGE // if you don't know what this is, don't use it.

// this affects wifi range. obviously it will affect power consumption. it does not affect lora range. for line powered stuff, it'll be turned up to 11. For default, keep it commented out.
#define WIFI_POWER_LEVEL 4 // 0 to 11, higher = stronger. lora power level is fixed at 19/20 because 20/20 can mess up some radios.


/**
 * BATTERY & SLEEP CONFIGURATION
 */

// more timing stuff
#define SLEEP_TIME 60 // this is in seconds - how long to sleep before waking up and checking power level again? NOT ACCURATE.
#define DISPLAY_INTERVAL 60 // In milliseconds, how long to keep the display on after button release, if it's there? NOT ACCURATE.

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


/**
 * DON'T TOUCH THESE, THEY ARE AUTOMATED
 */
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
#ifdef IRC_SERVER
#define PYLONTYPE "(STA+BT+I)"
#endif
#endif
#ifdef WIFI_IS_HYBRID // if both are uncommented, it'll go to HYBRID Mode.
#define RECONNECT_EVERY 2000000000 // every this many milliseconds, reconnect to the upstream wifi (useful in case your router is prone to crapping out, or does load balancing)
#define PYLONTYPE "(AP+STA+BT)"// identifier for how we are running
#ifdef IRC_SERVER
#define PYLONTYPE "(AP+STA+BT+I)"
#endif
#endif

/**
 * FURTHER AUTOMATED STUFF
 */
#ifdef REPEATER_ONLY
#define LPLOOP_BLINK 5000 // every this many cycles (not milliseconds!), blink the led
#define ADC_INTERVAL 10000 // In milliseconds, how often to read the battery level? NOT ACCURATE.
#define FULL_BATT 1450 // unused but a good reference. more than this = there probably is no battery
#define BATT_HIGH_ENOUGH_FOR_FULL_POWER 2000 // above this, allow wifi/bt
#define BATT_TOO_LOW_FOR_ANYTHING 1065 // below this, don't try to turn on, go back to sleep and wait for better times
#define BATT_HYSTERESIS_POWER 15 // hysteresis between power state changes
#define POWER_STATE_CHANGE_ANNOUNCE false //  should the module announce it when it's coming online or going offline? (probably only for debugging)
#define REFRESH_CHAT_EVERY 42 // doesn't really matter since it neveer gets called
#define PYLONTYPE "L0WPWR"// identifier for how we are running
#undef TUTORIALSTRINGS
#endif
#ifndef WIFI_IS_CLIENT
#undef IRC_SERVER // we aren't connecting to the wider internet
#endif
