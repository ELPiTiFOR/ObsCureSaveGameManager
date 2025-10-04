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
        strcat(msg, " / *SPECIAL MODE*");
    }

    return duplicate_string(msg);
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

    free(content);
    return res;
}