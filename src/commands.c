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
        else if (strcmp(argv[i], "-C") == 0)
        {
            if (argc > i + 1)
            {
                correct_checksum_backup(argv[i + 1]);
                i += 1;
            }
            else
            {
                printf("ERROR: You must provide a name of savegame when trying to correct its CRC\n");
                return;
            }
        }
        else if (strcmp(argv[i], "-c") == 0)
        {
            if (argc > i + 1)
            {
                correct_checksum_save(atoi(argv[i + 1]));
                i += 1;
            }
            else
            {
                printf("ERROR: You must provide an index of savegame when trying to correct its CRC\n");
                return;
            }
        }
        else if (strcmp(argv[i], "--show-crc32") == 0)
        {
            if (argc > i + 2)
            {
                unsigned int crc = crc_from_file(argv[i + 1], atoi(argv[i + 2]));
                printf("CRC = %x\n", crc);
                i += 2;
            }
            else
            {
                printf("ERROR: You must provide a path of a savegame and a skip number when trying to correct its CRC\n");
                return;
            }
        }
        else if (strcmp(argv[i], "-p") == 0)
        {
            if (argc > i + 1)
            {
                print_save_info(argv[i + 1]);
                i += 1;
            }
            else
            {
                printf("ERROR: You must provide a path of a savegame when trying to print its info\n");
                return;
            }
        }
    }
}
