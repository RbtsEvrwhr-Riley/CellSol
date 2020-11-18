CellSol Design

{{< toc >}}

# Rationale

We want to provide a last-mile mesh network to allow people to communicate and access knowledge in the event of a natural, social, or other disaster. Network infrastructure is intended to be inexpensive,
locally powered, and easy to build, maintain, and replace. It should be lightweight, and as interoperable as is feasible with other network technologies and the broader Internet.

# Design

Our network design utilizes a combination of "fixed pylons" (fixed repeaters) placed mostly on the rooftops and outside of buildings and "pocket pylons" (mobile repeaters that live in peoples' pockets 
and backpacks) to allow for more "hops" across the network. The pocket pylons will also provide wifi so that any wifi-enabled device can connect to the LoRa network and send and receive messages over it.

![cellsol mesh network diagram](../cellsol_meshdiagram_large.png)

Our "pylons" are very simple devices that do one of three things:
1. Have a list of phones/computers that are connected to me directly; serve a web or bluetooth interface to that device to allow terminal communication
2. Send incoming messages to connected devices
3. Re-transmit incoming messages along the network

The above is true whether there ARE endpoints (phones, PCs, WiFi/Ethernet routers) connected to the node or not, so it can always be the same bit of hardware.

The "re-transmit" can be done in a basic mesh format (always re-transmit everything), and routing can be implemented in the future. Each node is capable of being a router,
if the packet format is adapted for more complex addressing in the future.

A node also provides a gateway from other devices (via BLE or WiFi) INTO and OUT OF the mesh network, and also provides routing WITHIN the mesh.

# Hardware

The RF standard we are using is [LoRa](https://www.semtech.com/lora/what-is-lora) which is an 866MHz(EU) and 915MHz(North America and most other regions) standard.

The hardware is deliberately as simple as possible so that it can be made out of readily available parts in the field. The most specialized components are for the radio - the Semtech Sx1276 and a 915MHz
antenna. That said, any LoRa chip will work, and monopole antennas can be made from short lengths of wire. See our [antennas page](../cellsol-equipment/antennas/).

The [Heltec Lora32](https://heltec.org/project/wifi-lora-32/) and Arduino Nano(https://www.arduino.cc/) are chosen for microcontrollers, due to their simplicity, with the LoRa32 being a fully integrated out-of-the-box LoRa device, so no soldering or significant setup is needed.

## Hardware Philosophy

For the different types of devices, identical hardware is used for the repeater/transmitter parts. Whether you are making a WiFi pocket pylon, or a repeater, or a fixed pylon to place on top of a building,
using an ESP32, Arduino, or even a future design with a more powerful computer. The power systems are also designed to scale in the same way.

Any device on the CellSol network is able to receive and rebroadcast messages.

The overall design is intentionally flexible so that users can design their own pylons and connect up to the devices shown here.

## WiFi Pylons 

[WiFi pylons](../cellsol-equipment/esp32), or "pocket pylons", are designed to be endpoints that connect to existing WiFi networks or devices. They can act as access points for wireless clients, or act as a client and connect to an
existing WiFi network, providing CellSol connectivity to all devices on that network. These use the Heltec Lora32 board at their core. These will serve a bulletin-board style chat application via http,
allowing any device with a web browser or ability to send GET requests to be able to talk to the rest of the mesh network.

## Repeater Pylons

[Repeater pylons](../cellsol-equipment/arduino) are a lower cost device that is designed to be a network extender, without providing web endpoint functionality. Bluetooth connectivity can be built onto
these, so they can also act as an endpoint for a single device, talking via Bluetooh serial port. These are designed around an Arduino and the Sx1276, and can be built for very low cost if done in large
volume runs. [Robots Everywhere](https://www.robots-everywhere.com) will be developing a kit for this model of pylon.

## Power requirements - Using a Solar Panel

Although users will want a battery connected to both pocket and fixed pylon repeaters, the battery will quickly run out if it is not supplemented by a solar panel that recharges the battery during the day. For this reason, we have integrated a solar panel into the design.

Note that, for the pocket pylons, it is more likely that you will want to use an additional battery or two to keep your pylon going over the course of the day, rather then depend on a solar panel being able to recharge it successfully while you are walking around. For fixed pylons however, especially those living on roof tops, a 5v solar panel will do quite nicely at keeping the battery charged.

These "pocket pylons" that we are building are able to use a 5v solar panel as-is. (5.6v is also within tolerances).

Remember to plug in your batteries and let them charge overnight the night before you assemble your repeaters. This will ensure that it's charged up as much as possible.

The way this works, is by placing the battery in-between the esp32 module and the solar panel, allowing the solar panel to keep the battery charged.

Note: Neither the ESP32 nor the Arduino is able to be powered directly from the panel, because solar panels don't collect at a constant voltage. So, if you purchased say, a 5 watt solar panel, it's really going to be collecting at 4.5 watts on some days and 5.6 watts on other days, but the devices that need to be powered require a consistent 5 watt stream.

# Software

We want to stay very light on software, focusing primarily on providing text over serial. End-to-end encryption can be handled client side by any specialized clients that want to use it, same for
compression. WiFi pylons will serve a simple web chat application, allowing for communication between users.

This is all in the future; the current software is a simple APK for Android phones to connect to Bluetooth-enabled Repeater Pylons. To communicate with
the WiFi Pylons, simply connect to their web interface, which is built into the firmware.