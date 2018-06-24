#ifndef MENU_CALLBACKS_H
#define MENU_CALLBACKS_H

#include "../console/Menu.h"

Menu *fileMenu;

void openFileMenu();
void closeFileMenu();

void loadFile();
void saveFile();
void newFile();

#endif // !MENU_CALLBACKS_H

