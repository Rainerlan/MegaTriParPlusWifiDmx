# Mega TriPar Profile Plus Wifi Dmx
Adds a Wifi web frontend via ESP32 to an ADJ Mega Tripar Profile Plus (through DMX)

Any ESP32 with an 0.96 inch OLED display may be used to run this code.
I used a WIFI kit 32.
An additionaal small piggyback board is needed to convert the GPIO (14) signal (TTL) to an RS485 (differential) DMX signal.

Some code has already been added to also send/receive though Artnet over Wifi - but this is not the main purpose of this project.
So this has not been tested and most probably will not be working.

Ajax is used to implement a the communication between Webbrowser and ESP32.
Defaults for a 10ch setup are compiled into the project.
Settings can be changed via the webfrontend and can be stored to the flash using littlefs.
After reboot of the ESP, the new settings will be applied right from the start.

If anyone is interested, I can add pictures and layout of the piggyback board I made.
The code used GPIO 14 to output the DMX-TTL signal.
Artnet DMX universe might be switched though GPIO 27

To make the project work, the littlefs files from the data directory have to be uploaded to the ESP.
