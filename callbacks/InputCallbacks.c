#include "../console/InputHandler.h"
#include "../console/OutputHandler.h"
#include "InputCallbacks.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

extern StateCallbacks *currState;

void RegisterAllInputCallbacks(){
	registerCallback(currState, VK_RIGHT, false, false, true, 0.00, MoveRightCB);
	registerCallback(currState, VK_LEFT, false, false, true, 0.00, MoveLeftCB);
	registerCallback(currState, VK_UP, false, false, true, 0.05, MoveUpCB);
	registerCallback(currState, VK_DOWN, false, false, true, 0.05, MoveDownCB);
	registerCallback(currState, VK_HOME, false, false, true, 0.05, MoveToLineInit);
	registerCallback(currState, 'T', false, true, true, 0.05, clean);
	registerCallback(currState, 'Y', false, true, true, 0.05, printList);
	registerCallback(currState, 13, false, false, true, 0.05, breakLineCB);
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

void breakLineCB(){
	DynamicStringListNode *curr = lines.it.current;
	DynamicString *newLine = breakString(curr->str, cursor.X - 1);
	printString(newLine);
	pushCharacter(curr->str, '\n');
	moveListIterator(&lines, cursor.Y);
	addString(&lines, newLine);
	gotoxy(0, cursor.Y + 1);
	printf("%d\n", lines.it.pos);
}
