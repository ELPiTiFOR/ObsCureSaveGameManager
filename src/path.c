#include "path.h"

#include <stdio.h>
#include <string.h>

#include "config.h"

size_t path_saves_len = 61;
size_t path_backups_len = 48;
                  //0123456789012345678901234567890123456789012345678901234567890123456789
char path_saves[] = {0}; //config_get("path_saves"); //"E:\\Archivos de programa (x86)\\Steam\\userdata\\449564145\\254460\\remote";
                    //0123456789012345678901234567890123456789012345678901234567890123456789
char path_backups[] = {0}; //config_get("path_backups"); //"E:\\Fran\\obscure\\Game Data\\ObsCureSaveGameManager";
char path_game[] = {0}; //config_get("path_backups"); //"E:\\Fran\\obscure\\Game Data\\ObsCureSaveGameManager";

void setup_paths(void)
{
    strncpy(path_saves, config_get("path_saves"), 512);
    strncpy(path_backups, config_get("path_backups"), 512);
    strncpy(path_game, config_get("path_game"), 512);
}

void add_step_to_path(char *path, char *step)
{
    strcat(path, "\\");
    strcat(path, step);
}

void remove_step_from_path(char *path)
{
    size_t i = 0;
    while (i < 512 && path[i])
    {
        i++;
    }

    while (i > 1 && path[i] != '\\')
    {
        i--;
    }

    while (i < 512 && path[i])
    {
        path[i] = 0;
        i++;
    }
}

void set_path_saves(char *new_path)
{
    strncpy(path_saves, new_path, 512);
}

void set_path_backups(char *new_path)
{
    strncpy(path_backups, new_path, 512);
}

void set_path_game(char *new_path)
{
    strncpy(path_game, new_path, 512);
}

char *get_path_saves(void)
{
    return path_saves;
}

char *get_path_backups(void)
{
    return path_backups;
}

char *get_path_game(void)
{
    return path_game;
}