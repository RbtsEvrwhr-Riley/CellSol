The firmware for the ESP32 WiFi Pylon will be available on our GitHub when we make the project public, along with all of our other firmware or software.

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/80x15.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.

## Captive Portal Implementation

We are building capabilities into our little wifi access points so that a user will automatically redirect to our desired homepage upon accessing the network.

There are several different ways that this can be accomplished (and we'd love to hear more). So far we have:

1. There's an HTTP redirect: Usually used when people are already using a well known URL that you don't want to change, rather than a user that is firing up their client for the first time on the network, but still deserves to be on the list until we rule it out.

2. User's can be redirected to our desired IP address at the router level. This is how most "Captive Portals" as these are known as, are implemented. A "captive portal" is what you deal with when you access any corporate network - for instance, at a hotel, the airport, or when you're somewhere like whole foods - it makes you login or give a code or do whatever so you will be allowed to access anything else on the network.For us, we're not trying to limit what people can access, since there's nothing but our stuff on this network - although that might be a useful tool in the future, if resources are limited and a certain page accessible on the LoRa network is hogging all the bandwidth. For us, we're just trying to make it easy for new users to find the only page that exists, by taking them there automatically when they fire up their browser.