#include "keyboard.h"
#include "screen.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "../kernel/kernel.h"
#include "../libc/string.h"
#include "../libc/function.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C

static char key_buffer[256];

#define SC_MAX 57

const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6",
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

static void keyboard_callback(registers_t *r) {
    // PIC leaves us the scancode in port 0x60
    uint8_t scancode = port_byte_in(0x60);

    if (scancode > SC_MAX) {
        return;
    }

    if (scancode == BACKSPACE) {
        backspace(key_buffer);
        kprint_backspace();
    } else if (scancode == ENTER) {
        kprint("\n");
        user_input(key_buffer); // Kernel controlled function
        key_buffer[0] = '\0';
    } else {
        char letter = sc_ascii[(int32_t)scancode];
        append(key_buffer, letter);

        // remember that kprint only accepts 'char[]'
        char str[2] = { letter, '\0' };
        kprint(str);
    }

    UNUSED(r);
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}

/* uint8_t scancode_to_ascii(uint8_t scancode) { */
/*     switch (scancode) { */
/*         case 0x0: */
/*             return 0; */
/*         case 0x0: */
/*              */
/*     } */
/* } */
