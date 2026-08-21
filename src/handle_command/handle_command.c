#include <string.h>

#ifndef UART_C
    #include "../uart/uart.h"
#endif

#ifndef TOOLS_UTILS
    #include "../tools/utils.h"
#endif

#include "../file/driver_response.h"




#include "../commands/help.h"
#include "../commands/exit.h"
#include "../commands/echo.h"
#include "../commands/clear.h"
#include "../commands/ls.h"
#include "../commands/cd.h"
#include "../commands/mkdir.h"
#include "../commands/cat.h"
#include "../commands/pwd.h"
#include "../commands/hazelnut.h"

int handle_command_init() {
    return command_hazelnut_init();
}
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
    }else if (strcmp(cmd, "ls") == 0){
        command_ls(s);
    }else if (strcmp(cmd, "cd") == 0){
        command_cd(s);
    }else if (strcmp(cmd, "mkdir") == 0){
        command_mkdir(s);
    }else if (strcmp(cmd, "cat") == 0){
        command_cat(s);
    }else if (strcmp(cmd, "pwd") == 0){
        command_pwd();
    }else if (strcmp(cmd, "hazelnut") == 0){
        command_hazelnut(s);
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
