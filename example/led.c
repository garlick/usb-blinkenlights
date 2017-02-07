/* borrowed some functions directly from OBDEV powerswitch.c demo project */

#include <stdio.h>
#include <usb.h>
#include <string.h>
#include "libs-host/opendevice.h"

#define USBDEV_SHARED_VENDOR    0x16C0  /* VOTI */
#define USBDEV_SHARED_PRODUCT   0x05DC  /* Obdev's free shared PID */

void usage (void)
{
    fprintf (stderr, 
"Usage: led selftest\n"
"           clear\n"
"           set [0-6]=r:g:b (r, g, b are intensity values 0-255\n");
}

int main (int argc, char **argv)
{
    int nBytes;
    usb_dev_handle *handle = NULL;
    unsigned char buffer[8];

    if (argc < 2) {
        usage ();
        exit (1);
    }

    usb_init ();

    if (usbOpenDevice (&handle, USBDEV_SHARED_VENDOR, "WorlickWerx",
                                USBDEV_SHARED_PRODUCT, "Blinkenlights",
                                NULL, NULL, NULL) != 0) {
        fprintf (stderr, "Could not find \"Blinkenlights\" device\n");
        exit (1);
    }

    if (!strcmp (argv[1], "selftest")) {
        nBytes = usb_control_msg (handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE |
                                          USB_ENDPOINT_OUT,
                                          0, 0, 0,
                                          (char *)buffer, sizeof (buffer),
                                          5000);
        if (nBytes < 0) {
            fprintf (stderr, "USB error: %s\n", usb_strerror ());
            exit (1);
        }
    } else if (!strcmp (argv[1], "clear")) {
        nBytes = usb_control_msg (handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE |
                                          USB_ENDPOINT_OUT,
                                          1, 0, 0,
                                          (char *)buffer, sizeof (buffer),
                                          5000);
        if (nBytes < 0) {
            fprintf (stderr, "USB error: %s\n", usb_strerror ());
            exit (1);
        }
    } else if (!strcmp (argv[1], "set")) {
        int index, r, g, b, value;
        if (argc != 3) {
            usage ();
            exit (1);
        }
        if (sscanf (argv[2], "%d=%d:%d:%d", &index, &r, &g, &b) != 4) {
            usage ();
            exit (1);
        }
        if (index < 0 || index > 6
                || r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b> 255) {
            usage ();
            exit (1);
        }
        index = index << 8 | r;
        value = g << 8 | b;
        nBytes = usb_control_msg (handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE |
                                          USB_ENDPOINT_OUT,
                                          2, value, index,
                                          (char *)buffer, sizeof (buffer),
                                          5000);
        if (nBytes < 0) {
            fprintf (stderr, "USB error: %s\n", usb_strerror ());
            exit (1);
        }
    } else {
        usage ();
        exit (1);
    }

    usb_close (handle);

    return 0;
}

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
