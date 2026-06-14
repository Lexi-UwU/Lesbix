//Hazelut Programming Language Virtual Machine

//The system uses a 32 bit address space

//Format:

//ADDRESS  : INSTRUCTION : ADDR 1   : ADDR 2

//FFFFFFFF : FFFF    	 : FFFFFFFF : FFFFFFFF


#ifndef FILESYSTEM_MANAGER
    #include "../file/filesystem.c"
#endif


#ifndef UART_C
    #include "uart.c"
#endif




//More functionality needs to be added in the future
void command_hazelnut(const char *s){


    FILESYSTEM_GET_FILE(FILESYSTEM_MERGE_PATHS(FILESYSTEM_CURRENT_WORKING_DIRECTORY,s));

}

