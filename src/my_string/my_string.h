#ifndef MY_STRING_H
#define MY_STRING_H

#include <stddef.h>

char *duplicate_string(char *str);
char *duplicate_string_until(char *str, int c, size_t *index);

int remove_last(char *str, char *to_remove);

#endif /* MY_STRING_H */