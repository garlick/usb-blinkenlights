/* borrowed some functions directly from OBDEV powerswitch.c demo project */

#include <stdio.h>
#include <usb.h>
#include <string.h>

#define USBDEV_SHARED_VENDOR    0x16C0  /* VOTI */
#define USBDEV_SHARED_PRODUCT   0x05DC  /* Obdev's free shared PID */

static int  usbGetStringAscii(usb_dev_handle *dev, int index, int langid, char *buf, int buflen)
{
char    buffer[256];
int     rval, i;

    if((rval = usb_control_msg(dev, USB_ENDPOINT_IN, USB_REQ_GET_DESCRIPTOR, (USB_DT_STRING << 8) + index, langid, buffer, sizeof(buffer), 1000)) < 0)
        return rval;
    if(buffer[1] != USB_DT_STRING)
        return 0;
    if((unsigned char)buffer[0] < rval)
        rval = (unsigned char)buffer[0];
    rval /= 2;
    /* lossy conversion to ISO Latin1 */
    for(i=1;i<rval;i++){
        if(i > buflen)  /* destination buffer overflow */
            break;
        buf[i-1] = buffer[2 * i];
        if(buffer[2 * i + 1] != 0)  /* outside of ISO Latin1 range */
            buf[i-1] = '?';
    }
    buf[i-1] = 0;
    return i-1;
}

#define USB_ERROR_NOTFOUND  1
#define USB_ERROR_ACCESS    2
#define USB_ERROR_IO    3

static int usbOpenDevice(usb_dev_handle **device, int vendor, char *vendorName, int product, char *productName)
{
    struct usb_bus      *bus;
    struct usb_device   *dev;
    usb_dev_handle      *handle = NULL;
    int         errorCode = USB_ERROR_NOTFOUND;
    static int      didUsbInit = 0;

    if (!didUsbInit){
        didUsbInit = 1;
        usb_init();
    }
    usb_find_busses ();
    usb_find_devices ();
    for (bus=usb_get_busses(); bus; bus=bus->next) {
        for (dev=bus->devices; dev; dev=dev->next) {
            if (dev->descriptor.idVendor == vendor
                                && dev->descriptor.idProduct == product) {
                char    string[256];
                int     len;
                handle = usb_open (dev); /* we need to open the device in order to query strings */
                if (!handle) {
                    errorCode = USB_ERROR_ACCESS;
                    fprintf (stderr, "Warning: cannot open USB device: %s\n",
                            usb_strerror());
                    continue;
                }
                if (vendorName == NULL && productName == NULL) { /* name does not matter */
                    break;
                }
                /* now check whether the names match: */
                len = usbGetStringAscii (handle, dev->descriptor.iManufacturer,
                                         0x0409, string, sizeof(string));
                if(len < 0) {
                    errorCode = USB_ERROR_IO;
                    fprintf(stderr, "Warning: cannot query manufacturer for device: %s\n", usb_strerror());
                } else {
                    errorCode = USB_ERROR_NOTFOUND;
                    /* fprintf(stderr, "seen device from vendor ->%s<-\n", string); */
                    if (strcmp(string, vendorName) == 0) {
                        len = usbGetStringAscii(handle, dev->descriptor.iProduct, 0x0409, string, sizeof(string));
                        if (len < 0){
                            errorCode = USB_ERROR_IO;
                            fprintf(stderr, "Warning: cannot query product for device: %s\n", usb_strerror());
                        } else {
                            errorCode = USB_ERROR_NOTFOUND;
                            /* fprintf(stderr, "seen product ->%s<-\n", string); */
                            if(strcmp(string, productName) == 0)
                                break;
                        }
                    }
                }
                usb_close(handle);
                handle = NULL;
            }
        }
        if(handle)
            break;
    }
    if(handle != NULL){
        errorCode = 0;
       *device = handle;
    }
    return errorCode;
}

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
                                USBDEV_SHARED_PRODUCT, "Blinkenlights") != 0) {
        fprintf (stderr, "Could not find \"Blinkenlights\" device\n");
        exit (1);
    }

    if (!strcmp (argv[1], "selftest")) {
        nBytes = usb_control_msg (handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE |
                                          USB_ENDPOINT_IN,
                                          0, 0, 0,
                                          (char *)buffer, sizeof (buffer),
                                          5000);
        if (nBytes < 0) {
            fprintf (stderr, "USB error: %s\n", usb_strerror ());
            exit (1);
        }
    } else if (!strcmp (argv[1], "clear")) {
        nBytes = usb_control_msg (handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE |
                                          USB_ENDPOINT_IN,
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
                                          USB_ENDPOINT_IN,
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
