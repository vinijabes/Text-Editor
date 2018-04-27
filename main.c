#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <errno.h>
#include <string.h>
#include "console/consoleio.h"
#include "string/dynamicstring.h"

int main(int argc, char *argv[]) {
	
	initOutput();
	initInput();
	RegisterAllInputCallbacks();
	
	DynamicString str;
	InitString(&str);
	moveIterator(&str, 10);
	addCharacter(&str, 'A');
	moveIterator(&str, 0);
	addCharacter(&str, 'B');
	addCharacter(&str, 'C');
	moveIterator(&str, 0);
	addCharacter(&str, '\n');
	moveIterator(&str, 5);
	insertCharArray(&str, "Teste\n");
	printString(&str);
	
	
	while(1){
		handleInput();
		handleOutput();
	}
	
	return 0;
}
