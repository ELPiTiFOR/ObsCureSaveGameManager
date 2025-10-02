#ifndef FILE_IO_H
#define FILE_IO_H

#include <stddef.h>

void mkdir_p(char *path);
void copy_file(char *src, char *dst);

void free_argv(char **argv);
char *str_from_file(char *src, size_t *total_written);
char **str_to_argv(char *str, int *argc);
int file_from_string(char *dst, char *content, size_t len);

#endif // FILE_IO_H
