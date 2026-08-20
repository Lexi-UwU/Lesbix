#define TOOLS_UTILS

#if defined(__arm__) || defined(__aarch64__)

// Define standard types manually
typedef unsigned long size_t;
#define NULL ((void *)0)

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}


// Define standard types manually
//typedef unsigned long size_t;
//#define NULL ((void *)0)

// Helper: Find length of the initial segment consisting only of accepted characters
 size_t strspn(const char *str, const char *delim) {
    size_t count = 0;
    while (str[count] != '\0') {
        int found = 0;
        for (const char *d = delim; *d != '\0'; d++) {
            if (str[count] == *d) {
                found = 1;
                break;
            }
        }
        if (!found) break;
        count++;
    }
    return count;
}

// Helper: Locate first occurrence of any character from delim in str
 char *strpbrk(const char *str, const char *delim) {
    while (*str != '\0') {
        for (const char *d = delim; *d != '\0'; d++) {
            if (*str == *d) return (char *)str;
        }
        str++;
    }
    return NULL;
}

// The strtok implementation
char *strtok(char *str, const char *delim) {
    static char *last_token = NULL;
    char *token;

    if (str == NULL) {
        str = last_token;
    }

    if (str == NULL) return NULL;

    str += strspn(str, delim);

    if (*str == '\0') {
        last_token = NULL;
        return NULL;
    }

    token = str;
    str = strpbrk(token, delim);

    if (str == NULL) {
        last_token = NULL;
    } else {
        *str = '\0';
        last_token = str + 1;
    }

    return token;
}


char *strncpy(char *dest, const char *src, size_t n) {
    size_t i;

    // 1. Copy up to n characters
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    // 2. Pad the remainder with '\0'
    for (; i < n; i++) {
        dest[i] = '\0';
    }

    return dest;
}


char *strcat(char *dest, const char *src) {
    char *ptr = dest;

    // 1. Find the end of the destination string
    while (*ptr != '\0') {
        ptr++;
    }

    // 2. Copy the source string to the end of the destination
    while (*src != '\0') {
        *ptr = *src;
        ptr++;
        src++;
    }

    // 3. Add the mandatory null terminator
    *ptr = '\0';

    // 4. Return the pointer to the original destination
    return dest;
}


size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') len++;
    return len;
}

char *strcpy(char *dest, const char *src) {
    char *ptr = dest;
    while ((*dest++ = *src++));
    return ptr;
}

void *memcpy(void *dest, const void *src, size_t n) {
    char *d = (char *)dest;
    const char *s = (const char *)src;
    while (n--) *d++ = *s++;
    return dest;
}

// Simplified bump allocator (unsafe for complex systems)
extern char _end; // Defined in your linker script at the end of .bss
static void *heap_ptr = &_end;

void *malloc(size_t size) {
    void *ptr = heap_ptr;
    heap_ptr += size; // Simply "bump" the pointer forward
    return ptr;
}


#include <stdarg.h>

#include <stdio.h>

#else

#include <string.h>

#endif

#include <stdio.h>