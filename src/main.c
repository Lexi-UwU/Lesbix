volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;
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

void main(void) {
    print_uart0("Hello, Bare Metal World!\n\0");
    // In a real bare-metal app, you would initialize 
    // a hardware register here to blink an LED or 
    // write to a serial port address.
    while(1) {
        // Infinite loop
    }
}
