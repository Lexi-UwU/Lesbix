#ifndef UART_C
    #include "uart.c"
#endif



#ifndef TOOLS_HALT
    #include "tools/halt.c"
#endif

//This has to be defined before the CLI to avoid a crash
int LESBIX_RUNNING = 1;

#include "cli.c"




void main(void) {

    LESBIX_RUNNING = 1;

    // In a real bare-metal app, you would initialize 
    // a hardware register here to blink an LED or 
    // write to a serial port address.

    print_uart0("Welcome to Lesbix\n\0");
    print_uart0("Commands:\n\0");
    print_uart0(">");
    //print_uart0("Hello, Bare Metal World!\n\0");
    while(LESBIX_RUNNING) {
        handle_cli();

    }
    halt();
}
