#include "window_procs.h"

#include "business.h"
#include "config.h"
#include "data_access.h"
#include "gui.h"
#include "path.h"


/*
LRESULT CALLBACK ExportSingleWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        create_settings_window_elements(hwnd);
        break;
        
    case WM_COMMAND:
        if (LOWORD(wParam) == CLOSE_SETTINGS_BUTTON_ID)
        {
            DestroyWindow(hwnd);
        }
        else if (LOWORD(wParam) == BROWSE_SAVES_BUTTON_ID)
        {
            char ruta[MAX_PATH] = {0};
            if (SelectFolder(hwnd, ruta, MAX_PATH))
            {
                SetDlgItemText(hwnd, PATH_SAVES_EDIT_ID, ruta);
            }
        }
        else if (LOWORD(wParam) == BROWSE_BACKUPS_BUTTON_ID)
        {
            char ruta[MAX_PATH] = {0};
            if (SelectFolder(hwnd, ruta, MAX_PATH))
            {
                SetDlgItemText(hwnd, PATH_BACKUPS_EDIT_ID, ruta);
            }
        }
        else if (LOWORD(wParam) == APPLY_SETTINGS_BUTTON_ID)
        {
            char saves[512];
            memset(saves, 0, 512);
            GetDlgItemText(hwnd, PATH_SAVES_EDIT_ID, saves, sizeof(saves));
            char backups[512];
            memset(backups, 0, 512);
            GetDlgItemText(hwnd, PATH_BACKUPS_EDIT_ID, backups, sizeof(backups));

            config_set("path_backups", backups);
            config_set("path_saves", saves);
            set_path_backups(backups);
            set_path_saves(saves);
        }
        else if (LOWORD(wParam) == WM_ERASEBKGND)
        {
            //
        }
        break;
        
    case WM_DESTROY:
        break;
        
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
*/

LRESULT CALLBACK ExportSingleWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        create_export_single_window_elements(hwnd);
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == EXPORT_SINGLE_BUTTON2_ID)
        {
            char name[256];
            memset(name, 0, 256);
            GetDlgItemText(hwnd, EXPORT_SINGLE_NAME_INPUT2_ID, name, sizeof(name));

            if (!selected_save)
            {
                MessageBox(hwnd, "Please select a saved game first.\n", "Error: wrong argument", MB_OK);
            }
            else if (strcmp(name, "") == 0)
            {
                MessageBox(hwnd, "Please enter a name for the savegame\n", "Error: wrong argument", MB_OK);
            }

            export_save(selected_save->index, name);
            refresh_buttons();
            selected_save = NULL;
            memset(name, 0, 256);
            SetDlgItemText(hwnd, EXPORT_SINGLE_NAME_INPUT2_ID, name);
            DestroyWindow(hwnd);
        }
        break;
    case WM_DESTROY:
        selected_save = NULL;
        break;
        
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    //return 0;
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK SettingsWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        create_settings_window_elements(hwnd);
        break;
        
    case WM_COMMAND:
        if (LOWORD(wParam) == CLOSE_SETTINGS_BUTTON_ID)
        {
            DestroyWindow(hwnd);
        }
        else if (LOWORD(wParam) == BROWSE_SAVES_BUTTON_ID)
        {
            char ruta[MAX_PATH] = {0};
            if (SelectFolder(hwnd, ruta, MAX_PATH))
            {
                SetDlgItemText(hwnd, PATH_SAVES_EDIT_ID, ruta);
            }
        }
        else if (LOWORD(wParam) == BROWSE_BACKUPS_BUTTON_ID)
        {
            char ruta[MAX_PATH] = {0};
            if (SelectFolder(hwnd, ruta, MAX_PATH))
            {
                SetDlgItemText(hwnd, PATH_BACKUPS_EDIT_ID, ruta);
            }
        }
        else if (LOWORD(wParam) == APPLY_SETTINGS_BUTTON_ID)
        {
            char saves[512];
            memset(saves, 0, 512);
            GetDlgItemText(hwnd, PATH_SAVES_EDIT_ID, saves, sizeof(saves));
            char backups[512];
            memset(backups, 0, 512);
            GetDlgItemText(hwnd, PATH_BACKUPS_EDIT_ID, backups, sizeof(backups));

            config_set("path_backups", backups);
            config_set("path_saves", saves);
            set_path_backups(backups);
            set_path_saves(saves);
            refresh_buttons();
        }
        else if (LOWORD(wParam) == WM_ERASEBKGND)
        {
            //
        }
        break;
        
    case WM_DESTROY:
        break;
        
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
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
        else
        {
            check_backups_buttons(wParam);
            check_saves_buttons(wParam, hwnd);
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