#include "utils.h"

size_t remove_file_from_path(char *path)
{
    size_t i = 0;
    while (path[i] != 0)
    {
        i++;
    }

    while(path[i] != '\\')
    {
        i--;
    }

    path[i] = 0;

    return i;
}

void arfillzeros(char *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        arr[i] = 0;
    }
}
