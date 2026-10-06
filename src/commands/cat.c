
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
    char *cmd;
    char *temp_path;
    DriverResponse resp; // Local struct on the stack
    // Copy input safely
    strncpy(buffer, s, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // 1. Tokenize to find the command
    cmd = strtok(buffer, " ");

    // 2. Check if the command is "echo"
    if (cmd != NULL && strcmp(cmd, "cat") == 0) {
        // 3. Get the rest of the string as the argument
        char *arg = strtok(NULL, "");

        if (arg != NULL) {
            // Send the parsed argument to UART
            //print_uart0(arg);
            temp_path = FILESYSTEM_MERGE_PATHS(FILESYSTEM_CURRENT_WORKING_DIRECTORY, arg);

            print_uart0("\n LENGTH: ");

            print_int_uart0(strlen(temp_path));

            print_uart0(temp_path);

            print_uart0("\n");

            FILESYSTEM_GET_FILE(temp_path, &resp);

            print_uart0(resp.data_char);
        }
        send_uart0('\n');
    }


    //send_uart0('\n');



}
