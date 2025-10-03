#include "data_access.h"

#include <stdio.h>
#include <string.h>

#include "file_io.h"
#include "path.h"
#include "utils.h"

int export_save(int n, char *name)
{
    //printf("Name: %s\n", name);
    if (n < 0 || n > 9)
    {
        return 1;
    }

    char savegame[10] = "game0.sav";
    savegame[9] = 0;
    savegame[4] = '0' + n;

    char exported[64];
    arfillzeros(exported, 64);

    strcat(exported, name);
    strcat(exported, ".sav");

    add_step_to_path(path_saves, savegame);
    add_step_to_path(path_backups, exported);
    copy_file(path_saves, path_backups);

    remove_step_from_path(path_saves);
    remove_step_from_path(path_backups);

    return 0;
}

int import_save(int n, char *name)
{
    if (n < 0 || n > 9)
    {
        return 1;
    }

    char savegame[10] = "game0.sav";
    savegame[9] = 0;
    savegame[4] = '0' + n;

    char exported[64];
    arfillzeros(exported, 64);

    strcat(exported, name);
    strcat(exported, ".sav");

    add_step_to_path(path_saves, savegame);
    add_step_to_path(path_backups, exported);
    copy_file(path_backups, path_saves);

    remove_step_from_path(path_saves);
    remove_step_from_path(path_backups);

    return 0;
}

// a list is a file with `.ls` extension
// its content is just an identifier per line and each identifier
// is the name of a savegame
// the list must have at most 10 savegames
int add_save_to_list(char *save, char *ls)
{
    char save_list[64];
    arfillzeros(save_list, 64);
    strcat(save_list, ls);
    strcat(save_list, ".ls");

    add_step_to_path(path_backups, save_list);
    FILE *list_file = fopen(path_backups, "a+b");
    if (!list_file)
    {
        printf("ERROR: couldn't open %s\n", path_backups);
        return 1;
    }

    size_t read = fwrite(save, 1, strlen(save), list_file);
    read = fwrite("\r\n", 1, 2, list_file);

    remove_step_from_path(path_backups);
    fclose(list_file);
}

int overwrite_crc(char *path, unsigned int crc)
{
    FILE *file = fopen(path, "rb+");
    if (!file)
    {
        return 1;
    }

    unsigned char towrite[4] = {0};
    void *towrite_v = towrite;
    unsigned int *towrite_u = towrite_v;

    towrite_u[0] = crc;

    //printf("towrite: %x %x %x %x\n", towrite[0], towrite[1], towrite[2], towrite[3]);

    //fseek(file, 0, SEEK_SET);
    fwrite(towrite, 1, 4, file);

    fclose(file);
    return 0;
}