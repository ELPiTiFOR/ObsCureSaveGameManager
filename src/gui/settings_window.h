#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include <objbase.h>
#include <shlobj.h>
#include <shobjidl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// this is included because some macros are defined here
// I would like to have those macros (settings related) defined here, but
// I have to find a way to not repeat IDs, which is hard to do among several
// files
#include "main_window.h"

extern const char SETTINGS_CLASS_NAME[];

void OpenSettingsWindow(HWND parentHwnd);
int SelectFolder(HWND hwnd, char *path, int size);
void create_settings_window_elements(HWND hwnd);

LRESULT CALLBACK SettingsWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif /* !SETTINGS_WINDOW_H */