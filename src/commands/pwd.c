#ifndef FILESYSTEM_MANAGER
    #include "../file/filesystem.h"
#endif

#ifndef UART_C
    #include "../uart/uart.h"
#endif



void command_pwd(){


    //FILESYSTEM_GET_FILES("/");
    //print_uart0("ls : Command not implemented\n");
    print_uart0(FILESYSTEM_CURRENT_WORKING_DIRECTORY);
    //rint_uart0(FILESYSTEM_GET_FILES(FILESYSTEM_CURRENT_WORKING_DIRECTORY));
    send_uart0('\n');



}
