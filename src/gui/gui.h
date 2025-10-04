#ifndef GUI_H
#define GUI_H

#include <objbase.h>
#include <shlobj.h>
#include <shobjidl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "save_game_list.h"

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
#define PRE_PAGE_BUTTON_ID 201
#define POST_PAGE_BUTTON_ID 202
#define EXPORT_SINGLE_NAME_INPUT2_ID 301
#define EXPORT_SINGLE_BUTTON2_ID 302

#define SELECTED_BACKUP_TEXT 401
#define DESELECT_BACKUP_BUTTON_ID 402

extern const char MAIN_CLASS_NAME[];
extern const char SETTINGS_CLASS_NAME[];
extern const char EXPORT_SINGLE_CLASS_NAME[];

extern int current_list_offset;
extern int entries_per_page;

extern save_game_list *backups_list;
extern save_game_list *saves_list;

extern save_game_list *selected_backup;
extern save_game_list *selected_save;

extern HWND thisHwnd;
extern HINSTANCE thisHInstance;
extern HWND selectedBackupTextHwnd;

extern char exported_name[512];

void OpenExportSingleWindow(HWND parentHwnd);
void OpenSettingsWindow(HWND parentHwnd);
int SelectFolder(HWND hwnd, char *path, int size);

void create_export_single_window_elements(HWND hwnd);
void create_settings_window_elements(HWND hwnd);
void create_main_window_elements(HWND hwnd, HINSTANCE hInstance);

void scroll_backups(HWND hwnd, HINSTANCE hInstance, int up);

void check_backups_buttons(WPARAM wParam);
void check_saves_buttons(WPARAM wParam, HWND hwnd);

void refresh_buttons(void);

#endif /* GUI_H */