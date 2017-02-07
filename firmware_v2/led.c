#include <string.h>
#include "Light_WS2812/light_ws2812.h"

#include "millis.h"

#define NUMLEDS 7

static struct cRGB led_prev [NUMLEDS];
static struct cRGB led_next [NUMLEDS];

struct cRGB selftest_seq[] = {
    { .r = 16, .g = 0,  .b = 0  },
    { .r = 0,  .g = 16, .b = 0  },
    { .r = 0,  .g = 0,  .b = 16 },
    { .r = 0,  .g = 0,  .b = 0  },
};
static const uint32_t selftest_update_millis = 200;
static uint32_t selftest_last_update;
static int selftest_enabled = 0;
static int selftest_index;

static int led_sync_needed (void)
{
    int i;
    for (i = 0; i < NUMLEDS; i++)
        if (led_prev[i].r != led_next[i].r || led_prev[i].g != led_next[i].g
                                           || led_prev[i].b != led_next[i].b)
            return 1;
    return 0;
}

static void led_sync (void)
{
    ws2812_setleds (led_next, NUMLEDS);
    memcpy (led_prev, led_next, sizeof (led_prev));
}

void led_set (int i, int r, int g, int b)
{
    if (i >= 0 && i < NUMLEDS) {
        led_next[i].r = r;
        led_next[i].g = g;
        led_next[i].b = b;
    }
}

void led_get (int i, int *r, int *g, int *b)
{
    if (i >= 0 && i < NUMLEDS) {
        *r = led_prev[i].r;
        *g = led_prev[i].g;
        *b = led_prev[i].b;
    }
}

static void selftest_next (void)
{
    int i;
    for (i = 0; i < NUMLEDS; i++) {
        led_set (i, selftest_seq[selftest_index].r,
                    selftest_seq[selftest_index].g,
                    selftest_seq[selftest_index].b);
    }
    if (++selftest_index >= sizeof (selftest_seq) / sizeof (selftest_seq[0]))
        selftest_enabled = 0;
}

static void selftest_update (void)
{
    if (millis_since (selftest_last_update) > selftest_update_millis) {
        selftest_next ();
        selftest_last_update = millis ();
    }
}

void led_selftest (void)
{
    selftest_enabled = 1;
    selftest_index = 0;
    selftest_next ();
    selftest_last_update = millis ();
}

int led_count (void)
{
    return NUMLEDS;
}

void led_update (void)
{
    if (selftest_enabled)
        selftest_update ();
    if (led_sync_needed ())
        led_sync ();
}

void led_init (void)
{
    memset (led_prev, 0, sizeof (led_prev));
    memset (led_next, 0, sizeof (led_next));
    led_selftest ();
}

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
