#include <objbase.h>
#include <shlobj.h>
#include <shobjidl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "business.h"
#include "config.h"
#include "data_access.h"
#include "gui.h"
#include "path.h"
#include "utils.h"
#include "window_procs.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    setup_paths();
    arfillzeros(exported_name, 512);

    thisHInstance = hInstance;

    HICON hIcon = LoadImage(
        NULL,
        "icon_ocsgm.ico",
        IMAGE_ICON,
        0, 0,
        LR_LOADFROMFILE | LR_DEFAULTSIZE
    );

    // registering main window class
    WNDCLASS wc = {0};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = MAIN_CLASS_NAME;
    wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    if (hIcon != NULL) {
        wc.hIcon = hIcon;
        //wc.hIconSm = hIcon;
    }
    RegisterClass(&wc);

    // registering settings window class
    WNDCLASS settings_wc = {0};
    settings_wc.lpfnWndProc = SettingsWindowProc;
    settings_wc.hInstance = hInstance;
    settings_wc.lpszClassName = SETTINGS_CLASS_NAME;
    settings_wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    settings_wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    RegisterClass(&settings_wc);

    // registering export single window class
    WNDCLASS export_single_wc = {0};
    export_single_wc.lpfnWndProc = ExportSingleWindowProc;
    export_single_wc.hInstance = hInstance;
    export_single_wc.lpszClassName = EXPORT_SINGLE_CLASS_NAME;
    export_single_wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    export_single_wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    RegisterClass(&export_single_wc);
    
    // the window
    HWND hwnd = CreateWindowEx(
        0,
        //WS_EX_TOPMOST,
        MAIN_CLASS_NAME,
        "ObsCure Save Game Manager",
        //WS_OVERLAPPEDWINDOW,
        0 | WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, // | WS_VSCROLL,
        //WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        /*
        800,
        600,
        */
        1100,
        600,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    thisHwnd = hwnd;

    // buttons, etc of the main window
    create_main_window_elements(hwnd, hInstance);

    ShowWindow(hwnd, nCmdShow);
    
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}