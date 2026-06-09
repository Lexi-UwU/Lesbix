#include <string.h>

#ifndef UART_C
    #include "uart.c"
#endif

#ifndef TOOLS_UTILS
    #include "tools/utils.c"
#endif


#include "commands/help.c"
#include "commands/exit.c"

void handle_command(const char *s) {

    if (strcmp(s, "help") == 0){
        command_help();
    }
    if (strcmp(s, "exit") == 0){
        command_exit();
    }
    //print_uart0(s);
    //send_uart0('\n');


}
