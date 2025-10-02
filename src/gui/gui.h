#ifndef GUI_H
#define GUI_H

#include <objbase.h>
#include <shlobj.h>
#include <shobjidl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define SETTINGS_BUTTON_ID 1
#define EXPORT_SINGLE_BUTTON_ID 2
#define EXPORT_SINGLE_NUMBER_INPUT_ID 3
#define EXPORT_SINGLE_NAME_INPUT_ID 4
#define EXPORT_ALL_BUTTON_ID 5
#define EXPORT_ALL_NAME_INPUT_ID 7
#define IMPORT_SINGLE_BUTTON_ID 8
#define IMPORT_SINGLE_NUMBER_INPUT_ID 9
#define IMPORT_SINGLE_NAME_INPUT_ID 10
#define IMPORT_ALL_BUTTON_ID 11
#define IMPORT_ALL_NAME_INPUT_ID 12
#define APPLY_SETTINGS_BUTTON_ID 13
#define CLOSE_SETTINGS_BUTTON_ID 14
#define BROWSE_SAVES_BUTTON_ID 101
#define PATH_SAVES_EDIT_ID 102
#define BROWSE_BACKUPS_BUTTON_ID 103
#define PATH_BACKUPS_EDIT_ID 104

extern const char MAIN_CLASS_NAME[];
extern const char SETTINGS_CLASS_NAME[];

void OpenSettingsWindow(HWND parentHwnd);
int SelectFolder(HWND hwnd, char *path, int size);
void create_settings_window_elements(HWND hwnd);
void create_main_window_elements(HWND hwnd, HINSTANCE hInstance);

#endif /* GUI_H */