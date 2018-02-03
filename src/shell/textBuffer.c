/*
Text Buffer utilities
 */
#include "textBuffer.h"
/*
Begin Methods
 */

/**
* Writes the given character to the textBuffer
* @param buffer textBuffer to modify
* @param value  character to append on the buffer
*/
void buffer_write(textBuffer *buffer, char value){
  //If the current value is null, we need to null terminate after this.
  if(buffer->string[buffer->index] == '\0'){
    buffer->string[buffer->index + 1] = '\0';
  }
  buffer->string[buffer->index] = value;
  char str[2] = {value, '\0'};
  kprint(str);
  buffer->index++;

}

/**
 * Deletes the character at the previous index,
 * moves the index back one, and adjust the display
 * @param buffer Buffer to modify
 */
void buffer_backspace(textBuffer *buffer){

  if(buffer->index == 0){
    return;
  }
  buffer->index--;
  cursor_adjust(-1);
  buffer_delete(buffer);

}

/**
 * Deletes the character at the current index.
 * If at the end of the buffer, nothing happens
 * @param buffer [description]
 */
void buffer_delete(textBuffer *buffer){
  //First, if the index is 0 or less, we need to reset it to zero and exit
  char ind[32] = "Buffer Index: ";
  char temp[8];

  int_to_ascii(buffer->index,temp);
  concat(ind, temp);
  kStatusPrint(ind);
  if(buffer->index == strlen(buffer->string)){
    return;
  }

  int start = buffer->index;    //We will need this to adjust the cursor
  int length = strlen(buffer->string);
  char str[length - start];
  //Loop through the buffer to move each character left by one
  for(int i = start ; i < length; i++){
    buffer->string[i] = buffer->string[i + 1];
    str[i - start] = buffer->string[i];
  }
  append(str,' ');

  kprint(str);

  cursor_adjust(-strlen(str));
  return;
}

/**
 * Adjusts the index by the specified value
 * @param buffer textBuffer to modify
 * @param value  amount by which we modify the index
 */
void buffer_adjustIndex(textBuffer *buffer, int value){
  //If the end result is out of bounds, we ignore
  buffer->index += value;
  if((buffer->index < 0 ) | (buffer->index > strlen(buffer->string))){
    buffer->index -= value;
    return;
  }
  
  cursor_adjust(value);
}