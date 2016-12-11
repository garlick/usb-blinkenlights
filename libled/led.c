#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "hid.h"
#include "led.h"

#define VENDOR_ID             0x16C0
#define PRODUCT_ID            0x0486
#define RAWHID_USAGE_PAGE     0xFFAB  // recommended: 0xFF00 to 0xFFFF
#define RAWHID_USAGE          0x0200  // recommended: 0x0100 to 0xFFFF

int led_open (void)
{
    if (rawhid_open (1, VENDOR_ID, PRODUCT_ID, RAWHID_USAGE_PAGE,
                                               RAWHID_USAGE) <= 0)
        return -1;
    return 0;
}

void led_close (void)
{
    rawhid_close (0);
}

int led_write (const uint8_t *buf, int len)
{
    uint8_t cpy[64];
    if (len != 5) {
        errno = EINVAL;
        return -1;
    }
    memcpy (cpy, buf, len);
    if (rawhid_send (0, cpy, 64, 100) != 64)
        return -1;
    return 0;
}
