//
// Created by Lexi on 24/07/2026.
//

#include "linux_file_driver.h"
#include "../driver_response.h"

#ifndef FILESYSTEM_CONSTS
    #include "../consts.h"
#endif

#ifndef UART_C
    #include "../../uart/uart.h"
#endif

#include "../../tools/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "../error_message.h"



// Return codes (first line):
// 001 SUCCESS
// 010 ERROR
// 011 COMMAND NOT IMPLEMENTED
// 012 WTF case, Use for when you really shouldn't be able to get to this part of the code
// 021 File is a folder
// 022 Folder is a file

char *LINUX_FILESYSTEM_BASE_PATH = "./filesystem";

// Helper function to create error responses easily


DriverResponse LINUX_FILE_DRIVER_FOLDER_READ(const char *path) {
    if (path == NULL || path[0] == '\0') {
        path = "/";
    }

    char full_path[512];
    snprintf(full_path, sizeof(full_path), "%s%s", LINUX_FILESYSTEM_BASE_PATH, path);

    DIR *dir = opendir(full_path);
    if (!dir) {
        return FILESYSTEM_CREATE_ERROR_RESPONSE("010\nError: Folder not found or cannot be opened", 10);
    }

    size_t buffer_size = 4096;
    char *buffer = malloc(buffer_size);
    if (!buffer) {
        closedir(dir);
        return FILESYSTEM_CREATE_ERROR_RESPONSE("010\nError: Memory allocation failed", 10);
    }

    strcpy(buffer, "001\n");
    size_t current_len = 4;

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
                return FILESYSTEM_CREATE_ERROR_RESPONSE("010\nError: Memory reallocation failed", 10);
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

    DriverResponse resp;
    resp.data_char = buffer;
    resp.size = current_len;
    return resp;
}

DriverResponse LINUX_FILE_DRIVER_FILE_READ(const char *path) {
    if (path == NULL || path[0] == '\0') {
        return FILESYSTEM_CREATE_ERROR_RESPONSE("010\nError: Invalid path", 10);
    }

    char full_path[512];
    snprintf(full_path, sizeof(full_path), "%s%s", LINUX_FILESYSTEM_BASE_PATH, path);

    FILE *file = fopen(full_path, "rb");
    if (!file) {
        return FILESYSTEM_CREATE_ERROR_RESPONSE("010\nError: File not found or cannot be opened", 10);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size < 0) {
        fclose(file);
        return FILESYSTEM_CREATE_ERROR_RESPONSE("010\nError: Failed to measure file size", 10);
    }

    // Pad file_size to ensure the sentinel location is aligned to sizeof(int)
    size_t aligned_payload_size = (file_size + sizeof(int) - 1) & ~(sizeof(int) - 1);

    // Allocate space for file payload + 1 int sentinel (-1) + 1 extra byte for string '\0'
    size_t total_alloc_size = aligned_payload_size + sizeof(int) + 1;

    char *buffer = malloc(total_alloc_size);
    if (!buffer) {
        fclose(file);
        return FILESYSTEM_CREATE_ERROR_RESPONSE("010\nError: Memory allocation failed", 10);
    }

    // Zero out padding bytes
    memset(buffer, 0, total_alloc_size);

    // Read raw file content directly starting at offset 0
    size_t read_bytes = fread(buffer, 1, file_size, file);
    fclose(file);

    if (read_bytes != (size_t)file_size) {
        free(buffer);
        return FILESYSTEM_CREATE_ERROR_RESPONSE("010\nError: Failed to read file contents", 10);
    }

    // 1. Text terminator (ensures safe string operations if treating as text)
    buffer[file_size] = '\0';

    // 2. Binary sentinel (ensures hazlenut_run_file stops safely when treating as binary)
    int sentinel = -1;
    memcpy(buffer + aligned_payload_size, &sentinel, sizeof(int));

    DriverResponse resp;
    resp.data_char = buffer;                        // Clean string reading (starts at byte 0)
    resp.data_int  = (int *)(void *)buffer;        // Aligned integer array (starts at byte 0)
    resp.size      = file_size;
    resp.response_code = 1;
    return resp;
}

DriverResponse LINUX_FILE_DRIVER_RUN(const char *command, const char *path, const char *data) {
    if (strcmp(command, FILESYSTEM_CONSTS_FOLDER_READ) == 0) {
        return LINUX_FILE_DRIVER_FOLDER_READ(path);
    } else if (strcmp(command, FILESYSTEM_CONSTS_FILE_READ) == 0) {
        return LINUX_FILE_DRIVER_FILE_READ(path);
    } else if (strcmp(command, FILESYSTEM_CONSTS_FILE_WRITE) == 0) {
        // Handle file write implementation here
    } else {
        return FILESYSTEM_CREATE_ERROR_RESPONSE("011\n INVALID COMMAND",10);
    }

    return FILESYSTEM_CREATE_ERROR_RESPONSE("012\n how..... how did you even get here?",10);
}