
#define COMMAND_ECHO

#ifndef UART_C
    #include "uart.c"
#endif


void command_echo(const char *s) {
    char buffer[64];
    // Copy input safely
    strncpy(buffer, s, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // 1. Tokenize to find the command
    char *cmd = strtok(buffer, " ");

    // 2. Check if the command is "echo"
    if (cmd != NULL && strcmp(cmd, "echo") == 0) {
        // 3. Get the rest of the string as the argument
        char *arg = strtok(NULL, "");

        if (arg != NULL) {
            // Send the parsed argument to UART
            print_uart0(arg);
        }
        send_uart0('\n');
    }
}
