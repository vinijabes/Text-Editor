#include "../console/consoleio.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>

extern StateCallbacks *currState;

void printTeste() {
	printf("Pimbada");
}

void Backspace() {
	if(cursor.X != 1) printf("\b \b");
	else printf("\b \b ");
	removeCharacter(lines.it.current->str);
	unwriteLineAfterIterator(lines.it.current->str);
	gotoxy(cursor.X - 1, cursor.Y);
	printStringIt(lines.it.current->str);
	gotoxy(cursor.X, cursor.Y);
}

void Delete() {
	unwriteLineAfterIterator(lines.it.current->str);
	gotoxy(cursor.X + 1, cursor.Y);
	removeCharacter(lines.it.current->str);
	gotoxy(cursor.X - 1, cursor.Y);
	printStringIt(lines.it.current->str);
	gotoxy(cursor.X, cursor.Y);
}

void RegisterAllInputCallbacks(){
	registerCallback(currState, VK_F1, false, false, true, 0.5, openFileMenu);

	registerCallback(currState, VK_RIGHT, false, false, true, 0.00, MoveRightCB);
	registerCallback(currState, VK_LEFT, false, false, true, 0.00, MoveLeftCB);

	registerCallback(currState, VK_HOME, false, false, true, 0.05, MoveToLineInit);
	//registerCallback(currState, VK_HOME, true, false, true, 0.05, MoveToLineInit);
	registerCallback(currState, VK_LEFT, true, false, true, 0.00, shiftLeftSelection);
	registerCallback(currState, VK_RIGHT, true, false, true, 0.00, shiftRightSelection);
	registerCallback(currState, VK_UP, true, false, true, 0.00, shiftUpSelection);
	registerCallback(currState, VK_DOWN, true, false, true, 0.00, shiftDownSelection);
	registerCallback(currState, 'C', false, true, true, 0.00, copySelection);

	//registerCallback(currState, VK_LEFT, true, false, true, 1.00, SelectLeftCB);
	registerCallback(currState, VK_UP, false, false, true, 0.05, MoveUpCB);
	registerCallback(currState, VK_DOWN, false, false, true, 0.05, MoveDownCB);

	registerCallback(currState, VK_BACK, false, false, true, 0.05, Backspace);
	registerCallback(currState, VK_DELETE, false, false, true, 0.05, Delete);
	registerCallback(currState, 'T', false, true, true, 0.05, clean);
	registerCallback(currState, 'Y', false, true, true, 0.05, printList);
	registerCallback(currState, 'U', false, true, true, 0.05, writeToFile);
	registerCallback(currState, 13, false, false, true, 0.05, breakLineCB);
	registerCallback(currState, 'A', false, true, true, 0.05, Highlight);
	//registerCallback(MK_LBUTTON, false, false, false, 0, Highlight);
	
	//registerCallback('B', false, true, 0.2, cb3);
}

void MoveRightCB(){

	if(cursor.X < consoleBuffer.X - 1 && cursor.X < lines.it.current->str->size)
		gotoxy(cursor.X+1, cursor.Y);
}

void MoveLeftCB(){
	if(cursor.X > 0)
		gotoxy(cursor.X-1, cursor.Y);
}

void MoveUpCB(){
	if(cursor.Y > 0)
		gotoxy(cursor.X, cursor.Y - 1);
}

void MoveDownCB(){
	if(cursor.Y < consoleBuffer.Y - 1 && cursor.Y < lines.size)
		gotoxy(cursor.X, cursor.Y + 1);
}

void MoveToLineInit(){
	gotoxy(0, cursor.Y);
}

void Highlight(){
	setAttributes(FOREGROUND_RED|BACKGROUND_BLUE|BACKGROUND_RED|BACKGROUND_GREEN| BACKGROUND_INTENSITY);
}

void clean(){
	system("cls");
}

extern DynamicStringList lines;

void printList(){
	gotoxy(0,0);
	printAll(&lines);
}

void writeToFile(){
	FILE * f = fopen("teste.txt", "w+");
	DynamicString *str = lines.it.current->str;
	StringCharacter *ch =  str->ini;
	while(ch){
		fputwc(ascii_to_unicode(ch->ch), f);
		//fwprintf(f, L"%c", ch->ch);
		printf("%c", ch->ch);
		ch = ch->next;
	}
	//fwprintf(f, L"%c", getchar());
	fclose(f);
	system("pause");
	f = fopen("teste.txt", "r");
	int c = fgetc(f);
	printf("\n");
	while(c != EOF){
		printf("%c: %d\n", c, c);
		c = fgetc(f);
	}
	
	printString(str);
	printf("HI");
}

void breakLineCB(){
	DynamicStringListNode *curr = lines.it.current;
	DynamicString *newLine = breakString(curr->str, cursor.X - 1);
	printString(newLine);
	moveListIterator(&lines, cursor.Y + 1);
	addString(&lines, newLine);
	gotoxy(0, cursor.Y + 1);
}

void unwriteLineAfterIterator(DynamicString *str) {
	StringCharacter *ini;
	if (str->it.current) {
		ini = str->it.current;
	}else{
		ini = str->ini;
	}
	
	while (ini) {
		printf("%c", ' ');
		ini = ini->next;
	}
}