#include "uart.c"
#include "handle_command.c"

#define MAX_BUF 100
char line_buffer[MAX_BUF];
int command_buffer_index = 0;


void main(void) {

    // In a real bare-metal app, you would initialize 
    // a hardware register here to blink an LED or 
    // write to a serial port address.

    print_uart0("Welcome to Lesbix\n\0");
    print_uart0("Commands:\n\0");
    print_uart0(">");
    //print_uart0("Hello, Bare Metal World!\n\0");
    while(1) {
        char received = read_uart0();
        if (received == '\r'){

            send_uart0('\n');
            send_uart0('\r');
            line_buffer[command_buffer_index] = '\0';
            handle_command(line_buffer);

            print_uart0(">");
            command_buffer_index = 0;
        }else if (received == '\b'|| received == 127) {
            // Visually delete the character on the terminal
            if (command_buffer_index > 0) {
                command_buffer_index --;
                send_uart0('\b');
                send_uart0(' ');
                send_uart0('\b');
            }
        }
        else{
            line_buffer[command_buffer_index] = received; // Store character
            command_buffer_index++;
            send_uart0(received); // Echo the character back
        }
    }
}
