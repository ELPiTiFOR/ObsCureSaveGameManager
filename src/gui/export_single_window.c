#include "export_single_window.h"

#include "business.h"
#include "config.h"
#include "data_access.h"
#include "path.h"

const char EXPORT_SINGLE_CLASS_NAME[] = "ExportSingleWindowClass";

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

void create_export_single_window_elements(HWND hwnd)
{
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                 100, 20, 200, 20, hwnd, (HMENU)EXPORT_SINGLE_NAME_INPUT2_ID, NULL, NULL);

    CreateWindow("BUTTON", "Export", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 20, 15, 70, 30, hwnd, (HMENU)EXPORT_SINGLE_BUTTON2_ID, NULL, NULL);
}

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