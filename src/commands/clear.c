
#ifndef UART_C
    #include "uart.c"
#endif


void command_clear(){

    print_uart0("\033[2J");
}
