#include <stdarg.h>
#include <stdio.h>
#include "libc_name.h"

void printInt(long num) {
    // Handle negative numbers
    if (num < 0) {
        LIBC_SYMBOL(putchar)('-');
        num = -num;
    }

    // Handle zero separately
    if (num == 0) {
        LIBC_SYMBOL(putchar)('0');
        return;
    }

    long integralPart = (long) num;
    long divisor = 1;
    while (divisor <= integralPart / 10) {
        divisor *= 10;
    }
    while (divisor > 0) {
        int digit = integralPart / divisor;
        LIBC_SYMBOL(putchar)(digit + '0');
        integralPart %= divisor;
        divisor /= 10;
    }
}

void printHex(uint32_t value) {
    const int numDigits = sizeof(uint32_t) * 2;

    //Loop through digits in reverse
    for (int i = (numDigits - 1) * 4; i >= 0; i -= 4) {
        // Get the current hex digit value
        uint32_t hex_digit = (value >> i) & 0xF;

        // Convert the hex digit to its ASCII representation
        uint32_t hex_char = hex_digit < 10 ? '0' + hex_digit : 'A' + (hex_digit - 10);

        // Print the hex digit
        LIBC_SYMBOL(putchar)((int) hex_char);
    }
}

void printDouble(double num, int minDigitsBeforeDecimal, int minDigitsAfterDecimal) {
    // Handle negative numbers
    if (num < 0) {
        LIBC_SYMBOL(putchar)('-');
        num = -num;
    }

    // Handle zero separately
    if (num == 0) {
        LIBC_SYMBOL(putchar)('0');
        return;
    }

    // Calculate the number of digits before the decimal point
    int numDigitsBeforeDecimal = 0;
    long temp = (long) num;  // Typecast to integer for counting digits
    while (temp > 0) {
        temp /= 10;
        numDigitsBeforeDecimal++;
    }

    long integral = (long) num;
    if (integral < 0) integral *= -1;

    // Print leading zeroes
    if (numDigitsBeforeDecimal < minDigitsBeforeDecimal) {
        int leadingZeros = minDigitsBeforeDecimal - numDigitsBeforeDecimal;
        while (leadingZeros > 0) {
            LIBC_SYMBOL(putchar)('0');
            leadingZeros--;
        }
    }

    //print the digits before the decimal
    printInt(integral);

    LIBC_SYMBOL(putchar)('.');

    // Print digits after the decimal point
    int numDigitsAfterDecimal = minDigitsAfterDecimal;
    while (numDigitsAfterDecimal > 0) {
        num *= 10;
        char c = ((long) num % 10) + '0';
        if (c < '0') c = '0'; //hack for bug due to overflow
        LIBC_SYMBOL(putchar)(c);
        numDigitsAfterDecimal--;
    }
}

int LIBC_SYMBOL(printf)(const char* __restrict s, ...) {
    va_list parameters;
    va_start(parameters, s);

    int isParsingFormat = 0;
    int isEscaped = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        char c = s[i];
        if (!isParsingFormat && !isEscaped && c == '%') {
            isParsingFormat = 1;
            continue;
        } else if (!isEscaped && c == '\\') {
            isEscaped = 1;
            continue;
        }

        if (isParsingFormat) {
            if (c == '%') {
                LIBC_SYMBOL(putchar)('%');
                isParsingFormat = 0;
            } else if (c == 'd') {
                int d = va_arg(parameters, int);
                printInt(d);
                isParsingFormat = 0;
            } else if (c == 'f') {
                double d = va_arg(parameters, double);
                printDouble(d, 1, 6);
                isParsingFormat = 0;
            } else if (c == 's') {
                char* string = va_arg(parameters, char*);
                if (string) {
                    LIBC_SYMBOL(puts)(string);
                } else {
                    LIBC_SYMBOL(puts)("(null)");
                }
                isParsingFormat = 0;
            } else if (c == 'c') {
                char character = va_arg(parameters, int);
                LIBC_SYMBOL(putchar)(character);
                isParsingFormat = 0;
            } else if (c == 'x') {
                uint32_t value = va_arg(parameters, uint32_t);
                printHex(value);
                isParsingFormat = 0;
            }
        } else if (isEscaped) {
            if (c == '\\') {
                LIBC_SYMBOL(putchar)('\\');
            } else if (c == 'n') {
                LIBC_SYMBOL(putchar)('\n');
            }
            isEscaped = 0;
        } else {
            LIBC_SYMBOL(putchar)(c);
        }
    }

    return 0;
}
