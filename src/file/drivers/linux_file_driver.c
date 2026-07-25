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

// This driver creates a small hardcoded read only memory format, this should only be used as an example for how the interface works

// Return codes (first line):
// 001 SUCCESS
// 010 ERROR
// 011 COMMAND NOT IMPLEMENTED
// 012 WTF case, Use for when you really shouldn't be able to get to this part of the code
// 021 File is a folder
// 022 Folder is a file

char *LINUX_FILESYSTEM_BASE_PATH = "./filesystem";

// Helper function to create error responses easily
static DriverResponse create_error_response(const char *err_msg) {
    DriverResponse resp;
    resp.size = strlen(err_msg);
    resp.data = malloc(resp.size + 1);
    if (resp.data) {
        strcpy(resp.data, err_msg);
    }
    return resp;
}

DriverResponse LINUX_FILE_DRIVER_FOLDER_READ(const char *path) {
    if (path == NULL || path[0] == '\0') {
        path = "/";
    }

    char full_path[512];
    snprintf(full_path, sizeof(full_path), "%s%s", LINUX_FILESYSTEM_BASE_PATH, path);

    DIR *dir = opendir(full_path);
    if (!dir) {
        return create_error_response("010\nError: Folder not found or cannot be opened");
    }

    size_t buffer_size = 4096;
    char *buffer = malloc(buffer_size);
    if (!buffer) {
        closedir(dir);
        return create_error_response("010\nError: Memory allocation failed");
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
                return create_error_response("010\nError: Memory reallocation failed");
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
    resp.data = buffer;
    resp.size = current_len;
    return resp;
}

DriverResponse LINUX_FILE_DRIVER_FILE_READ(const char *path) {
    if (path == NULL || path[0] == '\0') {
        return create_error_response("010\nError: Invalid path");
    }

    char full_path[512];
    snprintf(full_path, sizeof(full_path), "%s%s", LINUX_FILESYSTEM_BASE_PATH, path);

    // Open file in binary read mode
    FILE *file = fopen(full_path, "rb");
    if (!file) {
        return create_error_response("010\nError: File not found or cannot be opened");
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size < 0) {
        fclose(file);
        return create_error_response("010\nError: Failed to measure file size");
    }

    // Allocate buffer: status code ("001\n" is 4 bytes) + binary file contents
    size_t status_prefix_len = 4;
    size_t buffer_size = status_prefix_len + file_size;
    char *buffer = malloc(buffer_size);
    if (!buffer) {
        fclose(file);
        return create_error_response("010\nError: Memory allocation failed");
    }

    // Copy success status code safely using memcpy
    memcpy(buffer, "001\n", status_prefix_len);

    // Read the binary file contents directly after the status code prefix
    size_t read_bytes = fread(buffer + status_prefix_len, 1, file_size, file);
    if (read_bytes != (size_t)file_size) {
        free(buffer);
        fclose(file);
        return create_error_response("010\nError: Failed to read file contents");
    }

    fclose(file);

    DriverResponse resp;
    resp.data = buffer;
    resp.size = buffer_size;
    return resp;
}

DriverResponse LINUX_FILE_DRIVER_RUN(const char *command, const char *path, const char *data) {
    if (command == FILESYSTEM_CONSTS_FOLDER_READ) {
        return LINUX_FILE_DRIVER_FOLDER_READ(path);
    } else if (command == FILESYSTEM_CONSTS_FILE_READ) {
        return LINUX_FILE_DRIVER_FILE_READ(path);
    } else if (command == FILESYSTEM_CONSTS_FILE_WRITE) {
        // Handle file write implementation here
    } else {
        return create_error_response("011\n INVALID COMMAND");
    }

    return create_error_response("012\n how..... how did you even get here?");
}