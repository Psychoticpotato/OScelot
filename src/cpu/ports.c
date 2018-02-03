#include "ports.h"

uint8_t port_byte_in(uint16_t port){
  uint8_t result;
  /*  Inline assemblyer syntax
   *  !!The source and destination registers are switched from NASM
   */
  __asm__ ("in %%dx, %%al" : "=a" (result) : "d" (port));
  return result;
}

void port_byte_out(uint16_t port, uint8_t data){
  /*  Both registers are mapped to C variables and
   *  nothing is returned, no equals "=" in asm syntax.
   *  We see a comma, as there are two variables in the 
   *  input area and none in the 'return' area.
   */
  __asm__ ("out %%al, %%dx" : :"a" (data), "d" (port));
}

uint16_t port_word_in(uint16_t port){
    uint16_t result;
  __asm__ ("in %%dx, %%ax" : "=a" (result) : "d" (port));
  return result;
}

void port_word_out(uint16_t port, uint16_t data){
  __asm__ ("out %%ax, %%dx" : : "a" (data), "d" (port));
}