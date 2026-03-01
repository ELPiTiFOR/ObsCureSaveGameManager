#include "main_window.h"

#include "business.h"
#include "config.h"
#include "data_access.h"
#include "export_single_window.h"
#include "my_string.h"
#include "path.h"
#include "settings_window.h"
#include "utils.h"

#define BACKUPS_BUTTONS_IDS_START 1000
#define SAVES_BUTTONS_IDS_START 2000

#define BACKUPS_REFRESH_BUTTONS_IDS_START 1100
#define SAVES_REFRESH_BUTTONS_IDS_START 2100

#define SLOT_WIDTH 560
#define SAVE_SLOT_WIDTH 440
#define SLOT_HEIGHT 25
#define SLOTS_GAP_TOP 120
#define SAVE_SLOTS_GAP_LEFT 500

const char MAIN_CLASS_NAME[] = "MainWindowClass";

HWND thisHwnd;
HINSTANCE thisHInstance;

int current_list_offset = 0;
int entries_per_page = 10;

// TODO: free the save_game_list
save_game_list *backups_list = NULL;
save_game_list *saves_list = NULL;

save_game_list *selected_backup = NULL;
save_game_list *selected_save = NULL;

HWND selectedBackupTextHwnd;

char exported_name[512];

void create_backups_save_game_list(HWND hwnd, HINSTANCE hInstance)
{
    // backups buttons
    WIN32_FIND_DATA FindFileData;
    add_step_to_path(path_backups, "*.sav");
    HANDLE hFind = FindFirstFile(path_backups, &FindFileData);
    remove_step_from_path(path_backups);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return;
    }

    save_game_list *p = make_sgl_sentinel();
    if (!p)
    {
        fprintf(stderr, "ERROR: Couldn't create save_game_list sentinel\n");
    }

    backups_list = p;

    int i = 0;
    int j = 0;
    /*
    ** TODO: we search the files each time from the start, which makes the
    ** program very slow when there's a lot of backups and we check the last
    ** pages
    */
    do
    {
        if (i < current_list_offset || i >= current_list_offset + entries_per_page)
        {
            i++;
            continue;
        }

        char filename_noext[512];
        arfillzeros(filename_noext, 512);
        strcpy(filename_noext, FindFileData.cFileName);
        remove_last(filename_noext, ".sav");

        add_step_to_path(path_backups, FindFileData.cFileName);
        char *save_info = get_save_info(path_backups);

        char whole_info[1024];
        arfillzeros(whole_info, 1024);
        strcpy(whole_info, filename_noext);
        strcat(whole_info, " / ");
        strcat(whole_info, save_info);

        HWND buttonHwnd = CreateWindow("BUTTON", whole_info, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    20, SLOTS_GAP_TOP + j * 35, SLOT_WIDTH, SLOT_HEIGHT, hwnd, (HMENU) (i + BACKUPS_BUTTONS_IDS_START), hInstance, NULL);

        HWND refreshButtonHwnd = CreateWindow("BUTTON", "C", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    SLOT_WIDTH + 25, SLOTS_GAP_TOP + j * 35, SLOT_HEIGHT, SLOT_HEIGHT, hwnd, (HMENU) (i + BACKUPS_REFRESH_BUTTONS_IDS_START), hInstance, NULL);

        remove_step_from_path(path_backups);

        save_game_list *next_sgl = make_sgl(NULL, duplicate_string(filename_noext), i + BACKUPS_BUTTONS_IDS_START, buttonHwnd, -1,
            i + BACKUPS_REFRESH_BUTTONS_IDS_START, refreshButtonHwnd);

        p->next = next_sgl;
        p = next_sgl;

        i++;
        j++;
    } while (FindNextFile(hFind, &FindFileData));

    FindClose(hFind);
}

void create_saves_save_game_list(HWND hwnd, HINSTANCE hInstance)
{
    // backups buttons
    WIN32_FIND_DATA FindFileData;
    add_step_to_path(path_saves, "game?.sav");
    HANDLE hFind = FindFirstFile(path_saves, &FindFileData);
    remove_step_from_path(path_saves);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return;
    }

    save_game_list *p = make_sgl_sentinel();
    if (!p)
    {
        fprintf(stderr, "ERROR: Couldn't create save_game_list sentinel\n");
    }

    saves_list = p;

    int j = 0;
    do
    {
        add_step_to_path(path_saves, FindFileData.cFileName);
        char info_and_index[512];
        arfillzeros(info_and_index, 512);
        sprintf(info_and_index, "%02d. ", j + 1);

        char *save_info = get_save_info(path_saves);
        strcat(info_and_index, save_info);
        HWND buttonHwnd = CreateWindow("BUTTON", info_and_index, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    SLOT_WIDTH + 60, SLOTS_GAP_TOP + j * 35, SAVE_SLOT_WIDTH, SLOT_HEIGHT, hwnd, (HMENU) (j + SAVES_BUTTONS_IDS_START), hInstance, NULL);

        HWND refreshButtonHwnd = CreateWindow("BUTTON", "C", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    SLOT_WIDTH + SAVE_SLOT_WIDTH + 65, SLOTS_GAP_TOP + j * 35, SLOT_HEIGHT, SLOT_HEIGHT, hwnd, (HMENU) (j + SAVES_REFRESH_BUTTONS_IDS_START), hInstance, NULL);

        remove_step_from_path(path_saves);

        char filename_noext[512];
        arfillzeros(filename_noext, 512);
        strcpy(filename_noext, FindFileData.cFileName);
        remove_last(filename_noext, ".sav");

        save_game_list *next_sgl = make_sgl(NULL, duplicate_string(filename_noext), j + SAVES_BUTTONS_IDS_START, buttonHwnd, j,
            j + SAVES_REFRESH_BUTTONS_IDS_START, refreshButtonHwnd);
        p->next = next_sgl;
        p = next_sgl;

        j++;
    } while (FindNextFile(hFind, &FindFileData));

    FindClose(hFind);
}

void create_main_window_elements(HWND hwnd, HINSTANCE hInstance)
{
    create_backups_save_game_list(hwnd, hInstance);
    create_saves_save_game_list(hwnd, hInstance);

    CreateWindow("BUTTON", "<<", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 20, 20, 40, 40, hwnd, (HMENU)PRE_PAGE_BUTTON_ID, hInstance, NULL);

    CreateWindow("BUTTON", ">>", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 80, 20, 40, 40, hwnd, (HMENU)POST_PAGE_BUTTON_ID, hInstance, NULL);

    CreateWindow("BUTTON", "Settings", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 1115 - 60 - 30 /*720*/, 20, 60, 60, hwnd, (HMENU)SETTINGS_BUTTON_ID, hInstance, NULL);

    CreateWindow("BUTTON", "Refresh", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 1115 - 60 - 30 /*720*/, 90, 60, 20, hwnd, (HMENU)REFRESH_BUTTON_ID, hInstance, NULL);


    CreateWindow("BUTTON", "Export All", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 SLOT_WIDTH + 60 /*420*/, 490, 100, 20, hwnd, (HMENU)EXPORT_ALL_BUTTON_ID, hInstance, NULL);

    // Text input - Name - Export All
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                 SLOT_WIDTH + 60 + 120 /*540*/, 490, 200, 20, hwnd, (HMENU)EXPORT_ALL_NAME_INPUT_ID, hInstance, NULL);

    CreateWindow("BUTTON", "Import All", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 20, 490, 100, 20, hwnd, (HMENU)IMPORT_ALL_BUTTON_ID, hInstance, NULL);

    // Text input - Name - Export All
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                 140, 490, 200, 20, hwnd, (HMENU)IMPORT_ALL_NAME_INPUT_ID, hInstance, NULL);

    CreateWindow("BUTTON", "X", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 20, 80, 20, 20, hwnd, (HMENU)DESELECT_BACKUP_BUTTON_ID, hInstance, NULL);

    selectedBackupTextHwnd = CreateWindow("STATIC", "", 
        WS_VISIBLE | WS_CHILD,
        50, 80, 420, 20, hwnd, (HMENU)SELECTED_BACKUP_TEXT, hInstance, NULL);
}

// up = 1 if scrolling up, otherwise up = 0
// TODO: are the arguments really needed? They are global variables
void scroll_backups(HWND hwnd, HINSTANCE hInstance, int up)
{
    //
    free_destroy_sgl(backups_list);

    // TODO: why making sentinel?
    save_game_list *p = make_sgl_sentinel();
    if (!p)
    {
        fprintf(stderr, "ERROR: Couldn't create save_game_list sentinel\n");
    }

    backups_list = p;

    if (up)
    {
        if (current_list_offset != 0)
        {
            current_list_offset -= entries_per_page;
        }
    }
    else
    {
        current_list_offset += entries_per_page;
    }

    create_backups_save_game_list(hwnd, hInstance);
}

void refresh_buttons(void)
{
    free_destroy_sgl(backups_list);
    free_destroy_sgl(saves_list);

    // TODO: why making sentinels?
    backups_list = make_sgl_sentinel();
    saves_list = make_sgl_sentinel();

    create_backups_save_game_list(thisHwnd, thisHInstance);
    create_saves_save_game_list(thisHwnd, thisHInstance);
}

void check_backups_buttons(WPARAM wParam)
{
    save_game_list *p = backups_list->next;
    if (!p)
    {
        fprintf(stderr, "ERROR: backups_list->next == NULL\n");
    }

    while (p)
    {
        if (LOWORD(wParam) == p->button_id)
        {
            selected_backup = p;
            char new_text[1024];
            arfillzeros(new_text, 1024);
            sprintf(new_text, "Selected backup: %s", selected_backup->filename);

            printf("Selected backup: %s\n", selected_backup->filename);
            SetWindowText(selectedBackupTextHwnd, new_text);
            return;
        }

        p = p->next;
    }
}

void check_saves_buttons(WPARAM wParam, HWND hwnd)
{
    save_game_list *p = saves_list->next;
    if (!p)
    {
        fprintf(stderr, "ERROR: saves_list->next == NULL\n");
    }

    while (p)
    {
        if (LOWORD(wParam) == p->button_id)
        {
            if (selected_backup)
            {
                // now we overwrite the selected save with selected backup
                printf("Selected backup: %s | selected save: %d\n", selected_backup->filename, p->index);
                import_save(p->index, selected_backup->filename);
                refresh_buttons();
                selected_backup = NULL;
                SetWindowText(selectedBackupTextHwnd, "");
            }
            else
            {
                selected_save = p;
                printf("opening\n");
                OpenExportSingleWindow(hwnd);
                printf("after\n");
                //OpenSettingsWindow(thisHwnd);
            }
            return;
        }

        p = p->next;
    }
}

void check_saves_refresh_buttons(WPARAM wParam)
{
    save_game_list *p = saves_list->next;
    if (!p)
    {
        fprintf(stderr, "ERROR: saves_list->next == NULL\n");
    }

    while (p)
    {
        if (LOWORD(wParam) == p->refresh_button_id)
        {
            correct_checksum_save(p->index);
        }

        p = p->next;
    }
}

void check_backups_refresh_buttons(WPARAM wParam)
{
    save_game_list *p = backups_list->next;
    if (!p)
    {
        fprintf(stderr, "ERROR: saves_list->next == NULL\n");
    }

    while (p)
    {
        if (LOWORD(wParam) == p->refresh_button_id)
        {
            correct_checksum_backup(p->filename);
        }

        p = p->next;
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == EXPORT_SINGLE_BUTTON_ID)
        {
            char name[256];
            memset(name, 0, 256);
            GetDlgItemText(hwnd, EXPORT_SINGLE_NAME_INPUT_ID, name, sizeof(name));
            char number[256];
            memset(number, 0, 256);
            GetDlgItemText(hwnd, EXPORT_SINGLE_NUMBER_INPUT_ID, number, sizeof(number));

            if (strcmp(name, "") == 0)
            {
                MessageBox(hwnd, "Please enter a name for the savegame\n", "Error: wrong argument", MB_OK);
            }

            if (export_save(atoi(number) - 1, name) != 0)
            {
                MessageBox(hwnd, "That number isn't valid, please enter a number between 1 and 10\n", "Error: wrong argument", MB_OK);
            }

            refresh_buttons();
        }
        else if (LOWORD(wParam) == EXPORT_ALL_BUTTON_ID)
        {
            char name[256];
            memset(name, 0, 256);
            GetDlgItemText(hwnd, EXPORT_ALL_NAME_INPUT_ID, name, sizeof(name));

            if (strcmp(name, "") == 0)
            {
                MessageBox(hwnd, "Please enter a name for the savegame\n", "Error: wrong argument", MB_OK);
            }

            export_all_saves(name);
            refresh_buttons();
        }
        else if (LOWORD(wParam) == IMPORT_SINGLE_BUTTON_ID)
        {
            char name[256];
            memset(name, 0, 256);
            GetDlgItemText(hwnd, IMPORT_SINGLE_NAME_INPUT_ID, name, sizeof(name));
            char number[256];
            memset(number, 0, 256);
            GetDlgItemText(hwnd, IMPORT_SINGLE_NUMBER_INPUT_ID, number, sizeof(number));

            if (strcmp(name, "") == 0)
            {
                MessageBox(hwnd, "Please enter a name for the savegame\n", "Error: wrong argument", MB_OK);
            }

            if (import_save(atoi(number) - 1, name) != 0)
            {
                MessageBox(hwnd, "That number isn't valid, please enter a number between 1 and 10\n", "Error: wrong argument", MB_OK);
            }
            refresh_buttons();
        }
        else if (LOWORD(wParam) == IMPORT_ALL_BUTTON_ID)
        {
            char name[256];
            memset(name, 0, 256);
            GetDlgItemText(hwnd, IMPORT_ALL_NAME_INPUT_ID, name, sizeof(name));

            if (strcmp(name, "") == 0)
            {
                MessageBox(hwnd, "Please enter a name for the savegame\n", "Error: wrong argument", MB_OK);
            }

            if (import_list(name) != 0)
            {
                printf("That didn't work\n");
            }
            refresh_buttons();
        }
        else if (LOWORD(wParam) == SETTINGS_BUTTON_ID)
        {
            OpenSettingsWindow(hwnd);
        }
        else if (LOWORD(wParam) == PRE_PAGE_BUTTON_ID)
        {
            scroll_backups(hwnd, thisHInstance, 1);
        }
        else if (LOWORD(wParam) == POST_PAGE_BUTTON_ID)
        {
            scroll_backups(hwnd, thisHInstance, 0);
        }
        else if (LOWORD(wParam) == DESELECT_BACKUP_BUTTON_ID)
        {
            selected_backup = NULL;
            SetWindowText(selectedBackupTextHwnd, "");
        }
        else if (LOWORD(wParam) == REFRESH_BUTTON_ID)
        {
            refresh_buttons();
        }
        else
        {
            check_backups_buttons(wParam);
            check_saves_buttons(wParam, hwnd);
            check_backups_refresh_buttons(wParam);
            check_saves_refresh_buttons(wParam);
        }
        break;
    case WM_PAINT:
            HDC     hdc;
            PAINTSTRUCT ps;
            RECT    rect;
            hdc = BeginPaint(hwnd, &ps);

            GetClientRect(hwnd, &rect);

            EndPaint(hwnd, &ps);
            return 0;
    case WM_DESTROY:
        free_config();
        PostQuitMessage(0);
        return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}