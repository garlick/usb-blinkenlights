#include <avr/power.h>
#include <util/delay.h>

#include "usbconfig.h"
#include "usbdrv/usbdrv.h"

#include "Light_WS2812/light_ws2812.h"

void usbBegin() {
        cli();

        // run at full speed, because Trinket defaults to 8MHz for low voltage compatibility reasons
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

usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
    usbRequest_t *rq = (void *)data;
    static uchar    replyBuf[2];

    usbMsgPtr = (unsigned short) replyBuf;
    switch (rq->bRequest) {
        case 0: // write new LED state
            return USB_NO_MSG; // defer transfer to usbFunctionWrite()
    }
    return 0;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
    return 0;
}

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny25__)
/* ------------------------------------------------------------------------- */
/* ------------------------ Oscillator Calibration ------------------------- */
/* ------------------------------------------------------------------------- */
// section copied from EasyLogger
/* Calibrate the RC oscillator to 8.25 MHz. The core clock of 16.5 MHz is
 * derived from the 66 MHz peripheral clock by dividing. Our timing reference
 * is the Start Of Frame signal (a single SE0 bit) available immediately after
 * a USB RESET. We first do a binary search for the OSCCAL value and then
 * optimize this value with a neighboorhod search.
 * This algorithm may also be used to calibrate the RC oscillator directly to
 * 12 MHz (no PLL involved, can therefore be used on almost ALL AVRs), but this
 * is wide outside the spec for the OSCCAL value and the required precision for
 * the 12 MHz clock! Use the RC oscillator calibrated to 12 MHz for
 * experimental purposes only!
 */
void calibrateOscillator(void)
{
    uchar       step = 128;
    uchar       trialValue = 0, optimumValue;
    int         x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);

    /* do a binary search: */
    do{
        OSCCAL = trialValue + step;
        x = usbMeasureFrameLength();    /* proportional to current real frequency */
        if(x < targetValue)             /* frequency still too low */
            trialValue += step;
        step >>= 1;
    }while(step > 0);
    /* We have a precision of +/- 1 for optimum OSCCAL here */
    /* now do a neighborhood search for optimum value */
    optimumValue = trialValue;
    optimumDev = x; /* this is certainly far away from optimum */
    for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++){
        x = usbMeasureFrameLength() - targetValue;
        if(x < 0)
            x = -x;
        if(x < optimumDev){
            optimumDev = x;
            optimumValue = OSCCAL;
        }
    }
    OSCCAL = optimumValue;
}
/*
Note: This calibration algorithm may try OSCCAL values of up to 192 even if
the optimum value is far below 192. It may therefore exceed the allowed clock
frequency of the CPU in low voltage designs!
You may replace this search algorithm with any other algorithm you like if
you have additional constraints such as a maximum CPU clock.
For version 5.x RC oscillators (those with a split range of 2x128 steps, e.g.
ATTiny25, ATTiny45, ATTiny85), it may be useful to search for the optimum in
both regions.
*/
#endif

#define NUMLEDS 7

struct cRGB led[NUMLEDS];

void clear_leds ()
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

void test_leds ()
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

int main()
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
