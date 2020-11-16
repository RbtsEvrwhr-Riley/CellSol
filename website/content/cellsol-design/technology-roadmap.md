---
title: Technology Roadmap
---

{{< toc >}}

## Multiple Channels

CellSol currently operates on the principle that a single bulletin board is sufficient for a local network. We intend, in the short term, to implement multiple channels, to allow users to filter messages.

## Internetworking

CellSol's mesh architecture does not scale, due to bandwidth limitations, so we will be developing bridging devices that will allow CellSol networks to communicate with one another either directly, or
through the wider Internet when it is available. The former is a more resilient implementation, but range limited, whereas the latter is limited by Internet connectivity, but will allow for a centralized
point of contact across the whole CellSol network. In either case, it is likely we will use Internet Relay Chat (IRC) as a transfer protocol, due to all packets being text and it being optimized for
low bandwidth operation.

## Datalogging and Instrumentation

Humans aren't the only users that have a place on the CellSol network. Going forward into 2021, documentation for writing firmware for autonomous instruments and integrating them into the CellSol network
will be provided. Bandwidth is, of course, a serious consideration, and restricting the resource usage of dataloggers on the main mesh, or even implementing a quality-of-service (QoS) system may be
required. How individual mesh networks are used will significantly determine how they need to regulate datalogger traffic, so further discussion is definitely needed before a design is decided upon.

One way we expect to support these dataloggers early into 2021 is to expose a webservice endpoint from WiFi pylons, so that HTTP requests can be used to send and receive messages instead of just refreshing
the entire chat page as a browser does.

## Security

By using standard client software (web browsers, IRC clients) endpoint security becomes much easier to achieve, but eventually CellSol may need some bandwidth-preservation security implementations. Spam
bots, redirect bots, and other denial of service attacks are effective given CellSol's low overall throughput, and low overhead software systems to prevent them will need to be designed as adoption
increases, especially if we are to rely on the system during a social crisis such as the Black Lives Matter protests of 2020.