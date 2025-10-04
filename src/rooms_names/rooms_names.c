#include "rooms_names.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#include "my_string.h"
#include "utils.h"

int check_line_id(char *content, char *id_c, unsigned char id)
{
    size_t i = 0;
    while (content[i] && content[i] != ',' && id_c[i] && content[i] == id_c[i])
    {
        i++;
    }

    return content[i] && !id_c[i];
}

char *room_name_from_hex_id(unsigned char id)
{
    char *content = str_from_file(".\\resources\\Room IDs.csv", NULL);
    if (!content)
    {
        fprintf(stderr, "ERROR: Couldn't str_from_fiel Room IDs.csv\n");
        return NULL;
    }

    char aux[512];
    arfillzeros(aux, 512);

    char id_c[32];
    arfillzeros(id_c, 32);
    sprintf(id_c, "0x%02X", id);
    //printf("id_c = %s | id = %X\n", id_c, id);

    size_t line = 0;
    size_t i = 0;


    while (content[i] && content[i] != '\r')
    {
        i++;
    }
    
    i += 2;

    while (content[i] && !check_line_id(content + i, id_c, id))
    {
        while (content[i] && content[i] != '\r')
        {
            i++;
        }

        if (!content[i])
        {
            fprintf(stderr, "ERROR: room not found in csv.\n");
            return NULL;
        }

        i += 2;
    }

    if (!content[i])
    {
        fprintf(stderr, "ERROR: room not found in csv.\n");
        return NULL;
    }

    for (size_t j = 0; j < 3; j++)
    {
        while (content[i] && content[i] != ',')
        {
            i++;
        }

        if (!content[i])
        {
            fprintf(stderr, "ERROR: room not found in csv.\n");
            return NULL;
        }

        i++;
    }

    free(content);
    return duplicate_string_until(content + i, ',', NULL);
}