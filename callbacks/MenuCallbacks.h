#ifndef MENU_CALLBACKS_H
#define MENU_CALLBACKS_H

#include "../console/Menu.h"

Menu *fileMenu;
Menu *searchMenu;
Menu *helpMenu;

void openFileMenu();
void closeFileMenu();

void openSearchMenu();
void closeSearchMenu();

void openHelpMenu();
void closeHelpMenu();

void loadFile();
void loadFiled();
void saveFile();
void saveFiled();
void newFile();
void newFiled();

#endif // !MENU_CALLBACKS_H

