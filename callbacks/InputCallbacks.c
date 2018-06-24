#include "../console/InputHandler.h"
#include "../console/OutputHandler.h"
#include "InputCallbacks.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>

extern StateCallbacks *currState;

void RegisterAllInputCallbacks(){
	registerCallback(currState, VK_RIGHT, false, false, true, 0.00, MoveRightCB);
	registerCallback(currState, VK_LEFT, false, false, true, 1.00, MoveLeftCB);
	//registerCallback(currState, VK_LEFT, true, false, true, 1.00, SelectLeftCB);
	registerCallback(currState, VK_UP, false, false, true, 0.05, MoveUpCB);
	registerCallback(currState, VK_DOWN, false, false, true, 0.05, MoveDownCB);
	registerCallback(currState, VK_HOME, false, false, true, 0.05, MoveToLineInit);
	registerCallback(currState, 'T', false, true, true, 0.05, clean);
	registerCallback(currState, 'Y', false, true, true, 0.05, printList);
	registerCallback(currState, 'U', false, true, true, 0.05, writeToFile);
	registerCallback(currState, 13, false, false, true, 0.05, breakLineCB);
	registerCallback(currState, 'A', false, true, true, 0.05, Highlight);
	//registerCallback(MK_LBUTTON, false, false, false, 0, Highlight);
	
	//registerCallback('B', false, true, 0.2, cb3);
}

void MoveRightCB(){
	//printf("MOVING RIGHT\n");
	if(cursor.X < consoleBuffer.X - 1)
		gotoxy(cursor.X+1, cursor.Y);
}

void MoveLeftCB(){
	//printf("MOVING LEFT\n");
	//printf("%d\n", clock());
	if(cursor.X > 0)
		gotoxy(cursor.X-1, cursor.Y);
}

void MoveUpCB(){
	//printf("MOVING UP\n");
	if(cursor.Y > 0)
		gotoxy(cursor.X, cursor.Y - 1);
}

void MoveDownCB(){
	//printf("MOVING DOWN\n");
	if(cursor.Y < consoleBuffer.Y - 1)
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

int ascii_to_unicode(unsigned short ch){
	if(ch < 128) return ch;
	switch(ch){
		case 130: return 233; //é
		case 131: return 226; //â
		case 132: return 228; //ä
		case 133: return 224; //à
		case 136: return 234; //ê
		case 137: return 235; //ë
		case 138: return 232; //è
		case 139: return 239; //ï
		case 140: return 238; //î
		case 141: return 236; //ì
		case 160: return 225; //á
		case 161: return 237; //í
		case 198: return 227; //ã
	}
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
	pushCharacter(curr->str, '\n');
	moveListIterator(&lines, cursor.Y + 1);
	addString(&lines, newLine);
	gotoxy(0, cursor.Y + 1);
	//printf("%d\n", lines.it.pos);
}
