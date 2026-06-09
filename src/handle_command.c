#ifndef UART_C
#include "uart.c"
#endif


void handle_command(const char *s) {
    print_uart0(s);
    send_uart0('\n');


}
