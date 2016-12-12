## usb-blinkenlights

Blinking Light Unit (BLU) for front of a Raspberry Pi that plugs into the
upper right USB port and covers the whole USB area.  This is visual bling
for a Pi cluster.  The idea is that cluster system software or user code
displays some pattern on each Pi, so that collectively the displays give
insight on system status or parallel program progress.

This first prototype uses the
[teensy 3.2](https://www.pjrc.com/store/teensy32.html)
and an [Avago HDSP-703E](https://www.broadcom.com/products/leds-and-displays/dot-matrix-leds-and-displays/hdsp-703e)
5x7 LED matrix from driven by a
[MAX 7219](https://www.maximintegrated.com/en/products/power/display-power-control/MAX7219.html).  The project is powered by USB and is updated using raw
USB HID protocol.  The teensy is sandwiched between a board with the
display, and a board with the USB connector and display driver.

The goal of the next version will be to get it all on one board and
reduce cost, replacing the teensy with something like the [ATtiny85](http://www.atmel.com/devices/attiny85.aspx) with [V-USB](https://www.obdev.at/products/vusb/index.html).

![Image of v1 prototype](https://github.com/garlick/usb-blinkenlights/blob/master/doc/v1.png)
