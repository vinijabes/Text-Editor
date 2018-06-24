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
	
	/*while(1){
		handleInput();
		handleOutput();
	}*/
	
	DynamicString c;
	InitString(&c);
	pushCharacter(&c, 'a');
	moveIterator(&c, -1);
	addCharacter(&c, 'b');
	addCharacter(&c, 'c');
	moveIterator(&c, 0);
	addCharacter(&c, 'd');
	pushCharacter(&c, 'e');
	addCharacter(&c, 'f');
	printString(&c);
	
	DynamicString *b = breakString(&c, 1);
	printf("\n");
	printString(&c);
	printString(b);
	
	return 0;
}
