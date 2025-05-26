#ifndef PATH_H
#define PATH_H

#include <stddef.h>

extern size_t path_saves_len;
extern size_t path_backups_len;
extern char path_saves[512];
extern char path_backups[512];

void add_step_to_path(char *path, char *step);
void remove_step_from_path(char *path);

#endif // PATH_H
