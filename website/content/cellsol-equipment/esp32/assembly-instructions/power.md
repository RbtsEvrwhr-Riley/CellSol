---
title: Power Circuits
---

# POWERING THE NODE

The node is designed to be very tolerant when it comes to input power, so that it can be run in a variety of emergency situations.

The node takes 4 to 6.5V power. You can provide this from USB, a solar panel, or a number of other sources such as:

* USB power bank. It has to be plugged into the USB power input port. You can break down a USB cable to do this and connect the red and black wires.

* 6V lead-acid battery, connected to the USB power input port.

* 6V bike dynamo. Using a diode in series with the node power input is recommended (1N4004 or similar). If you use this, the node MUST have a secondary battery.

* 3 alkaline cells (AAA to D cells) in series. Important: these must be NON-rechargeable!

* Anything that outputs USB (car adapter, wall plug, spare USB port on a wifi router, 10 pounds of potatos connected in series, etc)

* USB hand crank. If you use this, the node MUST have a secondary battery.

## BATTERY TYPES FOR THE NODE

You will need a 3.5 to 4 volt battery to keep the node operating overnight (or when its primary power input is off). The node has been designed to be compatible with a variety of battery typologies. Any one of these will work. If you want to use more than one battery, connect them in parallel. In this case you may NOT mix battery types!

* None. The node has a two stage voltage regulator (switching to 3.7v and linear to 3.3v) and a filter capacitor, so it's safe to run it without a battery if it has continuous power coming in. The exception, as noted above, is if you have a USB hand crank or bike dynamo.

* 3.7V LiPo battery. Any type is fine (18650, old quadcopter battery, etc). If it has no protection circuit, it's fine.

* 3.6V lithium battery. Any type is fine (Old cell phone battery, etc).

* 3 rechargeable cells (AAA to D cells) in series. Important: these must be rechargeable!

* 4V lead-acid battery, connected to the battery terminals. Optionally, add a 47 ohm resistor between solar in+ and battery in+. (Note that this also applies to a 6V lead-acid battery that has a dead cell: the node uses little continuous power, so this is safe to do! Good for emergencies.)

* A supercapacitor (if you want to show off) A 5V supercap will work just fine as if it was a battery. Warning: A 3.3V supercap will NOT! If you have one of those, connect it between 3V3 and GND.

* If all you have available is a 3.3V LiFePo4 cell, you can still use it if you connect a capacitor to the battery terminals, and connect the battery itself to the 3V3 and GND terminals on the Arduino. Connect a 47 ohm resistor between B+ on the battery charger board and 3V3. This one is at your own risk and the battery will never get a full charge, but it will work in an emergency.

Connect the battery to the B+ and B- terminals on the node. RESPECT POLARITIES. The node does NOT have reverse polarity protection in its current incarnation. If a battery has other connectors other than + or - you should be able to just ignore them (this is common with cell phone batteries).