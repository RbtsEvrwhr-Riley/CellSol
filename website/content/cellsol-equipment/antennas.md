---
title: Antennas
publishdate: 2019-11-17
lastmod: 2020-11-24
---

{{< toc >}}

# Antenna Concepts

If you're not familiar with antenna design, [this site](http://www.antenna-theory.com/) can be a good resource. You don't need to know any antenna theory to choose an antenna for your pylon,
but it's useful knowledge!

# Small Antennas (<10cm)

900MHz wifi antennas will often serve "okay" at both 866MHz for european LoRa and 915MHz for North American, but the performance in either case will be less than ideal.

Most of our data for LoRa antennas have come from the results of Mark Zachmann on his blog here: https://medium.com/home-wireless/testing-lora-antennas-at-915mhz-6d6b41ac8f1d

We have only been the 85mm copper wire and stock LoRa32 antennas ourselves so far. Please [reach out to us](mailto:cellsol@robots-everywhere.com) if you are testing other antennas! We would love
to hear from you, and see your network implementations.

## With a Ground Plane (Monopoles)

Monopole antennas will perform better than dipoles, but need a solid connection to earth ground to work effectively. For a static pylon, such as a repeater or a home
ESP32 WiFi pylon, you may have easy access to earth ground. REMEMBER: when connecting your pylon to a grounding stake, do not use a wire that is a multiple of 85mm in length,
it will resonate on the frequency you are trying to receive, making your ground less effective.

For a short-wire monopole antenna, 85mm of copper wire will work; our tests used standard 20 gauge stranded wire from our shop spools. 

REMINDER: the wire leading to the antenna is also an antenna, ensure it is either too short or more likely too long to resonate at 915MHz. If the antenna is near 8cm in length, it will reasonate, causing
a lot of noise, possibly disabling the connection entirely.

Linx makes a 915MHz monopole as well; https://linxtechnologies.com/wp/product/pw-series-antennas/ along with other manufacturers.

### 3D Printed Antenna Stand & Cutter

Kay designed a 3D printable antenna stand that also doubles as a ruler to measure the right length of wire to use for the best reception.

Our LoRa network uses 915MHz as its frequency. Different frequencies will use different lengths of wire on our antenna ruler/stand. 
You can use the ruler to measure exactly the right length of wire for your antenna, before you cut it.

1. Print out our antennae stand/ruler for frequency wire length tool

2. Use the tool to measure the desired length of wire for your antennae, and cut it

3. Slide the wire through the antennae stand

4. Connect the antennae to your LoRa repeater or Wifi Router

Be careful where you place your repeater, as the antenna itself can shadow the solar panel and interfere with it's ability to capture power from the sun.

## Without a Ground Plane (Dipoles)

When you do not have a ground plane you should be using a dipole antenna. The stock antenna on the Heltec and TTGO LoRa32s (and many other cheap LoRa antennas) are
actually WiFi antennas - tuned at 915MHz - with a broad frequency range. The 866MHz version is the same as the North American 915MHz one, so all models work in all regions.

One example of a high performance whip dipole for LoRa at 915MHz is here. https://linxtechnologies.com/wp/wp-content/uploads/ant-916-cw-hw.pdf They also make an 868MHz version.

If you are on the side of the building, use a directional reflector (aluminum) to improve performance, though the concrete of the building will do a fairly good job. Glass won't.

From our own calculations, large loop antennas are not worth it for 915MHz applications - diminishing returns on gain begin at very small loop diamters, around 8cm. We will continue further testing as the project goes on.

For a very tiny pocket node, small chip antennas exist:

https://www.mouser.ca/ProductDetail/Yageo/ANT1204F005R0915A?qs=gt1LBUVyoHnk%2Fihas5MohQ%3D%3D

https://linxtechnologies.com/wp/product/915-usp410-lpwa-antenna/ 

https://www.youtube.com/watch?v=eQEmFlMW50o This guide has some information on making your own LoRa dipole antennas using copper welding wire.

Otherwise, the stock dipole on the LoRa32 performs better than most 900MHz antennas at 916MHz.

# Large Antennas

We don't have any big-antenna deployments yet - we're working on securing funding to build some large pylons around the San Francisco Bay Area at the moment.

Depending on the region you are operating in and the regulation of the 915MHz or 866MHz band, the operation large transmitters may not be permitted. You may be
required to reduce the power of your transmission by modifying the firmware, or use a smaller antenna.

If you want to help, [contact us](mailto:cellsol@robots-everywhere.com)