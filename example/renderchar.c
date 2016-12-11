#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "font5x7.h"
#include "led.h"

void write_char (char c)
{
    uint8_t buf[5];
    int col;

    for (col = 0; col < 5; col++) {
        buf[col] = Font5x7[(c - 0x20)*5 + col];
    }
    led_write (buf, 5);
}

int main (int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: renderchar <c>\n");
        exit (1);
    }
    if (led_open () < 0) {
        fprintf (stderr, "failed to open led: %m\n");
        exit (1);
    }
    write_char (argv[1][0]);
    led_close ();
    exit (0);
}
