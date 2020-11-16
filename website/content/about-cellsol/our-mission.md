## Problem
What if the internet and cell phone towers went down for more than a few hours - or during an emergency situation when no communication for several hours could mean lives lost?

## Proposed Solution 
To build a LoRa-standard radio network using a combination of "pocket pylons" (repeaters that are carried in peoples' pockets or, with a larger antenna, in backpacks) and "fixed pylons" (fixed repeaters that forward the signal).
The pocket pylons (built on ESP32 microcontrollers) will have wifi access points on them for any wifi-enabled device to connect to. 
The fixed pylons are Arduino based and only have the ability to connect via Bluetooth to one device at a time, and to the network over LoRa.

These pylons will be solar powered in order to allow them to operate in remote areas without regular maintenance.

## End Goal

The end goal of the project is to have a widespread network able to handle low-bandwidth traffic (text, compressed images) for a large number of users, to fill gaps when the larger Internet is unavailable.
In the event of an emergency, the CellSol network can be used as a knowledge base, as well as a rally point, giving people a tool to use to coordinate and organize even if other communication systems go
down. We intend to scale the design, with long-haul routing capabilities, so that regional networks can intercommunicate and interoperate, allowing for an even wider range of use cases.

Pylons with more specialized uses, such as data repositories for local emergency resources (phone numbers, shelter locations, etc.) and knowledge bases (such as a Wikipedia mirror) are also intended to be
developed in the future, to add to the overall usefulness of the network.