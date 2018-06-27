#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <errno.h>
#include <string.h>
#include <locale.h>
#include "console/consoleio.h"
#include "string/dynamicstring.h"
#include "string/dynamicstringlist.h"

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "en_US.utf8");
	initOutput();
	initInput();
	RegisterAllInputCallbacks();

	openFileMenu();
	while(1){
		handleInput();
		handleOutput();
	}

	system("pause");

	return 0;
}