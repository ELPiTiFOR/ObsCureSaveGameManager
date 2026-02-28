#ifndef EXPORT_SINGLE_WINDOW_H
#define EXPORT_SINGLE_WINDOW_H

#include <objbase.h>
#include <shlobj.h>
#include <shobjidl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "main_window.h"

extern const char EXPORT_SINGLE_CLASS_NAME[];
void OpenExportSingleWindow(HWND parentHwnd);
void create_export_single_window_elements(HWND hwnd);
LRESULT CALLBACK ExportSingleWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif /* !EXPORT_SINGLE_WINDOW_H */