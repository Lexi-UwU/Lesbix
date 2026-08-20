//Abstraction layer for hazlenut operations like output


#define HVM_ABSTRACTION

#ifndef UART_C
    #include "../uart/uart.h"
#endif

#include "../tools/utils.h"

void hvm_print(const char *s) {
    print_uart0(s);
}

void hvm_print_int(const int *i) {
    if (i == NULL) return;

    int num = *i;
    char buffer[12]; // Fits 32-bit INT_MIN ("-2147483648\0")
    int idx = 0;

    // Handle 0 explicitly
    if (num == 0) {
        hvm_print("0");
        return;
    }

    // Handle negative numbers
    if (num < 0) {
        hvm_print("-");
        // Avoid overflow issue with INT_MIN by processing digits directly
        num = -num;
    }

    // Extract digits in reverse
    while (num > 0) {
        buffer[idx++] = (num % 10) + '0';
        num /= 10;
    }

    // Reverse string in-place
    for (int j = 0, k = idx - 1; j < k; j++, k--) {
        char temp = buffer[j];
        buffer[j] = buffer[k];
        buffer[k] = temp;
    }

    buffer[idx] = '\0';

    // Output using your existing string print function
    hvm_print(buffer);
}
