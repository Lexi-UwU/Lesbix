//
// Created by Lexi on 24/07/2026.
//

#include "linux_file_driver.h"



#ifndef FILESYSTEM_CONSTS
    #include "../consts.h"
#endif



#ifndef UART_C
    #include "../../uart/uart.h"
#endif

#include "../../tools/utils.h"



#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <dirent.h>


//This driver creates a small hardcoded read only memory format, this should only be used as an example for how the interface works


//Return codes (first line):
// 001 SUCCSESS

// 010 ERROR
// 011 COMMAND NOT IMPLMENETED

// 012 WTF case, Use for when you really shouldnt be able to get to this part of the code

char * LINUX_FILESYSTEM_BASE_PATH = "./filesystem";

char* LINUX_FILE_DRIVER_FOLDER_READ(const char *path){
    //print_uart0("Test\n");
    if (path == NULL || path[0] == '\0') {
        path = "/";
    }

    char full_path[512];
    snprintf(full_path, sizeof(full_path), "%s%s", LINUX_FILESYSTEM_BASE_PATH, path);

    DIR *dir = opendir(full_path);
    if (!dir) {
        return "010\nError: Folder not found or cannot be opened";
    }



    size_t buffer_size = 4096;
    char *buffer = malloc(buffer_size);
    if (!buffer) {
        closedir(dir);
        return "010\nError: Memory allocation failed";
    }


    //print_uart0("Before Strcpy\n");
    strcpy(buffer, "001\n");

    //print_uart0("After Strcpy\n");

    size_t current_len = 4;

    //print_uart0("After Len\n");



    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        size_t name_len = strlen(entry->d_name);
        if (current_len + name_len + 2 > buffer_size) {
            buffer_size *= 2;
            char *new_buffer = realloc(buffer, buffer_size);
            if (!new_buffer) {
                free(buffer);
                closedir(dir);
                return "010\nError: Memory reallocation failed";
            }
            buffer = new_buffer;
        }

        strcpy(buffer + current_len, entry->d_name);
        current_len += name_len;
        buffer[current_len++] = '\n';
        buffer[current_len] = '\0';
    }

    if (current_len > 4) {
        current_len--; // Move back past the last '\n'
        buffer[current_len] = '\0'; // Replace with null terminator
    }

    closedir(dir);
    return buffer;
}


char* LINUX_FILE_DRIVER_FILE_READ(const char *path){

    if (strcmp(path, "/test.hazel") == 0) {
        return ""; // String literal is stored in ROM
    }
    return "010\nError: Path not found";
}





char* LINUX_FILE_DRIVER_RUN(const char *command,const char *path, const char *data){
    if (command == FILESYSTEM_CONSTS_FOLDER_READ){
        //print_uart0("Reading folder\n");
        return LINUX_FILE_DRIVER_FOLDER_READ(path);
        //Read Folder contents
    }else if (command == FILESYSTEM_CONSTS_FILE_READ){

        return LINUX_FILE_DRIVER_FILE_READ(path);
        //Read File
    }else if (command == FILESYSTEM_CONSTS_FILE_WRITE){
        //Read File
    }
    else{
        return "011\n INVALID COMMAND";

    }

    return "012\n how..... how did you even get here?";

}

