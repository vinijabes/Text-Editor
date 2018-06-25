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
	else printf("\b \b ");	if (cursor.X != 0) {
		removeCharacter(lines.it.current->str);
		outputLine = cursor.Y;
		outputLineEnd = outputLine + 1;
		tempLineSize = lines.it.current->str->size + 1;
		gotoxy(cursor.X - 1, cursor.Y);
	}else if(cursor.Y != 0){
		if (!lines.it.current) moveListIterator(&lines, cursor.Y);
		tempX = lines.it.current->prev->str->size - 1;
		tempY = cursor.Y - 1;
		concatString(lines.it.current->prev->str, lines.it.current->str);
		outputLine = cursor.Y - 1;
		outputLineEnd = lines.size;
		
		gotoxy(0, lines.end);
		unwriteLineAfterIterator(lines.it.current->str);
		
		moveListIterator(&lines, tempY + 1);
		removeString(&lines);
	}
	//unwriteLineAfterIterator(lines.it.current->str);
	//gotoxy(cursor.X - 1, cursor.Y);
	//printStringIt(lines.it.current->str);
	//gotoxy(cursor.X, cursor.Y);
}

void Delete() {
	if (cursor.X == lines.it.current->str->size - 1 && lines.it.current->next) {
		tempX = lines.it.current->str->size - 1;
		tempY = cursor.Y;
		concatString(lines.it.current->str, lines.it.current->next->str);
		outputLine = cursor.Y;
		outputLineEnd = lines.size + 1;

		gotoxy(0, lines.end);
		unwriteLineAfterIterator(lines.it.current->str);

		moveListIterator(&lines, tempY + 1);
		removeString(&lines);
	}
	else {
		outputLine = cursor.Y;
		outputLineEnd = cursor.Y + 1;
		tempX = cursor.X - 1;
		unwriteLineAfterIterator(lines.it.current->str);
		gotoxy(cursor.X + 1, cursor.Y);
		removeCharacter(lines.it.current->str);
		gotoxy(cursor.X - 1, cursor.Y);
		printStringIt(lines.it.current->str);
		gotoxy(cursor.X, cursor.Y);
	}
}

void RegisterAllInputCallbacks(){
	registerCallback(currState, VK_F1, false, false, true, 0.5, openFileMenu);

	registerCallback(currState, VK_RIGHT, false, false, true, 0.00, MoveRightCB);
	registerCallback(currState, VK_LEFT, false, false, true, 0.00, MoveLeftCB);

	registerCallback(currState, VK_HOME, false, false, true, 0.05, MoveToLineInit);
	registerCallback(currState, VK_END, false, false, true, 0.05, MoveToLineEnd);
	registerCallback(currState, VK_LEFT, true, false, true, 0.00, shiftLeftSelection);
	registerCallback(currState, VK_RIGHT, true, false, true, 0.00, shiftRightSelection);
	registerCallback(currState, VK_UP, true, false, true, 0.00, shiftUpSelection);
	registerCallback(currState, VK_DOWN, true, false, true, 0.00, shiftDownSelection);
	registerCallback(currState, 'C', false, true, true, 0.00, copySelection);
	registerCallback(currState, 'V', false, true, true, 0.50, pasteClip);

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
	freeSelection();
	//if(cursor.X < consoleBuffer.X - 1 && cursor.X < lines.it.current->str->size)
	if (cursor.X == lines.it.current->str->size - 1) {
		if (lines.it.current == lines.end) {
			gotoxy(cursor.X + 1, cursor.Y);
		}
		else {
			gotoxy(0, cursor.Y + 1);
		}
	}
	else {
		gotoxy(cursor.X + 1, cursor.Y);
	}
}

void MoveLeftCB(){
	freeSelection();
	if (cursor.X != 0) {
		gotoxy(cursor.X - 1, cursor.Y);
	}
	else {
		if (lines.it.current->prev)
			gotoxy(lines.it.current->prev->str->size, cursor.Y - 1);
	}
}

void MoveUpCB(){
	freeSelection();
	if (cursor.X == lines.it.current->str->size || lines.it.current->str->it.current->next->ch == '\n') {
		if (lines.it.current->prev)
			gotoxy(lines.it.current->prev->str->size, cursor.Y - 1);		
	}else {
		gotoxy(cursor.X, cursor.Y - 1);
	}
}

void MoveDownCB(){
	freeSelection();
	if (cursor.X == lines.it.current->str->size-1) {
		if (lines.it.current->next)
			gotoxy(lines.it.current->next->str->size, cursor.Y + 1);
	}
	else {
		gotoxy(cursor.X, cursor.Y + 1);
	}
}

void MoveToLineInit(){
	freeSelection();
	gotoxy(0, cursor.Y);
}

void MoveToLineEnd() {
	freeSelection();
	gotoxy(lines.it.current->str->size, cursor.Y);
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

extern int outputLine;

void breakLineCB(){
	DynamicStringListNode *curr = lines.it.current;
	unwriteLineAfterIterator(curr->str);
	DynamicString *newLine = breakString(curr->str, cursor.X - 1);
	addString(&lines, newLine);
	gotoxy(curr->str->size, cursor.Y);
	outputChar('\n');
	curr = curr->next;
	outputLine = cursor.Y + 1;
	outputLineEnd = lines.size;
	int y = cursor.Y + 1;
	++cursor.Y;
	/*while (curr->next) {
		moveIterator(curr->next->str, 0);
		gotoxy(0, cursor.Y);
		unwriteLineAfterIterator(curr->next->str);
		gotoxy(0, cursor.Y);
		printString(curr->str);
		curr = curr->next;
		++cursor.Y;
	}*/

	printString(curr->str);

	//++outputLine;
	gotoxy(0, y);
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