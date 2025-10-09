#include "allitems_reader.h"

#include <stddef.h>
#include <stdio.h>

#include "path.h"

FILE *open_allitems_file(void)
{
    add_step_to_path(path_game, "data");
    add_step_to_path(path_game, "_common");
    add_step_to_path(path_game, "allitems.it");

    FILE *res = fopen(path_game, "rb");

    remove_step_from_path(path_game);
    remove_step_from_path(path_game);
    remove_step_from_path(path_game);

    if (!res)
    {
        fprintf(stderr, "ERROR: Couldn't fopen %s\n", path_game);
    }

    return res;
}

int get_field(FILE *file, int offset)
{
    fseek(file, offset, SEEK_CUR);
    char read[4];
    size_t r = fread(read, 1, 4, file);
    fseek(file, -offset - 4, SEEK_CUR);
    if (!r)
    {
        return -1;
    }

    char aux = read[0];
    read[0] = read[3];
    read[3] = aux;

    aux = read[1];
    read[1] = read[2];
    read[2] = aux;

    void *read_v = read;
    unsigned int *read_i = read_v;
    return read_i[0];
}

int goto_next_item(FILE *allitems)
{
    return fseek(allitems, 20, SEEK_CUR);
}

int get_item_id(FILE *file)
{
    return get_field(file, ITEM_ID_OFFSET);
}

int get_location_id(FILE *file)
{
    return get_field(file, LOCATION_ID_OFFSET);
}

int get_info(FILE *file)
{
    return get_field(file, INFO_OFFSET);
}

int get_multiplier(FILE *file)
{
    return get_field(file, MULTIPLIER_OFFSET);
}

int get_diff_mode(FILE *file)
{
    return get_field(file, DIFF_MODE_ALLITEMS_OFFSET);
}

int get_item_id_by_location_id(int loc_id)
{
    FILE *file = open_allitems_file();
    if (!file)
    {
        return -1;
    }

    int location_id = get_location_id(file);
    int i = 0;
    while (location_id != -1 && location_id != loc_id)
    {
        printf("Vuelta %d | location_id = %0X8\n", i, location_id);
        i++;
        if (goto_next_item(file) != 0)
        {
            return -1;
        }

        location_id = get_location_id(file);
    }

    if (location_id == -1)
    {
        return -1;
    }

    return get_item_id(file);
}