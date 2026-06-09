


#ifndef UART_C
    #include "uart.c"
#endif


void command_help(){
    print_uart0("COMMANDS: ");
    send_uart0('\n');
    print_uart0("   help : show this list");
    send_uart0('\n');
    print_uart0("   exit : stop the kernel");
    send_uart0('\n');
}
