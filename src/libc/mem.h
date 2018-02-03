#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <stddef.h>
/*
Declare Pulblic Methods
 */
void memory_copy(char* source, char* dest, int no_bytes);
void memory_set(uint8_t *dest, uint8_t val, uint32_t len);


uint32_t kmalloc(size_t size, int align, uint32_t *phys_addr);

#endif