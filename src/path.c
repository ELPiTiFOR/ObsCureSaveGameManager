#include "path.h"

#include <string.h>

size_t path_saves_len = 61;
size_t path_backups_len = 48;
                  //0123456789012345678901234567890123456789012345678901234567890123456789
char path_saves[] = "E:\\Archivos de programa (x86)\\Steam\\userdata\\449564145\\254460\\remote";
                    //0123456789012345678901234567890123456789012345678901234567890123456789
char path_backups[] = "E:\\Fran\\obscure\\Game Data\\ObsCureSaveGameManager";

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
