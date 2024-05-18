#include <stdarg.h>
#include <stdio.h>

#define BASE_SYSTEM 10
#define HEX_DIGIT_MASK 0xF

/**
 * Print a negative sign if the value pointed to by numPtr is negative
 * @param numPtr pointer to the value
 * @return 1 if printing was successful, 0 if not
 */
int printNegativeSignIfNeeded(long* numPtr) {
    if (*numPtr < 0) {
        *numPtr = -*numPtr;
        int success = putchar('-');

        if(!success) {
            return 0;
        }
    }

    return 1;
}

/**
 * Print an integer value.
 * @param num
 * @return 1 if printing was successful, 0 if not
 */
int printInt(long num) {
    // Handle negative numbers
    int success = printNegativeSignIfNeeded(&num);
    if(!success) {
        return 0;
    }

    // Handle zero separately
    if (num == 0) {
        return putchar('0');
    }

    long integralPart = (long) num;
    long divisor = 1;
    while (divisor <= integralPart / BASE_SYSTEM) {
        divisor *= BASE_SYSTEM;
    }
    while (divisor > 0) {
        int digit = integralPart / divisor;
        putchar((char)(digit + '0'));
        integralPart %= divisor;
        divisor /= BASE_SYSTEM;
    }

    return 1;
}

int printHex(uint32_t value) {
    const int numDigits = sizeof(uint32_t) * 2;

    //Loop through digits in reverse
    for (int i = (numDigits - 1) * 4; i >= 0; i -= 4) {
        // Get the current hex digit value
        uint8_t hex_digit = (value >> i) & HEX_DIGIT_MASK;

        // Convert the hex digit to its ASCII representation
        uint8_t hex_char = hex_digit < BASE_SYSTEM ? '0' + hex_digit : 'A' + (hex_digit - BASE_SYSTEM);

        // Print the hex digit
        int success = putchar((char) hex_char);

        if(!success) {
            return 0;
        }
    }

    return 1;
}

int printLeadingZeroes(int minDigitsBeforeDecimal, int numDigitsBeforeDecimal) {
    int leadingZeros = minDigitsBeforeDecimal - numDigitsBeforeDecimal;
    while (leadingZeros > 0) {
        leadingZeros--;
        int success = putchar('0');

        if(!success) {
            return 0;
        }
    }

    return 1;
}

int printDecimalPortion(double num, int minDigitsAfterDecimal) {
    // Print digits after the decimal point
    int numDigitsAfterDecimal = minDigitsAfterDecimal;
    while (numDigitsAfterDecimal > 0) {
        num *= BASE_SYSTEM;
        char c = (char)(((long) num % BASE_SYSTEM) + '0');
        if (c < '0') {
            c = '0'; //hack for bug due to overflow
        }

        int success = putchar(c);
        if(!success) {
            return 0;
        }

        numDigitsAfterDecimal--;
    }

    return 1;
}

void printAbsoluteIntegerPortion(double num, int minDigitsBeforeDecimal, int numDigitsBeforeDecimal) {
    long integral = (long) num;
    if (integral < 0) {
        integral *= -1;
    }

    printLeadingZeroes(minDigitsBeforeDecimal, numDigitsBeforeDecimal);

    //print the digits before the decimal
    printInt(integral);
}

int printDouble(double num, int minDigitsBeforeDecimal, int minDigitsAfterDecimal) {
    // Handle negative numbers
    long longNum = (long)num;

    int success = printNegativeSignIfNeeded(&longNum);

    if(!success) {
        return 0;
    }

    // Handle zero separately
    if (num == 0) {
        return putchar('0');
    }

    // Calculate the number of digits before the decimal point
    int numDigitsBeforeDecimal = 0;
    long temp = (long) num;  // Typecast to integer for counting digits
    while (temp > 0) {
        temp /= BASE_SYSTEM;
        numDigitsBeforeDecimal++;
    }

    printAbsoluteIntegerPortion(num, minDigitsBeforeDecimal, numDigitsBeforeDecimal);

    putchar('.');

    printDecimalPortion(num, minDigitsAfterDecimal);

    return 1;
}

int printf(const char* __restrict s, ...) {
    va_list parameters; //NOLINT
    va_start(parameters, s);

    int isParsingFormat = 0;
    int isEscaped = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        char c = s[i];
        if (!isParsingFormat && !isEscaped && c == '%') {
            isParsingFormat = 1;
            continue;
        }
        if (!isEscaped && c == '\\') {
            isEscaped = 1;
            continue;
        }

        if (isParsingFormat) {
            if (c == '%') {
                putchar('%');
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
                    puts(string);
                } else {
                    puts("(null)");
                }
                isParsingFormat = 0;
            } else if (c == 'c') {
                char character = va_arg(parameters, int);
                putchar(character);
                isParsingFormat = 0;
            } else if (c == 'x') {
                uint32_t value = va_arg(parameters, uint32_t);
                printHex(value);
                isParsingFormat = 0;
            }
        } else if (isEscaped) {
            if (c == '\\') {
                putchar('\\');
            } else if (c == 'n') {
                putchar('\n');
            }
            isEscaped = 0;
        } else {
            putchar(c);
        }
    }

    va_end(parameters);

    return 0;
}
