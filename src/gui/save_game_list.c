#include "save_game_list.h"

#include <stdio.h>

save_game_list *make_sgl(save_game_list *next, char *filename, int button_id,
    HWND hwnd, int index, int refresh_button_id, HWND refresh_hwnd)
{
    save_game_list *res = calloc(1, sizeof(save_game_list));
    if (!res)
    {
        return NULL;
    }

    res->next = next;
    res->filename = filename;
    res->button_id = button_id;
    res->hwnd = hwnd;
    res->index = index;
    res->refresh_button_id = refresh_button_id;
    res->refresh_hwnd = refresh_hwnd;
}

save_game_list *make_sgl_sentinel(void)
{
    return make_sgl(NULL, NULL, 0, (HWND)0, -1, 0, (HWND)0);
}

void free_destroy_sgl(save_game_list *sgl)
{
    if (!sgl)
    {
        return;
    }

    save_game_list *p = sgl->next;
    while (p)
    {
        //printf("Freeing %s\n", p->filename);
        free(p->filename);
        DestroyWindow(p->hwnd);
        // TODO: should I destroy refres_hwnd?
        p = p->next;
    }
}