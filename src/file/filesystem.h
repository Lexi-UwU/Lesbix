//
// Created by Lexi on 24/07/2026.
//

#ifndef LESBIX_FILESYSTEM_H
#define LESBIX_FILESYSTEM_H
#include <stddef.h>


// Global variables
extern char *FILESYSTEM_CURRENT_WORKING_DIRECTORY;

// Function declarations
char *FILESYSTEM_GET_FILES(const char *path);
char *FILESYSTEM_GET_FILE(const char *path);
char *FILESYSTEM_GET_FILE_BYTES(const char *path);
int FILESYSTEM_SET_DIRECTORY(char *path);
char *FILESYSTEM_MERGE_PATHS(const char *path1, const char *path2);

#endif //LESBIX_FILESYSTEM_H
