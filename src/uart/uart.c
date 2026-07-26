#define UART_C

#ifndef TOOLS_UTILS
    #include "../tools/utils.h"
#endif

#if defined(__arm__) || defined(__aarch64__)

#else
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#endif

//This is the data transfer.
volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;

//This are the flags for the current state of the hardware
volatile unsigned int * const UART0FR = (unsigned int *)0x101f1018;

void print_uart0(const char *s) {

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

}

void send_uart0(char c) {
#if defined(__arm__) || defined(__aarch64__)
    // Wait for TXFF (Transmit FIFO Full) to be 0
    while(*UART0FR & 0x20) {}
    *UART0DR = (unsigned int)c;
#else
    putchar(c);
#endif
}


char read_uart0(void) {
#if defined(__arm__) || defined(__aarch64__)
    // 1. Wait until the 'RXFE' (Receive FIFO Empty) bit is 0
    while (*UART0FR & 0x10) {
        // Wait until there is data in the buffer
    }

    // 2. Read the received data from the Data Register
    return (char)(*UART0DR);
#else
    // Host fallback: Configure terminal for non-canonical (raw) input
    struct termios oldt, newt;
    char c;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Disable canonical mode (line buffering) and local echo
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    c = getchar();

    // Restore original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    if (c == EOF) {
        return '\0';
    }
    //send_uart0(c);
    //send_uart0('\r');
    return c;

    /*
    // Host fallback (e.g., standard input for testing on x86)
    int c = getchar();
    if (c == EOF) {
        return '\0';
    }
    return (char)c;

    */
#endif
}


