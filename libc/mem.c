#include "mem.h"

void memcpy(uint8_t* dest, uint8_t* src, size_t count) {
    for (uint32_t i = 0; i < count; i++) {
        *(dest + i) = *(src + i);
    }
}

void memset(uint8_t* src, uint8_t c, size_t count) {
    for (uint32_t i = 0; i < count; i++) {
        *(src + i) = c;
    }
}

// This should be computed at link time, but a hardcoded value
// is fine for now. Remember that our kernel starts at 0x1000
// as defined in the Makefile.
uint32_t free_mem_addr = 0x10000;

// Implementation is just a pointer to some free memory which
// keeps growing.
uint32_t kmalloc(size_t size, int32_t align, uint32_t* phys_addr) {
    // Pages are aligned to 4096, or 0x1000
    if (align == 1 && (free_mem_addr & 0xFFFFF000)) {
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;
    }

    // Save also the physical address
    if (phys_addr) {
        *phys_addr = free_mem_addr;
    }

    uint32_t ret = free_mem_addr;
    free_mem_addr += size;

    return ret;
}
