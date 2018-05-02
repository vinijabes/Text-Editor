#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <errno.h>
#include <string.h>
#include "console/consoleio.h"
#include "string/dynamicstring.h"
#include "string/dynamicstringlist.h"

int main(int argc, char *argv[]) {
	
	initOutput();
	initInput();
	RegisterAllInputCallbacks();
	
	while(1){
		handleInput();
		handleOutput();
	}
	
	return 0;
}
