#ifndef MEM_H
#define MEM_H

#include "../cpu/types.h"

void memcpy(uint8_t* dest, uint8_t* src, size_t count);
void memset(uint8_t* dest, uint8_t val, size_t count);

// No 'free' implemented yet.
uint32_t kmalloc(size_t size, int32_t align, uint32_t *phys_addr);

#endif
