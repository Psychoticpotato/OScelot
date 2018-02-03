#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../libc/types.h"
#include "../libc/function.h"
#include "../shell/textBuffer.h"

void init_keyboard();
void setHexInputState(bool state);

#endif