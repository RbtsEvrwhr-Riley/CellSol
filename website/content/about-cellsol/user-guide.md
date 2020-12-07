---
publishdate: 2019-11-17
lastmod: 2020-12-07
---

# CellSol WiFi Pylon User Guide

This is a short how-to on how to use the CellSol WiFi Pylons. It is also packaged with the pylons themselves.

## Basic Concepts

CellSol is free to use and does not depend on any infrastructure, each pylon is self-contained: just deploy a few of them in an area and you are good to go.

This is a lot like existing LoRa mesh chat systems, except that it's intended to leave repeaters in place.

It will honor other mesh network systems by repeating their packets too, as long as they start with a xxxx: tag in front.

## About the Chat System

By using the chat in the main page, you will be able to communicate with people who are in range of the CellSol network, or people who are using CellSol gateways.

Each message is sent out to neighboring pylons, which can be a few kilometers apart, and rebroadcast.

You can consider this system akin to a single IRC/Discord/Twitch chat channel, except it will work when the internet at large will not.

Please note that CellSol does not store your location or identity data, but also please note that there is no encryption for these messages (to make it easier for other systems to interoperate with). This is intended to be used during or after natural disasters, so that should really not be a concern.

## Identifying Users

The tag in front of your message is a pseudonymous identifier used to tell people apart. It is four hex digits. We chose to remove nicknames, to simplify the data. You can always add one to your message.

![CellSol Web Interface with Help Highlights](../help_graphic.png)

## About Bluetooth and Repeater Pylons

Any phone with a Bluetooth chat app (available from our [github](https://github.com/RbtsEvrwhr-Riley/CellSol/) or from any pylon) can also use Bluetooth equipped repeater pylons, or the WiFi pylons. If you are on an iPhone, you will
need to use a Bluetooth terminal app, such as "BluTerm". We do not work on or endorse any third party apps.

If you are on a Bluetooth or serial CellSol pylon, typing ,,, on a line by itself will dump the pylon's status and last received strings (in case your phone loses them). Our app does this automatically on reconnect.

## Chatting with IRC (Internet Relay Chat)

By default, your pylon will connect to IRC on irc.rizon.net port 6667, and create or join the channel CellSol.

You can also connect to this (or whatever channel you have configured, see the [assembly instructions]({{< ref /cellsol-equipment/esp32/assembly-instructions >}})
by using any IRC client.

Once you have connected to IRC and joined the channel, you should see a number of users whose names begin with CellSol and end in 4 hex digits. These
are the IRC users for the CellSol nodes currently connected to the channel. These users will relay messages from their node to the IRC channel, starting
with the 4 hex digit code of the user that sent the message, followed by the message itself, just like on the CellSol web interface.

Messages from non-pylon users on the IRC channel will not automatically be relayed by the connected CellSol nodes, in order to save bandwidth. For a message
to be forwarded onto the network by the connected nodes, it must start with that node's chosen prefix (default is ~~). It is most useful if all nodes
connecting to the same IRC channel be set up to have the same prefix, and that the topic of the IRC channel is set to report that prefix correctly.

We hope you enjoy the new IRC feature!