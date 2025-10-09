#include "save_data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#include "my_string.h"
#include "rooms_names.h"
#include "utils.h"

unsigned int read_crc(char *content, size_t len)
{
    if (len < CRC_OFFSET + CRC_LEN)
    {
        return 0;
    }

    void *content_v = content + CRC_OFFSET;
    unsigned int *content_i = content_v;
    return content_i[0];
}

int read_index(char *content, size_t len)
{
    if (len < IDX_OFFSET + IDX_LEN)
    {
        return 0;
    }

    void *content_v = content + IDX_OFFSET;
    unsigned int *content_i = content_v;
    return content_i[0];
}

unsigned char read_room(char *content, size_t len)
{
    if (len < ROOM_OFFSET + ROOM_LEN)
    {
        return 0;
    }

    return content[ROOM_OFFSET];
}

unsigned char read_nb_saves(char *content, size_t len)
{
    if (len < NB_SAVES_OFFSET + NB_SAVES_LEN)
    {
        return 0;
    }

    return content[NB_SAVES_OFFSET];
}

unsigned char read_diff_mode(char *content, size_t len)
{
    if (len < DIFF_MODE_OFFSET + DIFF_MODE_LEN)
    {
        return 0;
    }

    return content[DIFF_MODE_OFFSET];
}

unsigned int read_time(char *content, size_t len)
{
    if (len < TIME_OFFSET + TIME_LEN)
    {
        return 0;
    }

    void *content_v = content + TIME_OFFSET;
    unsigned int *content_i = content_v;
    return content_i[0];
}

unsigned int read_gun_ammo(char *content, size_t len, char wanted,
    char not_wanted)
{
    if (len < ITEM_INVENTORY_OFFSET + GUN_AMMO_LEN)
    {
        return 0;
    }

    // checking if there is any handgun ammo info at all
    char *gun_ammo = content + ITEM_INVENTORY_OFFSET;
    if (gun_ammo[0] == wanted)
    {
        void *gun_ammo_v = gun_ammo + 1;
        unsigned int *gun_ammo_i = gun_ammo_v;

        // we take the 2nd element because the 1st one is in MSB
        return gun_ammo_i[1];
    }
    else if (gun_ammo[0] == not_wanted)
    {
        return read_gun_ammo(content + GUN_AMMO_LEN, len, wanted,
            not_wanted);
    }
    
    return 0;
}

save_data *parse_save(char *path)
{
    size_t r = 0;
    char *content = str_from_file(path, &r);
    save_data *res = calloc(1, sizeof(save_data));
    if (!res)
    {
        return NULL;
    }

    res->crc = read_crc(content, r);
    res->index = read_index(content, r);
    res->room = read_room(content, r);
    res->nb_saves = read_nb_saves(content, r);
    res->diff_mode = read_diff_mode(content, r);
    res->time = read_time(content, r);
    res->handgun_ammo = read_gun_ammo(content, r, HANDGUN_AMMO_KEY,
        SHOTGUN_AMMO_KEY);
    res->shotgun_ammo = read_gun_ammo(content, r, SHOTGUN_AMMO_KEY,
        HANDGUN_AMMO_KEY);

    free(content);
    return res;
}

// return value must be freed
char *save_data_to_string(save_data *save)
{
    char msg[512];
    arfillzeros(msg, 512);

    int hour = 0;
    int minute = 0;
    int seconds = 0;
    int total_milli = save->time;
    int total_seconds = total_milli / 1000;

    int t = total_seconds;

    hour = t / 3600;
    t -= hour * 3600;
    minute = t / 60;
    seconds = t - minute * 60;

    char *name = room_name_from_hex_id(save->room);

    if (name)
    {
        sprintf(msg, "%s / %d / %02d:%02d:%02d / ", name, save->nb_saves, hour, minute, seconds);

        free(name);
    }
    else
    {
        sprintf(msg, "%02X / %d / %02d:%02d:%02d / ", save->room, save->nb_saves, hour, minute, seconds);
    }

    unsigned char diff = save->diff_mode;
    if (diff & EASY)
    {
        strcat(msg, "EASY");
    }
    if (diff & NORMAL)
    {
        strcat(msg, "NORMAL");
    }
    if (diff & HARD)
    {
        strcat(msg, "HARD");
    }
    if (diff & SPECIAL_MODE)
    {
        strcat(msg, " / SPECIAL");
    }

    return duplicate_string(msg);
}

void print_save_data(save_data *save, char *name)
{
    printf("save_data *(%s) = {\n", name);
    printf("    crc = %u\n", save->crc);
    printf("    index = %d\n", save->index);
    printf("    room = 0x%02X\n", save->room);
    printf("    nb_saves = %hhd\n", save->nb_saves);
    printf("    diff_mode = 0x%02X\n", save->diff_mode);
    printf("    time = %d\n", save->time);
    printf("    handgun_ammo = %d\n", save->handgun_ammo);
    printf("    shotgun_ammo = %d\n", save->shotgun_ammo);
    printf("}\n");
}