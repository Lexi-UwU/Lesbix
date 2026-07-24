//
// Created by Lexi on 24/07/2026.
//

#ifndef LESBIX_UTILS_H
#define LESBIX_UTILS_H


#include <stddef.h>
#include <string.h>

int strcmp(const char *s1, const char *s2);
size_t strspn(const char *s, const char *accept);
char *strpbrk(const char *s, const char *accept);
char *strtok(char *str, const char *delim);
char *strncpy(char *dest, const char *src, size_t n);
char *strcat(char *dest, const char *src);
size_t strlen(const char *s);
char *strcpy(char *dest, const char *src);
void *memcpy(void *dest, const void *src, size_t n);
void *malloc(size_t size);

#endif //LESBIX_UTILS_H