#include "consoleio.h"

Menu *newMenu() {
	Menu *menu = (Menu*)malloc(sizeof(Menu));
	menu->open = false;
	menu->state = newState();

	InitStringList(&menu->lines);
	addString(&menu->lines, newLine());
	return menu;
}

void deleteMenu(Menu *menu) {

}

bool isOpenMenu(Menu menu) {
	return menu.open;
}

bool openMenu(Menu *menu) {
	menu->open = true;
	Sleep(200);
}

bool closeMenu(Menu *menu) {
	menu->open = false;
	Sleep(200);
}

void updateMenu(Menu menu) {
	handleKeyboard();
}

void freeMenu(Menu *menu) {
	freeDynamicStringList(&menu->lines);
}