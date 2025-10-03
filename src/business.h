#ifndef BUSINESS_H
#define BUSINESS_H

#include <stddef.h>

int export_all_saves(char *ls);
int import_list(char *ls);

unsigned int crc_from_file(char *path, size_t skip);
int correct_checksum_backup(char *name);
int correct_checksum_save(int index);

#endif // BUSINESS_H
