#include <stdlib.h>

#ifndef UART_C
    #include "uart.c"
#endif



#ifndef TOOLS_HALT
    #include "tools/halt.c"
#endif

//#include "vga/vga.c"

//This has to be defined before the CLI to avoid a crash
int LESBIX_RUNNING = 1;

#include "cli.c"




void delay(int count) {
    for (int i = 0; i < count; i++) {
        __asm__("nop"); // Does nothing, just burns cycles
    }
}


void main(void) {
    //initialise_lcd();

    // delay(1000000);

    //draw_pixel(32,32,0xffff);

    //clear_screen(0xFFFFFF);

    //draw_diagonal();



    LESBIX_RUNNING = 1;


    print_uart0("Welcome to Lesbix\n\0");
    print_uart0("\n");
    command_help();
    print_uart0("\n");
    print_uart0(">");


    while(LESBIX_RUNNING) {

        //clear_screen(0xFFF);
        handle_cli();

    }
    halt();
}
