#include "isr.h"
#include "ports.h"
#include "timer.h"
#include "../libc/function.h"

uint32_t tick = 0;

static void timer_callback(registers_t *r) {
    tick++;

    UNUSED(r);
}

void init_timer(uint32_t freq) {
    // Install the function we just wrote
    register_interrupt_handler(IRQ0, timer_callback);

    // Get the PIT value: hardware clock at 1193180 Hz
    uint32_t divisor = 1193180 / freq;
    uint8_t low = (uint8_t)(divisor & 0xff);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xff);

    // Send the command
    port_byte_out(0x43, 0x36); // Command port
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}
