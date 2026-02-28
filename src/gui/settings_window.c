#include "settings_window.h"

#include "business.h"
#include "config.h"
#include "data_access.h"
#include "path.h"

const char SETTINGS_CLASS_NAME[] = "SettingsWindowClass";

void OpenSettingsWindow(HWND parentHwnd)
{
    HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(parentHwnd, GWLP_HINSTANCE);
    
    HWND settingsHwnd = CreateWindowEx(
        0,
        SETTINGS_CLASS_NAME,
        "Settings | ObsCure Save Game Manager",
        0 | WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        600, 370,
        parentHwnd,
        NULL,
        hInstance,
        NULL
    );
    
    if (settingsHwnd)
    {
        printf("dentro open settings\n");
        ShowWindow(settingsHwnd, SW_SHOW);
        UpdateWindow(settingsHwnd);
    }
}

int SelectFolder(HWND hwnd, char *path, int size)
{
    CoInitialize(NULL);
    
    IFileDialog *pfd;
    if (SUCCEEDED(CoCreateInstance(&CLSID_FileOpenDialog, NULL, 
                                  CLSCTX_INPROC_SERVER, &IID_IFileOpenDialog, 
                                  (void **)&pfd)))
    {
        DWORD dwOptions;
        pfd->lpVtbl->GetOptions(pfd, &dwOptions);
        pfd->lpVtbl->SetOptions(pfd, dwOptions | FOS_PICKFOLDERS);
        
        pfd->lpVtbl->SetTitle(pfd, L"Selecciona una carpeta");
        
        if (SUCCEEDED(pfd->lpVtbl->Show(pfd, hwnd)))
        {
            IShellItem *psi;
            if (SUCCEEDED(pfd->lpVtbl->GetResult(pfd, &psi)))
            {
                PWSTR pszPath;
                if (SUCCEEDED(psi->lpVtbl->GetDisplayName(psi, SIGDN_FILESYSPATH, &pszPath)))
                {
                    WideCharToMultiByte(CP_UTF8, 0, pszPath, -1, path, size, NULL, NULL);
                    CoTaskMemFree(pszPath);
                    psi->lpVtbl->Release(psi);
                    pfd->lpVtbl->Release(pfd);
                    CoUninitialize();
                    return TRUE;
                }
                psi->lpVtbl->Release(psi);
            }
        }
        pfd->lpVtbl->Release(pfd);
    }
    CoUninitialize();
    return FALSE;
}

void create_settings_window_elements(HWND hwnd)
{
    CreateWindow("BUTTON", "Close",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10, 10, 80, 30, hwnd, (HMENU)CLOSE_SETTINGS_BUTTON_ID, NULL, NULL);

    /* PATH_SAVES */
    CreateWindow("BUTTON", "Browse...",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10, 70, 100, 30, hwnd, (HMENU)BROWSE_SAVES_BUTTON_ID, NULL /*hInstance*/, NULL);


    CreateWindow("STATIC", "Path where savegames are stored", 
        WS_VISIBLE | WS_CHILD,
        120, 80, 250, 20, hwnd, NULL, NULL, NULL);
    
    CreateWindow("EDIT", get_path_saves(),
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        10, 110, 560, 25, hwnd, (HMENU)PATH_SAVES_EDIT_ID, NULL /*hInstance*/, NULL);

    /* PATH_BACKUPS */
    CreateWindow("BUTTON", "Browse...",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10, 160, 100, 30, hwnd, (HMENU)BROWSE_BACKUPS_BUTTON_ID, NULL /*hInstance*/, NULL);
    
    CreateWindow("STATIC", "Path you want to save your backups", 
        WS_VISIBLE | WS_CHILD,
        120, 170, 260, 20, hwnd, NULL, NULL, NULL);

    CreateWindow("EDIT", get_path_backups(),
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        10, 200, 560, 25, hwnd, (HMENU)PATH_BACKUPS_EDIT_ID, NULL /*hInstance*/, NULL);

    /* PATH_GAME */
    CreateWindow("BUTTON", "Browse...",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10, 250, 100, 30, hwnd, (HMENU)BROWSE_GAME_BUTTON_ID, NULL /*hInstance*/, NULL);
    
    CreateWindow("STATIC", "Path where your game is installed", 
        WS_VISIBLE | WS_CHILD,
        120, 260, 260, 20, hwnd, NULL, NULL, NULL);

    CreateWindow("EDIT", get_path_game(),
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        10, 290, 560, 25, hwnd, (HMENU)PATH_GAME_EDIT_ID, NULL /*hInstance*/, NULL);



    CreateWindow("BUTTON", "Apply",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        100, 10, 80, 30, hwnd, (HMENU)APPLY_SETTINGS_BUTTON_ID, NULL, NULL);
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
        else if (LOWORD(wParam) == BROWSE_GAME_BUTTON_ID)
        {
            char ruta[MAX_PATH] = {0};
            if (SelectFolder(hwnd, ruta, MAX_PATH))
            {
                SetDlgItemText(hwnd, PATH_GAME_EDIT_ID, ruta);
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
            char game[512];
            memset(game, 0, 512);
            GetDlgItemText(hwnd, PATH_GAME_EDIT_ID, game, sizeof(game));

            config_set("path_backups", backups);
            config_set("path_saves", saves);
            config_set("path_game", game);
            set_path_backups(backups);
            set_path_saves(saves);
            set_path_game(game);
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