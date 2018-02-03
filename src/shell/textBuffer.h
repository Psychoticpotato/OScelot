#ifndef textBuffer_H
#define textBuffer_H
#include "../libc/types.h"
#include "../libc/string.h"
#include "../drivers/screen.h"

/**
 * textBuffer struct; holds text info
 */
typedef struct {
  char string[256];  //String to modify
  uint32_t index;   //Current index
} textBuffer;

//Public Methods
void buffer_write(textBuffer *buffer, char value);
void buffer_backspace(textBuffer *buffer);
void buffer_delete(textBuffer *buffer);
void buffer_adjustIndex(textBuffer *buffer, int value);


#endif