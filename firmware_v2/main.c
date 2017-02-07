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

#include "Light_WS2812/light_ws2812.h"

#define NUMLEDS 7

static struct cRGB led[NUMLEDS];
static int led_selftest_run = 0;
static int led_update_needed = 0;

void test_leds (void);
void clear_leds (void);

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
    int i;

    usbMsgPtr = (unsigned short) replyBuf;
    switch (rq->bRequest) {
        case 0: // re-execute self-test
            led_selftest_run = 1;
            break;
        case 1: // clear all LEDs
            clear_leds ();
            led_update_needed = 1;
            break;
        case 2: // address single LED
            i = rq->wIndex.bytes[1];
            if (i >= 0 && i < NUMLEDS) {
                led[i].r = rq->wIndex.bytes[0];
                led[i].g = rq->wValue.bytes[1];
                led[i].b = rq->wValue.bytes[0];
                led_update_needed = 1;
            }
            break;
    }
    return 0;
}

void clear_leds (void)
{
    int i;
    for (i = 0; i < NUMLEDS; i++) {
        led[i].r=0;
        led[i].g=0;
        led[i].b=0;
    }
}

/* Dislpay test pattern on LEDs.
 */
void test_leds (void)
{
    const int t = 50;
    const int b = 10;
    int i;

    clear_leds ();
    for (i = 0; i < NUMLEDS; i++) {
        led[i].r=b;
        ws2812_setleds(led, NUMLEDS);
        _delay_ms(t);
    }
    clear_leds ();
    for (i = 0; i < NUMLEDS; i++) {
        led[i].g=b;
        ws2812_setleds(led, NUMLEDS);
        _delay_ms(t);
    }
    clear_leds ();
    for (i = 0; i < NUMLEDS; i++) {
        led[i].b=b;
        ws2812_setleds(led, NUMLEDS);
        _delay_ms(t);
    }
    clear_leds ();
    ws2812_setleds(led, NUMLEDS);
}

int main (int argc, char **argv)
{
    clock_prescale_set(clock_div_1);
    test_leds ();

    wdt_enable (WDTO_1S);
    usbBegin();

    while(1) {
        wdt_reset ();
        usbPoll();

        if (led_selftest_run) {
            test_leds ();
            led_selftest_run = 0;
        }

        if (led_update_needed) {
            ws2812_setleds (led, NUMLEDS);
            led_update_needed = 0;
        }
    }

    return 0;
}

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
