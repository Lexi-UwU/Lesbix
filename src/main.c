#include <stdio.h>
#include <stdlib.h>

#ifndef UART_C
    #include "uart/uart.h"
#endif



#ifndef TOOLS_HALT
    #include "tools/halt.h"
#endif

//#include "vga/vga.c"

//This has to be defined before the CLI to avoid a crash
int LESBIX_RUNNING = 1;

#include "cli/cli.h"




void delay(int count) {
    for (int i = 0; i < count; i++) {
        __asm__("nop"); // Does nothing, just burns cycles
    }
}

#include "./commands/help.h"


void main(void) {
    //initialise_lcd();

    // delay(1000000);

    //draw_pixel(32,32,0xffff);

    //clear_screen(0xFFFFFF);

    //draw_diagonal();

    //printf("Hello World!\n");



    LESBIX_RUNNING = 1;


    print_uart0("Welcome to Lesbix\n\0");
    print_uart0("\n");
    command_help();
    print_uart0("\n");
    print_uart0(">");


    while(LESBIX_RUNNING) {
        //printf("Hello World!\n");

        //clear_screen(0xFFF);b
        handle_cli();

    }
    halt();
}
