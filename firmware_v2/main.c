/* Inspiration (and some code) taken from the following open source projects:
 * - Adafruit trinket keyboard
 * - BlinkStick firmware
 * - V-USB Powerswitch demo project
 */

#include <avr/power.h>
#include <util/delay.h>

#include "usbconfig.h"
#include "usbdrv/usbdrv.h"

#include "Light_WS2812/light_ws2812.h"

#define NUMLEDS 7

static struct cRGB led[NUMLEDS];

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

    usbMsgPtr = (unsigned short) replyBuf;
    switch (rq->bRequest) {
        case 0: // re-execute self-test
            break;
    }
    return 0;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
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
    ws2812_setleds(led,NUMLEDS);
    _delay_ms(100);
}

/* Dislpay test pattern on LEDs.
 */
void test_leds (void)
{
    const int t = 200;
    const int b = 10;
    int i;

    clear_leds ();
    for (i = 0; i < NUMLEDS; i++) {
        led[i].r=b;
        ws2812_setleds(led,NUMLEDS);
        _delay_ms(t);
    }
    clear_leds ();
    for (i = 0; i < NUMLEDS; i++) {
        led[i].g=b;
        ws2812_setleds(led,NUMLEDS);
        _delay_ms(t);
    }
    clear_leds ();
    for (i = 0; i < NUMLEDS; i++) {
        led[i].b=b;
        ws2812_setleds(led,NUMLEDS);
        _delay_ms(t);
    }
    clear_leds ();
}

int main (int argc, char **argv)
{ 
    clock_prescale_set(clock_div_1);
    test_leds ();

    usbBegin();

    while(1) {
        usbPoll();
    }

    return 0;
}

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
