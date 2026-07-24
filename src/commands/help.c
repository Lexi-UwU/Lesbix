


#ifndef UART_C
    #include "./uart/uart.h"
#endif


void command_help(){
    print_uart0("available commands: ");
    send_uart0('\n');
    print_uart0("   help  : show this list");
    send_uart0('\n');
    print_uart0("   exit  : stop the kernel");
    send_uart0('\n');
    print_uart0("   echo  : prints the text that comes afterwards");
    send_uart0('\n');
    print_uart0("   clear : clears the screen");
    send_uart0('\n');
}
