#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#include <inttypes.h>

#include "millis.h"

static volatile uint32_t count;

ISR (TIMER0_COMPA_vect)
{
    count++;
}

void millis_init (void)
{
    TCCR0A = (1 << WGM01);             // CTC mode
    TCCR0B = (1 << CS02);              // prescaler 256
    OCR0A = F_CPU/256 * 0.001 - 1;     // 1ms compare value
    TIMSK |= (1<<OCIE0A);              // enable interrupt on compare match
}

uint32_t millis (void)
{
    uint32_t ret;

    ATOMIC_BLOCK (ATOMIC_FORCEON)
    {
        ret = count;
    }
    return ret;
}

uint32_t millis_since (uint32_t t)
{
    uint32_t now = millis ();
    return (now < t ? (0xffffffff - t + now)
                    : (now - t));
}

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
