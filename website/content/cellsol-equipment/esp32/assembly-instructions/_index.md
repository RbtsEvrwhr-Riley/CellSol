# CellSol ESP32 Pocket Pylon Setup

## What You Will Need
* A Heltec Lora32 or Lora32v2 module
* The Arduino software, available [here](https://www.arduino.cc/en/Main/Software)
* A USB cable
* Our CellSol ESP32 Pocket Pylon firmware, available on this GitHub repository
* A battery for the Lora32 - 1S LiPo or other 3.3-4V source. See [Powering the Node](power)
* A solar panel to charge the battery (optional, USB may also be used for charging if the pylon is connected to mains)
* A case for your Lora32, antenna, and battery - see this website for case designs (coming soon)

## Step 1: Setting Up the Arduino Software

### 1.1 Board Manager
To program your Lora32, you will need to have the board manager in the arduino software configured to support it. You will also need to install several arduino libraries used by our firmware.

You will need to add a URL to the Board Manager before starting. In the Arduino software, navigate to File -> Preferences... and in the dialog box, click the browse icon next to "Additional Board Manager URLs". In the text box that opens, add a new line, and enter the following URL:
[https://dl.espressif.com/dl/package_esp32_index.json](https://dl.espressif.com/dl/package_esp32_index.json)

{{< figure src="./photos/fig1_boardmanagerurls.png" title="Board Manager URLs After Adding Espressif ESP32" >}}

Once this URL is added, you can go on and add the Lora32 boards using the Board Manager.

Navigate to Tools -> Board -> Boards Manager...

Enter "Heltec" in the search, and "Heltec ESP32 Series Dev-Boards" should appear. Click "Install" at the bottom right. Once installed, this button will be replaced by "Remove".

{{< figure src="./photos/fig2_boardmanager.png" title="Board Manager After Installing Heltec ESP32" >}}

### 1.2 Libraries

With the boards configured, it is on to configuring the libraries. Navigate to Tools -> Manage Libraries...

A window similar to the Board Manager will open.

{{< figure src="photos/fig3_librarymanager.png" title="Arduino Library Manager" >}}

You will need to search for the following libraries, and, just like installing the boards in Board Manager, install them:
* Adafruit BusIO
* Adafruit GFX Library
* Adafruit SSD1306
* Heltec ESP32 Dev-Boards
* LoRa (by Sandeep Mistry)

{{< figure src="photos/fig4_librarymanager_lora.png" title="LoRa Library - This is the one to use!" >}}

## Step 2: Setting Up the Lora32 for Programming

With the Lora32 unplugged from your computer, navigate to Tools -> Port and look at the serial ports listed there.

Now, plug in your Lora32 using the USB cable. Your operating system should automatically detect it as a serial port, and configure that port. Navigate again to Tools -> Port. A new one should have appeared: this will be the serial port for the Lora32 you just plugged in. Click on it to select it for programming.

You also need to tell the arduino software that it is using a Lora32 or 32V2. Do this by again navigating to Tools -> Board -> Heltec ESP32 Arduino and selecting Lora32 (black board) or Lora32V2 (white board) depending on which one you are using.

{{< figure src="photos/fig5_selectlora32.png" title="Selecting the Lora32 Board" >}}

The other settings for the Lora32 board can be left default; even if you are not in the EU, the REGION setting is not used by the firmware.

## Step 3: Configuring the Firmware

There are several firmware settings you will need to choose in order to get the performance you want out of your pocket pylons.

If you have not already done so, open the firmware's arduino project by File -> open, or using your system's file manager, and opening "esp32_wifi_bt.ino" that you downloaded at the beginning.

All of these options are configured in #define lines at the top of the root file "esp32_wifi_bt.ino"

Most of these options are turned on and off by commenting (making the line begin with //) or uncommenting (removing the //) the #define statements. The options you must set to make your pocket pylon work are explained below. The options not described are intended for developers, for more advanced configuration - you don't have to worry about them right now.

|Option Name|Purpose|Allowed Values|
|-----------|-------|--------------|
|WIFI_IS_CLIENT|Activates wifi gateway mode if uncommented. This connects the CellSol network to a wifi router.|None|
|WIFI_IS_HYBRID|Activates wifi hybrid mode if uncommented (gateway + AP). Takes priority over WIFI_IS_CLIENT.|None|
|DHCP|Uses DHCP for connecting to an upstream router (gateway or hybrid mode) if uncommented|None|
|REPEATER_ONLY|Emulates the Arduino repeater, turning off the wifi features entirely, if uncommented. Takes priority over everything.|None|
|CLIENT_IP_ADDR|Client IP address, with the numbers separated by commas; only used in IS_CLIENT or IS_HYBRID and only if DHCP is off.|An IP address separated by commas such as 192,168,1,55|
|GATEWAY_IP_ADDR|IP address of the gateway router, with the numbers separated by commas; only used in IS_CLIENT or IS_HYBRID and only if DCHP is off.|An IP address separated by commas such as 192,168,1,1|
|GATEWAY_SUBNET|Subnet mask for the gateway router, with the numbers separated by commas; only used in IS_CLIENT or IS_HYBRID and only if DCHP is off.|An IP subnet mask separated by commas such as 255,255,255,0|
|WIFI_UPSTREAM_AP|The access point name to be connected to if in IS_CLIENT or IS_HYBRID mode.|Network name in double quotes, like "RobotsEverywhere_24"|
|WIFI_UPSTREAM_PWD|The password to the access point to connect to in client or hybrid mode|Password in double quotes, like "derpderp"|

## Step 4: Assembling the Power System

See [Powering the Node](power) for information on what batteries and charging equipment can be safely connected.

## Step 5: Assemble the Case

This is TBD, case design is a work in progress.