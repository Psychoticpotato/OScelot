/*
General utility methods for the whole OS
 */
#include "string.h"
/*
Begin Methods
 */


void int_to_ascii(int n, char str[]){
  int i, sign;
  if((sign = n) < 0) n = -n;
  i = 0;
  do{
    str[i++] = n % 10 + '0';
  } while ((n /=10) > 0);
  
  if(sign < 0) str[i++] = '-';
  str[i] = '\0';
  
  
  reverse(str);
}

void hex_to_ascii(int n, char str[]) {
    
  char hexArray[] = "0123456789abcdef";
  do{
    append(str,hexArray[n % 16]);
    n = n/16;
  } while (n > 0);
  
  reverse(str);
  //Now, we add the '0x'
  prepend(str, 'x');
  prepend(str, '0');

}


//Allright, let us reverse
void reverse(char s[]){
  int c, i, j;
  for(i = 0, j = strlen(s) -1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

int strlen(char s[]){
  int i = 0;
  while(s[i] != '\0') i++;
  return i;
}

void append(char s[], char n){
  int len = strlen(s);
  s[len] = n;
  s[len+1] = '\0';
}

void prepend(char str[], char chr){
  int length = strlen(str);

  //Stretch out str
  for(int i = length; i > 0; i--){
    str[i] = str[i - 1];
  }
  //Add the character
  str[0] = chr;
}

/**
 * Concatenates the second string onto the first
 * @param first  First String (string that is modified)
 * @param second Second String that will be added to first
 */
void concat(char first[], char second[]){
  int offset = strlen(first);
  for(int i = 0; i < strlen(second); i++){
    first[offset + i] = second[i];
  }
}

void backspace(char s[]) {
  int len = strlen(s);
  s[len -1] = '\0';
}

void write(char n, int pos, char s[]){
  if(pos > strlen(s) + 1){
    append(s, n);
  } else if(pos < strlen(s)){
    s[pos] = n;
  } 
  
}

/**
 * Compares the two strings
 * @param  s1   First string to compare 
 * @param  s2   Second String to compare
 * @return      Result is based on the strings:
 *  < 0 : s1 < s2
 *  = 0 : s1 = s2
 *  > 0 : s1 > s2
 */
int strcmp(char s1[], char s2[]){
  int i;
  for(i = 0; s1[i] == s2[i]; i++){
    if(s1[i] == '\0') return 0;
  }
  return s1[i] - s2[i];
}

bool strEqual(char s1[], char s2[]){
  int result = strcmp(s1, s2);
  if(result == 0){
    return true;
  }
  return false;
}

/**
 * Deletes the value at the specified location,
 * then shifts all subsequent values left by one.
 * @param string String to change
 * @param index  Index to delete
 */
void strDelete(char string[], int index){
  int length = strlen(string);
  while(index < length){
    string[index] = string[++index];
  }
  string[index] = '\0';
}

