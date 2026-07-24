//Abstraction layer for hazlenut operations like output


#define HVM_ABSTRACTION

#ifndef UART_C
    #include "../uart/uart.h"
#endif



void hvm_print(const char *s) {
    print_uart0(s);
}
