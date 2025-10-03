#ifndef SAVE_DATA_H
#define SAVE_DATA_H

#include <stddef.h>

#define CRC_OFFSET 0
#define CRC_LEN 4
#define IDX_OFFSET 5
#define IDX_LEN 4
#define ROOM_OFFSET 9
#define ROOM_LEN 1
#define NB_SAVES_OFFSET 14
#define NB_SAVES_LEN 1
#define DIFF_MODE_OFFSET 17
#define DIFF_MODE_LEN 1
#define TIME_OFFSET 11
#define TIME_LEN 3

#define EASY 1
#define NORMAL 2
#define HARD 4
#define SPECIAL_MODE 8

typedef struct
{
    unsigned int crc;
    int index;
    unsigned char room;
    unsigned char nb_saves;
    unsigned char diff_mode;
    unsigned int time;
} save_data;

unsigned int read_crc(char *content, size_t len);
int read_index(char *content, size_t len);
unsigned char read_room(char *content, size_t len);
unsigned char read_nb_saves(char *content, size_t len);
unsigned char read_diff_mode(char *content, size_t len);
unsigned int read_time(char *content, size_t len);

char *save_data_to_string(save_data *save);
save_data *parse_save(char *path);

#endif /* !SAVE_DATA_H */