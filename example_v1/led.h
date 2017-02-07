// return 0 on success, -1 on failure
int led_open (void);

void led_close (void);

// write 5x7 bitmap, where
// - each byte represents a column (len must be 5)
// - within each byte, bits 0:6 represent rows
// return 0 on success, -1 on failure
int led_write (const uint8_t *buf, int len);
