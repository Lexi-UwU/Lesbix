
extern int LESBIX_RUNNING;


#ifndef UART_C
    #include "./uart/uart.h"
#endif

#ifndef TOOLS_HALT
    #include "tools/halt.h"
#endif

//int LESBIX_RUNNING = 1;



void command_exit(){
    print_uart0("Goodbye");
    LESBIX_RUNNING = 0;
    halt();
}
