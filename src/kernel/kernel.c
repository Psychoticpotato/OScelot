#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "kernel.h"
#include "../cpu/isr.h"
#include "../libc/string.h"
#include "../libc/mem.h"

void kernel_main() {

  isr_install();
  irq_install();

  kprint("Begin user input.\n");
  kprint("Type \"END\" to halt the CPU\n> ");

}

void user_input(char *input) {
  
  if(strEqual(input, "END")){
      kprint("Halting CPU.");
      asm volatile("hlt");
    } else if(strEqual(input, "PAGE")){
      //Let us test kmalloc
      uint32_t phys_addr;
      uint32_t page = kmalloc(1000, 1, &phys_addr);
      char page_str[16] = "";
      hex_to_ascii(page, page_str);
      char phys_str[16] = "";
      hex_to_ascii(phys_addr, phys_str);
      kprint("Page: ");
      kprint(page_str);
      kprint(", physical address: ");
      kprint(phys_str);
      kprint("\n");
    } else if(strEqual(input, "HEX")){
      kprint("Printing Hex Values of keys.\nPress \"ESC\" to end.");
      kprint("\n");
      setHexInputState(true);
    } else{

      kprint("User input was: ");
      kprint(input);
      kprint("\n> ");
  }
  

}