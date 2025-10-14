//define the pins used by the LoRa transceiver module. note that these change between ttgo and lora32!
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

/**
   BATTERY ADC BLOCK: DEFINE THESE TWO PINS TO ENABLE BATTERY LEVEL SENSOR
   SOME VERSIONS OF THE LORA32 WILL REQUIRE EXT_PWR_PIN 12 HIGH AND BATT_ADC 13
   DO NOT SET EXT_PWR_PIN LOW ON THESE YOU MAY FRY YOUR ADC CHECK HELTEC DOCUMENTATION
*/

#define EXT_PWR_PIN 21
#define BATT_ADC 37

// Assign output variables to GPIO pins
#define USER_BUTTON_PIN 0
#define USER_LED_PIN 25
#define enablecomstring "COMCHIAVE"
#define PYLONMODEL " "


#ifdef LORA32V3
#undef GPS_SERIAL_1 // no gps on the 
#define EXT_PWR_PIN 36
#define BATT_ADC 1
#define SCK 9
#define MISO 11
#define MOSI 10
#define SS 8
#define RST 12
#define DIO0 14
#define USER_BUTTON_PIN 0
#define USER_LED_PIN 35
#define PYLONMODEL "L3"



#define RADIO_DIO_1    14
#define RADIO_NSS      8
#define RADIO_RESET    12
#define RADIO_BUSY     13
#define LORA_CLK       9
#define LORA_MISO      11
#define LORA_MOSI      10


//OLED pins
#define OLED_SDA 17
#define OLED_SCL 18
#define OLED_RST 21
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#endif

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
#define PYLONMODEL "L2"

#define RADIO_RESET                                14

#define LORA_MOSI                                  27
#define LORA_MISO                                  19
#define LORA_CLK                                   5
#define RADIO_NSS                                  18

#define RADIO_DIO_0                                26
#define RADIO_DIO_1    35   // GPIO35 -- SX127x's IRQ(Interrupt Request) V2


#endif

#ifdef TBEAM
#define GPS_SERIAL_1 1800 // every this many seconds, send out UTC seconds. Internal is updated as often as they come in
#define NODISPLAY // force no display since the system doesn't have it
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DIO0 26
#define NO_BINARY_SUPPORT
#undef TUTORIALSTRINGS
#undef SERVE_FAQ_PAGE
#define USER_BUTTON_PIN 38
#undef USER_LED_PIN
#undef EXT_PWR_PIN
#undef BATT_ADC
#define enablecomstring "" // not needed
#define PYLONMODEL "TB"
#endif

#ifdef LORA32V1
#define MINDISPLAY // the display works, as such, but we're running out of progmem here, so just initialize to show bluetooth or wifi and the IP address.
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26
#define USER_LED_PIN 2
#define NO_BINARY_SUPPORT
#undef TUTORIALSTRINGS
#undef DEBUG_OPTIONS_PAGE
#undef SERVE_FAQ_PAGE
#undef EXT_PWR_PIN
#undef BATT_ADC
#undef COMMUNITY_MEMORY_SIZE
#undef MODEFLIP
#define USER_BUTTON_PIN 0
#define PYLONMODEL "L1"
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define enablecomstring "COMCHIAVE"
#endif

// binary support undef. must be at end of file
#ifdef NO_BINARY_SUPPORT
#undef PROVIDE_SOURCE_CODE
#undef PROVIDE_CAT_PICTURE
#undef PROVIDE_APK
#endif
