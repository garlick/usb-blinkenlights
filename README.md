## usb-blinkenlights

Blinking Light Unit (BLU) for front of a Raspberry Pi that plugs into the
upper right USB port and covers the whole USB area.  This is visual bling
for a Pi cluster.  Cluster system software or user code displays some
pattern on each Pi, so that collectively the displays give insight on
system status or parallel program progress.

#### v2 prototype

The second prototype uses an Adafruit Trinket 5V (ATTiny85) and
WS-2812 addressable RGB LED's.  I experimented with different numbers
of LED's, finally arriving at 4.

#### v1 prototype

First prototype used a single color 5x7 monochrome LED matrix, a MAX 7219,
and a Teensy 3.2.  This was just fooling around with the form factor,
a matrix display, and interfacing to a rawhid USB target using libusb.
The Teensy is so easy to work with, but is outsized for the project.
