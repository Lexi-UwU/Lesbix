#define UART_C
//This is the data transfer.
volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;

//This are the flags for the current state of the hardware
volatile unsigned int * const UART0FR = (unsigned int *)0x101f1018;

void print_uart0(const char *s) {
    while(*s != '\0') {
        // Wait until the 'TXFF' (Transmit FIFO Full) bit is 0
        while(*UART0FR & 0x20) {
            // Do nothing, wait for hardware to be ready
        }
        *UART0DR = (unsigned int)(*s);
        s++;
    }
}

void send_uart0(char c) {
    // Wait for TXFF (Transmit FIFO Full) to be 0
    while(*UART0FR & 0x20) {}
    *UART0DR = (unsigned int)c;
}


char read_uart0(void) {
    // 1. Wait until the 'RXFE' (Receive FIFO Empty) bit is 0
    // The RXFE bit is typically bit 4 (0x10) in the Flag Register
    while (*UART0FR & 0x10) {
        // Wait until there is data in the buffer
    }

    // 2. Read the received data from the Data Register
    return (char)(*UART0DR);
}


