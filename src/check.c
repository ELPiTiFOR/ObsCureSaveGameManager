#include "check.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "path.h"
#include "utils.h"

#define SAV_LENGHT 22528
#define MOST_RARE_CHAR 0xD8

size_t offset = 0;

void print_hex_arr(unsigned char *read, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        printf("%02X ", read[i]);
    }

    puts("");
}

void print_file_content(FILE *file)
{
    unsigned char read[16];
    size_t r = 0;
    while ((r = fread(read, 1, 16, file)) != 0)
    {
        print_hex_arr(read, r);
    }

    fseek(file, 0, SEEK_SET);
}

// returns the next char of each games[i] if the char is equal.
// updates the are_equal accordingly
unsigned char get_next_equal_byte(FILE *games[], int *are_equal, int *error)
{
    size_t i = 0;
    offset++;
    *are_equal = 1;

    // first game
    unsigned char read[1];
    memset(read, 0, 1);
    size_t r = fread(read, 1, 1, games[i]);
    if (!r || r != 1)
    {
        fprintf(stderr, "ERROR %d: Couldn't read games[%d] = %p: read = { %d, %d, %d }\n", ferror(games[i]), i, games[i], read[0], read[1], read[2]);
        *are_equal = 0;
        *error = 1; // couldn't read
        return 0;
    }

    unsigned char res = read[0];
    unsigned char curr = res;

    do
    {
        i++;
        res = curr;
        r = fread(read, 1, 1, games[i]);
        curr = read[0];
        if (res != curr)
        {
            *are_equal = 0;
        }
    } while(i < 9);

    if (!*are_equal)
    {
        //printf("Failed at offset %X with games[%d] == %p | res = %02X | curr = %02X\n", offset, i, games[i], res, curr);
    }
    return res;
}

void close_streams(FILE *games[], size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        fclose(games[i]);
    }
}

int init_streams_game_n(FILE *games[])
{
    for (size_t i = 0; i < 10; i++)
    {
        add_step_to_path(path_saves, "gameN.sav");
        size_t len = strlen(path_saves);
        path_saves[len - 5] = '0' + i;

        games[i] = fopen(path_saves, "rb");
        if (!games[i])
        {
            fprintf(stderr, "ERROR: Couldn't open %s\n", path_saves);
            for (size_t j = 0; j < i; j++)
            {
                fclose(games[j]);
            }

            return 1;
        }

        remove_step_from_path(path_saves);
    }

    return 0;
}

int create_template_file(FILE *games[], size_t len)
{
    FILE *file = fopen("game.sav", "w+b");
    if (!file)
    {
        fprintf(stderr, "ERROR: Couldn't open game.sav\n");
        return 1;
    }

    int are_equal = 0;
    int error = 0;
    for (int i = 0; i < SAV_LENGHT; i++)
    {
        unsigned char to_write[1];
        to_write[0] = get_next_equal_byte(games, &are_equal, &error);

        if (error)
        {
            fprintf(stderr, "ERROR: %d\n", error);
            fclose(file);
            close_streams(games, len);
            return 1;
        }

        if (!are_equal)
        {
            to_write[0] = MOST_RARE_CHAR;
        }

        size_t r = fwrite(to_write, 1, 1, file);
        //printf("Printing for the %dth time, r = %d\n", i, r);
        if (r != 1)
        {
            //printf("Printing for the %dth time, r = %d\n", i, r);
        }
        if (!r)
        {
            fprintf(stderr, "ERROR: Couldn't read file\n");
            return 1;
        }

        if (are_equal && to_write[0] == MOST_RARE_CHAR)
        {
            printf("INFO: Writing EQUAL most rare char\n");
        }
    }

    close_streams(games, len);
    fclose(file);
}

int create_template_file_game_n(void)
{
    FILE *games[10];
    if (init_streams_game_n(games))
    {
        return 1;
    }

    int res = create_template_file(games, 10);
    close_streams(games, 10);
    return res;
}

int create_template_file_path(char *path)
{
    // TODO: dynamically extend the array
    FILE *games[32];
    memset(games, 0, 32 * sizeof(FILE *));

    char path_cpy[512];
    arfillzeros(path_cpy, 512);
    strcpy(path_cpy, path);

    WIN32_FIND_DATA FindFileData;
    add_step_to_path(path_cpy, "*.sav");
    HANDLE hFind = FindFirstFile(path_cpy, &FindFileData);
    remove_step_from_path(path_cpy);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "ERROR: FindFirstFile: hFind = INVALID_HANDLE_VALUE\n");
        return 1;
    }

    int i = 0;
    do
    {
        add_step_to_path(path_cpy, FindFileData.cFileName);
        games[i] = fopen(path_cpy, "rb");
        if (!games[i])
        {
            fprintf(stderr, "ERROR: Couldn't open %s\n", path_cpy);
            for (size_t j = 0; j < i; j++)
            {
                fclose(games[j]);
            }

            return 1;
        }

        remove_step_from_path(path_cpy);
        i++;
    } while (i < 32 && FindNextFile(hFind, &FindFileData));

    FindClose(hFind);

    int res = create_template_file(games, i);
    close_streams(games, i);
    return res;
}
