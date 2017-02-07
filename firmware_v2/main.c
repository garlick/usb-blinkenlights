/* Inspiration (and some code) taken from the following open source projects:
 * - Adafruit trinket keyboard
 * - BlinkStick firmware
 * - V-USB Powerswitch demo project
 */

#include <avr/power.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "usbconfig.h"
#include "usbdrv/usbdrv.h"

#include "millis.h"
#include "led.h"

void usbBegin (void)
{
    cli();

    // run at full speed, because Trinket defaults to 8MHz for
    // low voltage compatibility reasons
    clock_prescale_set(clock_div_1);

    // fake a disconnect to force the computer to re-enumerate
    PORTB &= ~(_BV(USB_CFG_DMINUS_BIT) | _BV(USB_CFG_DPLUS_BIT));
    usbDeviceDisconnect();
    _delay_ms(250);
    usbDeviceConnect();

    // start the USB driver
    usbInit();
    sei();
}

usbMsgLen_t usbFunctionSetup (uint8_t data[8])
{
    usbRequest_t *rq = (void *)data;
    static uchar    replyBuf[2];
    int i, len;

    usbMsgPtr = (unsigned short) replyBuf;
    switch (rq->bRequest) {
        case 0: // re-execute self-test
            led_selftest ();
            break;
        case 1: // clear all LEDs
            len = led_count (); 
            for (i = 0; i < len; i++)
                led_set (i, 0, 0, 0);
            break;
        case 2: // address single LED
            i = rq->wIndex.bytes[1];
            led_set (i, rq->wIndex.bytes[0],  // r
                        rq->wValue.bytes[1],  // g
                        rq->wValue.bytes[0]); // b
            break;
    }
    return 0;
}

int main (int argc, char **argv)
{
    wdt_disable ();

    usbBegin();
    millis_init ();
    led_init ();

    wdt_enable (WDTO_1S);
    while(1) {
        wdt_reset ();
        usbPoll();
        led_update ();
    }

    return 0;
}

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
