


#ifndef UART_C
    #include "uart.c"
#endif

#ifndef TOOLS_HALT
    #include "tools/halt.c"
#endif

//int LESBIX_RUNNING = 1;



void command_exit(){
    print_uart0("Goodbye");
    LESBIX_RUNNING = 0;
    halt();
}
