
#ifndef FILESYSTEM_MANAGER
    #include "../file/filesystem.h"
#endif

#ifndef UART_C
#include "./uart/uart.h"
#endif

#include "../tools/utils.h"



void command_cat(const char *s){

    //print_uart0("cat : Command not implemented\n");

    char buffer[64];
    // Copy input safely
    strncpy(buffer, s, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // 1. Tokenize to find the command
    char *cmd = strtok(buffer, " ");

    // 2. Check if the command is "echo"
    if (cmd != NULL && strcmp(cmd, "cat") == 0) {
        // 3. Get the rest of the string as the argument
        char *arg = strtok(NULL, "");

        if (arg != NULL) {
            // Send the parsed argument to UART
            //print_uart0(arg);
            print_uart0(FILESYSTEM_GET_FILE(FILESYSTEM_MERGE_PATHS(FILESYSTEM_CURRENT_WORKING_DIRECTORY, arg)).data_char);
        }
        send_uart0('\n');
    }


    //send_uart0('\n');



}
