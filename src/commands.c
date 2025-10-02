#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "business.h"
#include "data_access.h"

void execute_commands(int argc, char **argv)
{
    for (size_t i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-E") == 0)
        {
            if (argc > i + 2)
            {
                export_save(atoi(argv[i + 1]), argv[i + 2]);
                i += 2;
            }
            else
            {
                printf("ERROR: You must provide a number and a name of savegame when exporting\n");
                return;
            }
        }

        else if (strcmp(argv[i], "-I") == 0)
        {
            if (argc > i + 2)
            {
                import_save(atoi(argv[i + 1]), argv[i + 2]);
                printf("argv[i + 1] = %s | argv[i + 2] = %s\n", argv[i + 1], argv[i + 2]);
                i += 2;
            }
            else
            {
                printf("ERROR: You must provide a number and a name of savegame when importing\n");
                return;
            }
        }

        else if (strcmp(argv[i], "--export-all") == 0)
        {
            if (argc > i + 1)
            {
                export_all_saves(argv[i + 1]);
                i++;
            }
            else
            {
                printf("ERROR: You must provide a name of savegame when exporting\n");
                return;
            }
        }

        else if (strcmp(argv[i], "--import-list") == 0)
        {
            if (argc > i + 1)
            {
                import_list(argv[i + 1]);
                i++;
            }
            else
            {
                printf("ERROR: You must provide a name of savegame when exporting\n");
                return;
            }
        }

        else if (strcmp(argv[i], "-A") == 0)
        {
            if (argc > i + 2)
            {
                add_save_to_list(argv[i + 1], argv[i + 2]);
                i += 2;
            }
            else
            {
                printf("ERROR: You must provide a name of savegame and a name of list when adding a savegame\n");
                return;
            }
        }
    }
}
