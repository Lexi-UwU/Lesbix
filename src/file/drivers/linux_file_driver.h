//
// Created by Lexi on 24/07/2026.
//

#ifndef LESBIX_LINUX_FILE_DRIVER_H
#define LESBIX_LINUX_FILE_DRIVER_H

char* LINUX_FILE_DRIVER_FOLDER_READ(const char *path);
char* LINUX_FILE_DRIVER_FILE_READ(const char *path);
char* LINUX_FILE_DRIVER_RUN(const char *command, const char *path, const char *data);

#endif //LESBIX_LINUX_FILE_DRIVER_H
