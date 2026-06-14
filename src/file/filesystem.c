
#define FILESYSTEM_MANAGER

#ifndef FILESYSTEM_CONSTS
    #include "consts.c"
#endif

#ifndef TOOLS_UTILS
    #include "../tools/utils.c"
#endif



#include "drivers/example_driver.c"


char * FILESYSTEM_CURRENT_WORKING_DIRECTORY = "/";


char *FILESYSTEM_GET_FILES(const char *path){
    //Find driver and send appropriate command


    return EXAMPLE_DRIVER_RUN(FILESYSTEM_CONSTS_FOLDER_READ,path, "");

}


char *FILESYSTEM_GET_FILE(const char *path){
    //Find driver and send appropriate command


    return EXAMPLE_DRIVER_RUN(FILESYSTEM_CONSTS_FILE_READ,path, "");

}


char *FILESYSTEM_GET_FILE_BYTES(const char *path){
    //Find driver and send appropriate command


    return EXAMPLE_DRIVER_RUN(FILESYSTEM_CONSTS_FILE_READ_BYTES,path, "");

}




int FILESYSTEM_SET_DIRECTORY(char *path){

    FILESYSTEM_CURRENT_WORKING_DIRECTORY = path;
    return 0;
}

char *FILESYSTEM_MERGE_PATHS(const char *path1, const char *path2) {
    // 1. Calculate required length: path1 + "/" + path2 + '\0'
    size_t len1 = strlen(path1);
    size_t len2 = strlen(path2);

    // Check if path1 already ends with a slash to avoid double slashes
    int needs_slash = (path1[len1 - 1] != '/');

    // Allocate: len1 + (optional /) + len2 + null terminator
    char *result = malloc(len1 + (needs_slash ? 1 : 0) + len2 + 1);

    if (result == NULL) return NULL; // Allocation failed

    // 2. Build the string
    strcpy(result, path1);
    if (needs_slash) {
        strcat(result, "/");
    }
    strcat(result, path2);

    return result;
}




