#include "gui.h"

#include "path.h"

const char MAIN_CLASS_NAME[] = "MainWindowClass";
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
        600, 300,
        parentHwnd,
        NULL,
        hInstance,
        NULL
    );
    
    if (settingsHwnd)
    {
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

    CreateWindow("BUTTON", "Browse...",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10, 70, 100, 30, hwnd, (HMENU)BROWSE_SAVES_BUTTON_ID, NULL /*hInstance*/, NULL);


    CreateWindow("STATIC", "Path where savegames are stored", 
        WS_VISIBLE | WS_CHILD,
        120, 80, 250, 20, hwnd, NULL, NULL, NULL);
    
    CreateWindow("EDIT", get_path_saves(),
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        10, 110, 560, 25, hwnd, (HMENU)PATH_SAVES_EDIT_ID, NULL /*hInstance*/, NULL);

    CreateWindow("BUTTON", "Browse...",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10, 160, 100, 30, hwnd, (HMENU)BROWSE_BACKUPS_BUTTON_ID, NULL /*hInstance*/, NULL);
    
    CreateWindow("STATIC", "Path you want to save your backups", 
        WS_VISIBLE | WS_CHILD,
        120, 170, 260, 20, hwnd, NULL, NULL, NULL);

    CreateWindow("EDIT", get_path_backups(),
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        10, 200, 560, 25, hwnd, (HMENU)PATH_BACKUPS_EDIT_ID, NULL /*hInstance*/, NULL);


    CreateWindow("BUTTON", "Apply",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        100, 10, 80, 30, hwnd, (HMENU)APPLY_SETTINGS_BUTTON_ID, NULL, NULL);
}

void create_main_window_elements(HWND hwnd, HINSTANCE hInstance)
{
    CreateWindow("BUTTON", "Settings", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 720, 20, 60, 60, hwnd, (HMENU)SETTINGS_BUTTON_ID, hInstance, NULL);
    
    CreateWindow("STATIC", "ObsCure Save Game Manager", 
        WS_VISIBLE | WS_CHILD,
        20, 20, 210, 20, hwnd, NULL, hInstance, NULL);

    // Button - Export single
    CreateWindow("BUTTON", "Export Single", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 20, 200, 100, 100, hwnd, (HMENU)EXPORT_SINGLE_BUTTON_ID, hInstance, NULL);

    // Text input - Number - Export single
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                 140, 220, 20, 20, hwnd, (HMENU)EXPORT_SINGLE_NUMBER_INPUT_ID, hInstance, NULL);

    // Text input - Name - Export single
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                 140, 260, 200, 20, hwnd, (HMENU)EXPORT_SINGLE_NAME_INPUT_ID, hInstance, NULL);

    CreateWindow("BUTTON", "Export All", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 20, 400, 100, 100, hwnd, (HMENU)EXPORT_ALL_BUTTON_ID, hInstance, NULL);

    // Text input - Name - Export All
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                 140, 440, 200, 20, hwnd, (HMENU)EXPORT_ALL_NAME_INPUT_ID, hInstance, NULL);

    CreateWindow("BUTTON", "Import Single", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 420, 200, 100, 100, hwnd, (HMENU)IMPORT_SINGLE_BUTTON_ID, hInstance, NULL);

    // Text input - Number - Import single
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                 540, 220, 20, 20, hwnd, (HMENU)IMPORT_SINGLE_NUMBER_INPUT_ID, hInstance, NULL);

    // Text input - Name - Import single
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                 540, 260, 200, 20, hwnd, (HMENU)IMPORT_SINGLE_NAME_INPUT_ID, hInstance, NULL);

    CreateWindow("BUTTON", "Import All", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 420, 400, 100, 100, hwnd, (HMENU)IMPORT_ALL_BUTTON_ID, hInstance, NULL);

    // Text input - Name - Export All
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                 540, 440, 200, 20, hwnd, (HMENU)IMPORT_ALL_NAME_INPUT_ID, hInstance, NULL);
}