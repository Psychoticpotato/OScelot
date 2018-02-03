#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/string.h"
#include "../libc/mem.h"

/* Declaration of private functions */
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char character, int col, int row, char attribute_byte);
int get_screen_offset(int col, int row);
int get_cursor_row(int offset);
int get_cursor_col(int offset);
int handle_scrolling(int offset);
void kStatusClear();

/****************************************
*        Public functions below         *
*****************************************/
void kprint_at(char* message, int col, int row){
  //Update the cursor and row if not negative
  int offset;
  if(col >= 0 && row >= 0){
    offset = get_screen_offset(col, row);
  } else {
    offset = get_cursor_offset();
    row = get_cursor_row(offset);
    col = get_cursor_col(offset);
  }
  //Loop through each char and print
  int i = 0;
  while(message[i] != 0){
    offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
    //Update next row and column
    row = get_cursor_row(offset);
    col = get_cursor_col(offset);
  }
}

/**
 * Prints the specified message at the cursor location
 * @param message Message to print
 */
void kprint(char* message){
  kprint_at(message, -1, -1);
}

void kStatusPrint(char* message){
  int length = strlen(message);
  //Save the cursor so we can set it when we are done
  int offset = get_cursor_offset();
  kStatusClear();
  
  kprint_at(message, MAX_COLS - length -1, 0);
  set_cursor_offset(offset);
}

void clear_screen(){
  int row = 0;
  int col = 0;
  
  //Loop through video memory and write blank chars
  for(row  = 0; row < MAX_ROWS; row++){
    for(col = 0; col<MAX_COLS; col++){
      print_char(' ', col, row, WHITE_ON_BLACK);
    }
  }
  
  //Set the cursor to the top left
  set_cursor_offset(get_screen_offset(0,0));
}

void kprint_backspace(){
  int offset = cursor_adjust(-1);
  int row = get_cursor_row(offset);
  int col = get_cursor_col(offset);
  print_char(0x08, col, row, WHITE_ON_BLACK);
}

int cursor_adjust(int spaces){
  spaces *= 2;  //Adjust the number of spaces to adjust
  int offset = get_cursor_offset() + spaces;
  set_cursor_offset(offset);
  return offset;
}

/****************************************
*        Private functions below        *
*****************************************/

//Print a char on the screen at col, row, or 
//current curser position
int print_char(char character, int col, int row, char attribute_byte){
  //Create a byte (char) pointer to the start of video memory
  uint8_t *vidmem = (uint8_t *) VIDEO_ADDRESS;
  
  //If attribute byte is zero, assume default style
  if(!attribute_byte){
    attribute_byte = WHITE_ON_BLACK;
  }
  
  //Get the video memory offset for the screen location
  int offset;
  //If the column and row are non-negative, use them
  if(col >= 0 && row >= 0){
    offset = get_screen_offset(col, row);
  } else {
    offset = get_cursor_offset();
  }
  
  /*
      If we see a newline character, set offset to the
      end of current row, so it will be advanced to
      the first col of the next row.
   */
  if(character == '\n') {
    row = get_cursor_row(offset);
    offset = get_screen_offset(0, row+1);

  } else if(character == 0x08) {
    //Backspace
    vidmem[offset] = ' ';
    vidmem[offset+1] = attribute_byte;
  /*
  Otherwise, write the character and its atribute
  byte to the video memory at the calculated offset.
   */
  } else {
    vidmem[offset] = character;
    vidmem[offset+1] = attribute_byte;
    offset += 2;
  }
  //Make scrolling adjustment, in case the bottom is reached
  offset = handle_scrolling(offset);
  //Update the cursor position on the screen device
  set_cursor_offset(offset);  
  return offset;
}

int get_screen_offset(int col, int row){
  return 2 * (row * MAX_COLS + col);
}

int get_cursor_row(int offset){
  return offset/(2 * MAX_COLS);
}

int get_cursor_col(int offset){
  return (offset - (get_cursor_row(offset) * 2 * MAX_COLS))/2;
}

int get_cursor_offset(){
  /*
  The device uses its control register as
  an index to select its internal register,
  of which we are interested in:
      reg 14: the high byte of the cursor offset
      reg 15: the  low byte of the cursor offset
  Once the internal register has been selected,
  we may read or write a byte on the data register
   */
  port_byte_out(REG_SCREEN_CTRL, 14);
  int offset = port_byte_in(REG_SCREEN_DATA) << 8;
  port_byte_out(REG_SCREEN_CTRL, 15);
  offset += port_byte_in(REG_SCREEN_DATA);
  /*
  The cursor offset reported by the VGA hardware 
  is the number of characters; we must multipy by
  two to convert it to a character cell offset.
   */
  return offset*2;
}

void set_cursor_offset(int offset){
  //Convert from cell offset to char offset.
  //This is similar to get_cursor, only now
  //we write the bytes to the internal registers.
  offset /= 2;  
  port_byte_out(REG_SCREEN_CTRL,14);
  port_byte_out(REG_SCREEN_DATA, (uint8_t) (offset >> 8));
  port_byte_out(REG_SCREEN_CTRL,15);
  port_byte_out(REG_SCREEN_DATA, (uint8_t) (offset & 0xff));

}

/**
 * Advance the text cursor, scrolling the entire
 * video buffer, as required
 * @param  cursor_offset  new (attempted) cursor offset
 * @return                updated cursor offset
 */
int handle_scrolling(int cursor_offset){
  //First, if we are not equal to the max, no adjustment required
  if(cursor_offset < MAX_ROWS*MAX_COLS*2){
    return cursor_offset;
  }
  
  //Bring all rows up by one
  int i;
  for(i = 1; i < MAX_ROWS; i++){
    memory_copy(get_screen_offset(0,i) + VIDEO_ADDRESS,
                get_screen_offset(0,i - 1) + VIDEO_ADDRESS,
                MAX_COLS *2
              );
  }
  //Blank out the last line (by setting all bytes to 0)
  char* last_line = get_screen_offset(0,MAX_ROWS-1) + VIDEO_ADDRESS;
  for(i = 0; i < MAX_COLS*2; i++){
    last_line[i] = 0;
  }
  
  //Move the offset back one row, so that it is on the last row,
  //rather than off the edge of the screen.
  cursor_offset -= 2*MAX_COLS;
  
  //Finally, return the updated location
  return cursor_offset;
}

void kStatusClear(){
  for(int col = 0; col < MAX_COLS; col++){
      print_char(' ', col, 0, WHITE_ON_BLACK);
  }


}
