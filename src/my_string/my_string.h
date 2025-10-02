#ifndef MY_STRING_H
#define MY_STRING_H

#include <stddef.h>

char *duplicate_string(char *str);
char *duplicate_string_until(char *str, int c, size_t *index);

#endif /* MY_STRING_H */