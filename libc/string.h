#ifndef STRING_H
#define STRING_H

#include "../cpu/types.h"

void int_to_ascii(int32_t n, char str[]);
void hex_to_ascii(int32_t n, char str[]);
int32_t strlen(char str[]);
void reverse(char str[]);
void backspace(char str[]);
void append(char str[], char n);
int strcmp(char s1[], char s2[]);

#endif
