#include "kernel.h"
#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../libc/function.h"
#include "../libc/mem.h"
#include "../libc/string.h"

void kmain() {
    isr_install();
    irq_install();

    clear_screen();
    kprintln("Initializing keyboard...");

    kprintln("Type something, it will go through the kernel");
    kprintln("Type END to halt the CPU");
    kprint(">");
}

void user_input(char* input) {
    char* end = "END";
    char* page = "ALLOC-PAGE";
    *page = 'P';

    if (strcmp(input, page) == 0) {
        // Test kmalloc()
        uint32_t phys_addr;
        uint32_t page = kmalloc(1000, 1, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprintln(phys_str);
    }

    if (strcmp(input, end) == 0) {
        kprintln("Stopping the CPU. Bye!");
        __asm__ __volatile__("hlt");
    }

    kprint("You said: ");
    kprintln(input);
    kprint(">");
}
