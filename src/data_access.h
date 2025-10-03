#ifndef DATA_ACCESS
#define DATA_ACCESS

int export_save(int n, char *name);
int import_save(int n, char *name);
int add_save_to_list(char *save, char *ls);
int overwrite_crc(char *path, unsigned crc);

#endif // DATA_ACCESS
