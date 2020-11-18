## Introduction
What if the internet and cell phone towers went down for more than a few hours - or during an emergency situation when no communication for several hours could mean lives lost?

## Project

The end goal of the project is to have a widespread network able to handle low-bandwidth traffic (text, compressed images) for a large number of users, to fill gaps when the larger Internet is unavailable.
In the event of an emergency, the CellSol network can be used as a knowledge base, as well as a rally point, giving people a tool to use to coordinate and organize even if other communication systems go
down. We intend to scale the design, with long-haul routing capabilities, so that regional networks can intercommunicate and interoperate, allowing for a wider breadth of use cases.

## Design Summary

The overall design is a [mesh network](https://en.wikipedia.org/wiki/Mesh_networking) of [LoRa](https://www.semtech.com/lora/what-is-lora) devices, called "Pylons"
that act as repeaters (extending the range of the network). Terminals (devices that users access the network with) also repeat packets, so that a network
made up entirely of end users is possible.

The two basic types of pylons are the ESP32 WiFi Pylon (a terminal device) and the Ardunio Repeater Pylon (a pure repeater, but can have bluetooth to use as a terminal).
 
Pylons with more specialized uses, such as data repositories for local emergency resources (phone numbers, shelter locations, etc.) and knowledge bases (such as a Wikipedia mirror) are also intended to be
developed in the future, to add to the overall usefulness of the network.