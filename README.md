## usb-blinkenlights

Blinking Light Unit (BLU) for front of a Raspberry Pi that plugs into the
upper right USB port and covers the whole USB area.  This is visual bling
for a Pi cluster.  Cluster system software or user code displays some
pattern on each Pi, so that collectively the displays give insight on
system status or parallel program progress.

First prototype used 5x7 monochrome LED matrix and a teensy 3.2
(overkill for sure).  This worked but the hardware has now been retired,
and a second prototype based on ATTiny85 and 7 WS-2812 addressable
LED's is in progress.
