#include "uart.c"


#define MAX_BUF 100
char line_buffer[MAX_BUF];
int index = 0;


void main(void) {

    // In a real bare-metal app, you would initialize 
    // a hardware register here to blink an LED or 
    // write to a serial port address.
    print_uart0("Hello, Bare Metal World!\n\0");
    while(1) {
        char received = read_uart0();
        if (received == '\r'){

            send_uart0('\n');
            send_uart0('\r');
            line_buffer[index] = '\0';
            print_uart0(line_buffer);
            send_uart0('\n');
            print_uart0(">");
            index = 0;
        }else if (received == '\b'|| received == 127) {
            // Visually delete the character on the terminal
            if (index > 0) {
                index --;
                send_uart0('\b');
                send_uart0(' ');
                send_uart0('\b');
            }
        }
        else{
            line_buffer[index] = received; // Store character
            index++;
            send_uart0(received); // Echo the character back
        }
    }
}
