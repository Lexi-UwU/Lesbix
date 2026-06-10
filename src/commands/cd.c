
#ifndef UART_C
    #include "uart.c"
#endif

#ifndef FILESYSTEM_MANAGER
    #include "../file/filesystem.c"
#endif



void command_cd(const char *s){


    char buffer[64];
    // Copy input safely
    strncpy(buffer, s, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // 1. Tokenize to find the command
    char *cmd = strtok(buffer, " ");

    // 2. Check if the command is "echo"
    if (cmd != NULL && strcmp(cmd, "cd") == 0) {
        // 3. Get the rest of the string as the argument
        char *arg = strtok(NULL, "");

        if (arg != NULL) {
            // Send the parsed argument to UART
            print_uart0(arg);
            FILESYSTEM_SET_DIRECTORY(FILESYSTEM_MERGE_PATHS(FILESYSTEM_CURRENT_WORKING_DIRECTORY,arg));


        }
        send_uart0('\n');
    }



}
