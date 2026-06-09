#include <string.h>

#ifndef UART_C
    #include "uart.c"
#endif

#ifndef TOOLS_UTILS
    #include "tools/utils.c"
#endif




#include "commands/help.c"
#include "commands/exit.c"
#include "commands/echo.c"
#include "commands/clear.c"




void handle_command(const char *s) {

    char buffer[128];

    strncpy(buffer, s, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // 2. Tokenize the COPY, not the original 's'
    char *cmd = strtok(buffer, " ");

    if (cmd == NULL) return; // Empty input

    //print_uart0(cmd);

    if (strcmp(cmd, "help") == 0){
        command_help();
    }
    else if (strcmp(cmd, "exit") == 0){
        command_exit();
    }else if (strcmp(cmd, "echo") == 0){
        command_echo(s);
    }else if (strcmp(cmd, "clear") == 0){
        command_clear();
    }

    else{
        //TODO: Handle running binary files located in the filesystem
        if (1 == 1){

        }


        //Raise not found error
        //send_uart0('\n');
        print_uart0(cmd);
        print_uart0(" : Command not found");
        send_uart0('\n');
    }
    //print_uart0(s);
    //send_uart0('\n');


}
