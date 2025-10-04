#ifndef SAVE_GAME_LIST_H
#define SAVE_GAME_LIST_H

#include <windows.h>

typedef struct save_game_list
{
    struct save_game_list *next;
    char *filename;
    int button_id;
    HWND hwnd;
    int index;
} save_game_list;

save_game_list *make_sgl(save_game_list *next, char *filename, int button_id,
    HWND hwnd, int index);
save_game_list *make_sgl_sentinel(void);
void free_destroy_sgl(save_game_list *sgl);

#endif /* !SAVE_GAME_LIST_H */