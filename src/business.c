#include "business.h"

#include <stdio.h>
#include <string.h>

#include "data_access.h"
#include "file_io.h"
#include "my_crc.h"
#include "path.h"
#include "utils.h"

int export_all_saves(char *ls)
{
    char save[64];
    arfillzeros(save, 64);
    size_t len = strlen(ls);
    strcat(save, ls);
    strcat(save, "_game0");

    for (int i = 0; i < 10; i++)
    {
        save[len + 1 + 4] = '0' + i;
        export_save(i, save);
        add_save_to_list(save, ls);
    }

    return 1;
}

int import_list(char *ls)
{
    char save_list[64];
    arfillzeros(save_list, 64);
    strcat(save_list, ls);
    strcat(save_list, ".ls");

    add_step_to_path(path_backups, save_list);

    FILE *list_file = fopen(path_backups, "rb");
    if (!list_file)
    {
        printf("ERROR: Couldn't open %s\n", path_backups);
        return 1;
    }

    char *list_content = str_from_file(path_backups, NULL);
    int argc;
    char **saves = str_to_argv(list_content, &argc);

    if (!saves)
    {
        printf("ERROR: Couldn't take saves fro list\n");
        fclose(list_file);
        return 1;
    }
    remove_step_from_path(path_backups);

    for (size_t i = 0; i < argc; i++)
    {
        import_save(i, saves[i]);
    }

    free_argv(saves);
    fclose(list_file);
    return 0;
}

unsigned int crc_from_file(char *path, size_t skip)
{
    size_t written = 0;
    unsigned char *content = str_from_file(path, &written);
    if (!content)
    {
        fprintf(stderr, "ERROR: Couldn't retrieve content from file %s\n", path);
        return 1;
    }

    //printf("CRC = %x | written = %zu\n", crc);
    return crc32(content + skip, written - skip);
}

static int correct_checksum(char *path)
{
    unsigned int crc = crc_from_file(path, 4);
    if (overwrite_crc(path, crc))
    {
        fprintf(stderr, "ERROR: Couldn't overwrite crc in file %s\n", path);
        return 1;
    }

    return 0;
}

// name doesn't end in ".sav"
int correct_checksum_backup(char *name)
{
    char filename[512];
    arfillzeros(filename, 512);
    strcpy(filename, name);
    strcat(filename, ".sav");

    add_step_to_path(path_backups, filename);
    int res = correct_checksum(path_backups);
    remove_step_from_path(path_backups);
    return res;
}

// index belongs to 0-9
int correct_checksum_save(int index)
{
    char filename[512];
    char num[] = {'0', 0};
    num[0] += index;
    arfillzeros(filename, 512);
    strcpy(filename, "game");
    strcat(filename, num);
    strcat(filename, ".sav");

    add_step_to_path(path_saves, filename);
    int res = correct_checksum(path_saves);
    remove_step_from_path(path_saves);
    return res;
}
