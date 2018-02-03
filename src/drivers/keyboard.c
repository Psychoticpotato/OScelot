#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
//#include "screen.h"
#include "../libc/string.h"
#include "../kernel/kernel.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define ESCAPE 0x1

//static char key_buffer[256];
bool capsOn = false;
bool shiftDown = false;
bool printHex = false;

textBuffer *buffer;

//Let us get our array of characters

#define SC_MAX 57
#define RIGHT_SEC_START 0x46
#define RIGHT_SEC_FINAL 0x52

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

/* DECLARE METHODS */
static void keyboard_callback(registers_t *regs);
void print_letter(uint8_t scancode);
void handle_Scancode(uint8_t scancode);
void printKeyHex(uint8_t scancode);

static void keyboard_callback(registers_t *regs) {
  //The PIC leaves us the scancode in port 0x60
  uint8_t scancode = port_byte_in(0x60);
  //Here we have the "print hex" function
  if(printHex){
    printKeyHex(scancode);
    return;
  }

  //if(scancode > SC_MAX) return;
  //Load up the scancode code
  handle_Scancode(scancode);
  
  UNUSED(regs);
}

/**
 * Prints the given scancode in hexidecimal
 * @param scancode scancode to print
 */
void printKeyHex(uint8_t scancode){
  //First, if we have the escape key, we cancel this mode

  if(scancode == ESCAPE){
    printHex = false;
    kprint("\n");
    kprint("Ending Hex Print");
    kprint("\n");
    return;
  }
  //We need to throw out 0xe0 (for this)
  if(scancode == 0xe0 | scancode > 0x80){
    return;
  }
  //ONWARD!
  
  char str[16] = "";
  hex_to_ascii(scancode, str);
  kprint(str);
  kprint("\n");
}

void handle_Scancode(uint8_t scancode){

  switch(scancode){
    
    case BACKSPACE:
      buffer_backspace(buffer);
      break;
    case ENTER:
      kprint("\n");
      user_input(buffer->string); //Kernel-controlled function
      buffer->string[0] = '\0';
      buffer->index = 0;
      break;
    case 0x4b:    //Arrow left
      buffer_adjustIndex(buffer, -1);
      break;
    case 0x4d:    //Arrow right
      buffer_adjustIndex(buffer, 1);
      break;
    case 0x48:    //Arrow up
      
      break;
    case 0x50:    //Arrow down
      
      break;
    case 0x53:
      buffer_delete(buffer);
      break;
    default:
      if(scancode == 0xe0 | scancode > 0x80){
        break;
      }

      char letter = sc_ascii[(int)scancode];
      buffer_write(buffer, letter);
  }
}

void setHexInputState(bool state){
  printHex = state;
}

void init_keyboard() {

  register_interrupt_handler(IRQ1, keyboard_callback);
  kprint("Keyboard Driver Initialized");
  kprint("\n");
  //Set up the buffer
  buffer->index = 0;
  
}

