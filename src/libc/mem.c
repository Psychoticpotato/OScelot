#include "mem.h"

void memory_copy(char* source, char* dest, int no_bytes){
  int i;
  for(i = 0; i < no_bytes; i++){
    *(dest + i) = *(source + i);
  }
}

void memory_set(uint8_t *dest, uint8_t val, uint32_t len) {
  uint8_t *temp = (uint8_t *)dest;
  for( ; len != 0; len--) *temp++ = val;
}
/* This should be computed at link time, but a hardcoded 
 * value is okay for now.  TODO: WORK ON THAT.
 * 0x1000 is where our kernel starts.
 */

uint32_t free_mem_addr = 0x10000;
//Simply a pointer to free memory that keeps growing

uint32_t kmalloc(size_t size, int align, uint32_t *phys_addr) {
  //Pages are aligned to 4k (0x1000)
  if(align ==1 && (free_mem_addr & 0xFFFFF000)){
    free_mem_addr &= 0xFFFFF000;
    free_mem_addr += 0x1000;
  }
  //Save the physical address
  if(phys_addr) * phys_addr = free_mem_addr;
  
  uint32_t ret = free_mem_addr;
  free_mem_addr += size;  //Always increment pointer
  return ret;
}