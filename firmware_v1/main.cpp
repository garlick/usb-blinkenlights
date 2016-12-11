#include <WProgram.h>
#include <Matrix.h>
#include <usb_dev.h>

Matrix leds = Matrix (16, 17, 18, 1);

void led_walk_bits (void)
{
    byte x, y;

    leds.clear();
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 5; x++) {
	    leds.write (x, y, HIGH);
	    delay (50);
	    leds.write (x, y, LOW);
        }
    }
}

void led_set_from_buf (uint8_t *buf)
{
    byte row, col;

    leds.clear ();
    for (col = 0; col < 5; col++)
        for (row = 0; row < 7; row++) {
            leds.write (col, 6-row, (buf[col]>>row & 1) ? HIGH : LOW);
    }
}


extern "C" int main (void)
{
    static uint8_t buf[64];

    usb_init ();

    // test each pixel
    led_walk_bits ();

    for (;;) {
        if (usb_rawhid_recv (buf, 0) > 0)
            led_set_from_buf (buf);
    }
}

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */

