#include "gui.h"

#include "business.h"
#include "data_access.h"
#include "my_string.h"
#include "path.h"
#include "save_game_list.h"
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
const char SETTINGS_CLASS_NAME[] = "SettingsWindowClass";
const char EXPORT_SINGLE_CLASS_NAME[] = "ExportSingleWindowClass";

int current_list_offset = 0;
int entries_per_page = 10;

HWND thisHwnd;
HINSTANCE thisHInstance;
HWND selectedBackupTextHwnd;

// TODO: free the save_game_list
save_game_list *backups_list = NULL;
save_game_list *saves_list = NULL;

save_game_list *selected_backup = NULL;
save_game_list *selected_save = NULL;

char exported_name[512];

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

void OpenExportSingleWindow(HWND parentHwnd)
{
    HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(parentHwnd, GWLP_HINSTANCE);
    
    HWND exportSingleHwnd = CreateWindowEx(
        0,
        EXPORT_SINGLE_CLASS_NAME,
        "Export Single | ObsCure Save Game Manager",
        0 | WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        320, 90,
        parentHwnd,
        NULL,
        hInstance,
        NULL
    );

    if (!exportSingleHwnd)
    {
        DWORD error = GetLastError();
        
        char errorMsg[256];
        sprintf(errorMsg, "CreateWindowEx falló. Código de error: %lu", error);
        MessageBox(NULL, errorMsg, "Error", MB_ICONERROR);
        
        // Opcional: obtener descripción del error
        LPVOID lpMsgBuf;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL
        );
        
        MessageBox(NULL, (LPCTSTR)lpMsgBuf, "Error Detallado", MB_ICONERROR);
        LocalFree(lpMsgBuf);
    }
    
    if (exportSingleHwnd)
    {
        printf("dentro open export\n");
        ShowWindow(exportSingleHwnd, SW_SHOW);
        UpdateWindow(exportSingleHwnd);
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

void create_export_single_window_elements(HWND hwnd)
{
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                 100, 20, 200, 20, hwnd, (HMENU)EXPORT_SINGLE_NAME_INPUT2_ID, NULL, NULL);

    CreateWindow("BUTTON", "Export", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 20, 15, 70, 30, hwnd, (HMENU)EXPORT_SINGLE_BUTTON2_ID, NULL, NULL);
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

// up = 1 if scrolling up, otherwise up = 0
void scroll_backups(HWND hwnd, HINSTANCE hInstance, int up)
{
    //
    free_destroy_sgl(backups_list);

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

    backups_list = make_sgl_sentinel();
    saves_list = make_sgl_sentinel();

    create_backups_save_game_list(thisHwnd, thisHInstance);
    create_saves_save_game_list(thisHwnd, thisHInstance);
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

    /*
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


    CreateWindow("BUTTON", "Import Single", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 420, 200, 100, 100, hwnd, (HMENU)IMPORT_SINGLE_BUTTON_ID, hInstance, NULL);

    // Text input - Number - Import single
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                 540, 220, 20, 20, hwnd, (HMENU)IMPORT_SINGLE_NUMBER_INPUT_ID, hInstance, NULL);

    // Text input - Name - Import single
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                 540, 260, 200, 20, hwnd, (HMENU)IMPORT_SINGLE_NAME_INPUT_ID, hInstance, NULL);

    */
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