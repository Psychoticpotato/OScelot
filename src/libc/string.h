#ifndef STRING_H
#define STRING_H

#include "../libc/types.h"
/*
Declare Pulblic Methods
 */

void int_to_ascii(int n, char str[]);
void hex_to_ascii(int n, char str[]);
void reverse(char s[]);
int  strlen(char s[]);
void backspace(char s[]);
void write(char n, int pos, char s[]);
void append(char s[], char n);
void prepend(char str[], char chr);
void concat(char first[], char second[]);
int strcmp(char s1[], char s2[]);
bool strEqual(char s1[], char s2[]);
void strDelete(char string[], int index);

#endif