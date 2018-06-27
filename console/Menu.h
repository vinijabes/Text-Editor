#ifndef MENU_H
#define MENU_H

#include "../console/InputHandler.h"

typedef struct menu {
	StateCallbacks *state;
	DynamicStringList lines;
	bool open;
} Menu;

Menu * newMenu();
void deleteMenu(Menu *menu);
bool isOpenMenu(Menu menu);
bool openMenu(Menu *menu);
bool closeMenu(Menu *menu);
void freeMenu(Menu *menu);

void updateMenu(Menu menu);

#endif // !MENU_H

