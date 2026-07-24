//Hazelut Programming Language Virtual Machine

//The system uses a 32 bit address space

//Format:

//ADDRESS  : INSTRUCTION : ADDR 1   : ADDR 2

//FFFFFFFF : FFFF    	 : FFFFFFFF : FFFFFFFF


#ifndef FILESYSTEM_MANAGER
    #include "../file/filesystem.h"
#endif


#ifndef UART_C
    #include "../uart/uart.h"
#endif

#ifndef HVM_CORE
    #include "../hazelnut/hazelnut_vm.h"
#endif


#include "../tools/utils.h"




//More functionality needs to be added in the future
//Run Command
void command_hazelnut(const char *s){


    //Get file from filesystem
    char *file =  FILESYSTEM_GET_FILE(FILESYSTEM_MERGE_PATHS(FILESYSTEM_CURRENT_WORKING_DIRECTORY,s));

    hazlenut_run_file(file);

}

