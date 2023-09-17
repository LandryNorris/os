#ifndef OS_STRING_H
#define OS_STRING_H

#include <stddef.h>

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char*);
char* strcat(char*, char*);
char* strchr(const char*, int);
char* strcpy(char*, const char*);
int strcmp(const char* str1, const char* str2);

#endif //OS_STRING_H
