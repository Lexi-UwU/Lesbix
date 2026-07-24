
#ifndef FILESYSTEM_CONSTS
    #include "../consts.h"
#endif



#ifndef UART_C
    #include "../../uart/uart.h"
#endif

#include "../../tools/utils.h"


//This driver creates a small hardcoded read only memory format, this should only be used as an example for how the interface works


//Return codes (first line):
// 001 SUCCSESS

// 010 ERROR
// 011 COMMAND NOT IMPLMENETED

// 012 WTF case, Use for when you really shouldnt be able to get to this part of the code
char* EXAMPLE_DRIVER_FOLDER_READ(const char *path){

    if (strcmp(path, "/root") == 0) {
        return "001\nfile1.txt\nfile2.txt\ntest.hazel"; // String literal is stored in ROM
    }
    if (strcmp(path, "/") == 0) {
        return "001\nfile1.txt\nfile2.txt"; // String literal is stored in ROM
    }
    return "010\nError: Path not found";
}


char* EXAMPLE_DRIVER_FILE_READ(const char *path){

    if (strcmp(path, "/test.hazel") == 0) {
        return ""; // String literal is stored in ROM
    }
    return "010\nError: Path not found";
}





char* EXAMPLE_DRIVER_RUN(const char *command,const char *path, const char *data){
    if (command == FILESYSTEM_CONSTS_FOLDER_READ){
        //print_uart0("Reading folder\n");
        return EXAMPLE_DRIVER_FOLDER_READ(path);
        //Read Folder contents
    }else if (command == FILESYSTEM_CONSTS_FILE_READ){

        return EXAMPLE_DRIVER_FILE_READ(path);
        //Read File
    }else if (command == FILESYSTEM_CONSTS_FILE_WRITE){
        //Read File
    }
    else{
        return "011\n INVALID COMMAND";

    }

    return "012\n how..... how did you even get here?";

}

