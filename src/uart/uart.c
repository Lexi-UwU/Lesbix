#define UART_C

#ifndef TOOLS_UTILS
#include "../tools/utils.h"
#endif

#if defined(__arm__) || defined(__aarch64__)

#else
#include <stdio.h>
#endif


#include "uart.h"

#ifdef __CC65__
#include "acia.c"
#include "screen.h"

#else




    #if defined(__arm__)

        //This is the data transfer.
        volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;

        //This are the flags for the current state of the hardware
        volatile unsigned int * const UART0FR = (unsigned int *)0x101f1018;
    #endif

#endif

void print_uart0(const char *s) {

    #ifdef __CC65__

    acia_print(s);
    #else
    #if defined(__arm__) || defined(__aarch64__)

    while(*s != '\0') {
        // Wait until the 'TXFF' (Transmit FIFO Full) bit is 0
        while(*UART0FR & 0x20) {
            // Do nothing, wait for hardware to be ready
        }
        *UART0DR = (unsigned int)(*s);
        s++;
    }

    #else
    printf(s);
    #endif
    #endif

}

void send_uart0(char c) {

    #ifdef __CC65__

    acia_send(c);

    #else
    #if defined(__arm__) || defined(__aarch64__)
    // Wait for TXFF (Transmit FIFO Full) to be 0
    while(*UART0FR & 0x20) {}
    *UART0DR = (unsigned int)c;
    #else
    putchar(c);
    #endif
    #endif
}


void init_uart0(){
    #ifdef __CC65__

    init_acia();

    acia_test();

    screen_init();

    #endif

}

void print_int_uart0(int n) {
    char buf[10];
    int i = 0;
    if (n == 0) {
        send_uart0('0');
        return;
    }

    while (n > 0) {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }
    while (i > 0) {
        send_uart0(buf[--i]);
    }
}


char read_uart0(void) {

    #ifdef __CC65__

    return acia_read();

    #else
    #if defined(__arm__) || defined(__aarch64__)
    // 1. Wait until the 'RXFE' (Receive FIFO Empty) bit is 0
    while (*UART0FR & 0x10) {
        // Wait until there is data in the buffer
    }

    // 2. Read the received data from the Data Register
    return (char)(*UART0DR);
    #else
    // Host fallback (e.g., standard input for testing on x86)
    int c = getchar();
    if (c == EOF) {
        return '\0';
    }
    return (char)c;
    #endif

    #endif
}
