#ifndef UART_C
    #include "../uart/uart.h"
#endif

#include "../handle_command/handle_command.h"


#define MAX_BUF 128
char line_buffer[MAX_BUF];
int command_buffer_index = 0;

int cli_init(void) {
   return handle_command_init();

}

void handle_cli(){
    char received = read_uart0();
    if (received == '\r' || received == '\n'){

#if defined(__arm__) || defined(__aarch64__)

        send_uart0('\n');
        send_uart0('\r');
#else

        send_uart0('\n');
        send_uart0('\r');
#endif
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
#if defined(__arm__) || defined(__aarch64__)
        send_uart0(received); // Echo the character back
#else
        send_uart0(received);

#endif
    }
}
