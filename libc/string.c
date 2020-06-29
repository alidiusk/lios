#include "string.h"

void int_to_ascii(int32_t n, char str[]) {
    int32_t sign;
    if ((sign = n) < 0) {
        n = -n;
    }

    uint32_t i = 0;
    do {
        // get number and convert to ascii by adding to '0' ascii character
        // in one step.
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) {
        str[i++] = '-';
    }
    str[i] = '\0';

    reverse(str);
}

void hex_to_ascii(int32_t n, char str[]) {
    append(str, '0');
    append(str, 'x');
    char zeroes = 0;

    int32_t tmp, i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;

        if (tmp == 0 && zeroes == 0) {
            continue;
        }

        zeroes = 1;
        if (tmp > 0xA) {
            append(str, tmp - 0xA + 'a');
        } else {
            append(str, tmp + '0');
        }
    }

    tmp = n & 0xF;
    if (tmp > 0xA) {
        append(str, tmp - 0xA + 'a');
    } else {
        append(str, tmp + '0');
    }
}

int32_t strlen(char s[]) {
    int i = 0;

    while (*(s + i) != '\0') {
        ++i;
    }

    return i;
}

void reverse(char s[]) {
    char c;
    int32_t i, j;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void backspace(char str[]) {
    int32_t length = strlen(str);

    *(str + length - 1) = '\0';
}

void append(char str[], char n) {
    int32_t length = strlen(str);

    *(str + length) = n;
    *(str + length + 1) = '\0';
}

// Returns <0 if s1<s2, 0 if s1==s2, >0 if s1>s2
int strcmp(char s1[], char s2[]) {
    int32_t i = 0;

    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') {
            return 0;
        }
    }

    return s1[i] - s2[i];
}
