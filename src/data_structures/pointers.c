#include "pointers.h"

#include <stddef.h>
#include <stdlib.h>

void free_triple_char_pointer(char ***config)
{
    for (size_t i = 0; config[i]; i++)
    {
        free_argv(config[i]);
    }

    free(config);
}

// we assume argv ends with a NULL pointer
void free_argv(char **argv)
{
    size_t i = 0;
    while (argv[i])
    {
        free(argv[i]);
        i++;
    }

    free(argv);
}