#include "timer.h"
#include "../drivers/screen.h"
#include "ports.h"
#include "../libc/function.h"
#include "isr.h"

/*  DECLARE METHODS */
static void timer_callback(registers_t *regs);
/*   END METHODS   */

uint32_t tick = 0;

static void timer_callback(registers_t *regs) {
  tick++;
  UNUSED(regs);
  
  /*
  char tick_ascii[256];
  int_to_ascii(tick, tick_ascii);
  kprint(tick_ascii);
  kprint("\n");
  */
}

void init_timer(uint32_t freq) {
  //Install the function
  register_interrupt_handler(IRQ0, timer_callback);
  
  //get the PIT value: hardware clock at 1193180 HZ
  uint32_t divisor = 1193180 /freq;
  uint8_t low  = (uint8_t) (divisor & 0xFF);
  uint8_t high = (uint8_t) ((divisor >> 8) & 0xFF);
  //Send... The Command.
   port_byte_out(0x43, 0x36); //Command port
   port_byte_out(0x40, low);
   port_byte_out(0x40, high);
}