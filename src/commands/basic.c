#define COMMAND_BASIC

#ifndef UART_C
#include "./uart/uart.h"
#endif

#include "../tools/utils.h"


#include "../basic/basic.h"

#include "../file/filesystem.h"

void command_basic(const char *s) {
    char buffer[64];
    char *cmd;
    char *arg;
    char *path;

    //print_uart0("RUNNING BASIC");



    // Copy input safely
    strncpy(buffer, s, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // 1. Tokenize to find the command
    cmd = strtok(buffer, " ");

    //print_uart0(cmd);

    // 2. Check if the command is "echo"
    if (cmd != NULL) {
        // 3. Get the rest of the string as the argument
        arg = strtok(NULL, "");

        if (arg != NULL) {
            // Send the parsed argument to UART

            path = FILESYSTEM_MERGE_PATHS(FILESYSTEM_CURRENT_WORKING_DIRECTORY, arg);
            //print_uart0("\n ARG: ");
            //print_uart0(arg);
            //print_uart0("\n PATH: ");
            //print_uart0(path);
            //print_uart0("\n");


            LESBIX_BASIC_LOAD_PROGRAM(path);

            //print_uart0("LINES LOADED, RUNNING BASIC");

            LESBIX_BASIC_RUN();

        }
        send_uart0('\n');
    }
}
