## Problem
What if the internet and cell phone towers went down for more than a few hours - or during an emergency situation when no communication for several hours could mean lives lost?

## Proposed Solution 
To build a LoRa-standard radio network using a combination of "pocket pylons" (repeaters that are carried in peoples' pockets or, with a larger antenna, in backpacks) and "fixed pylons" (fixed repeaters that forward the signal).
The pocket pylons (built on ESP32 microcontrollers) will have wifi access points on them for any wifi-enabled device to connect to. The fixed pylons are Arduino based and only have the ability to connect via Bluetooth to one device at a time, and to the network over LoRa.

These pylons will be solar powered in order to allow them to operate in remote areas without regular maintenance.

## End Goal
The end goal of the project is to have a widespread network able to handle low-bandwidth traffic (text, compressed images) for a large number of users, to act as a fail
over for the larger Internet. Our intention is to have some form of channel or routing system, replacing the bandwidth-heavy broadcast routing of our proof of concept,
so that the system is scalable to higher numbers of users.

We hope to partner with knowledge bases such as Wikipedia to preserve knowledge and distribute information resources.