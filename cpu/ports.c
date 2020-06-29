#include "ports.h"

/**
  * Read a byte from the specified port.
  */
uint8_t port_byte_in(uint16_t port) {
    uint8_t result;
    /* Inline assembler syntax
     * !! NOTICE how the source and destination registers are swapped from nasm !!
     *
     * '"=a" (result)' ; set the C variable '(result)' to the value of register e'a'x
     * '"d" (port)' ; map the C variable '(port)' into 'e'd'x register
     *
     * Inputs and outputs are separated by colons
     */
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

/**
  * Set the specified port to a specific byte.
  */
void port_byte_out(uint16_t port, uint8_t data) {
    /* Notice how both registers are mapped to C variables and
     * nothing is returned, thus no "=" in the asm syntax.
     * However, we see a comma since there are two variables in
     * the input area and none in the 'return' area.
     */
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t port_word_in(uint16_t port) {
    uint16_t result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out(uint16_t port, uint16_t data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
