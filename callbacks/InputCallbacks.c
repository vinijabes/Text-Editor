#include "../console/consoleio.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>

extern StateCallbacks *currState;

void Backspace() {
	if(cursor.X != 1) printf("\b \b");
	else printf("\b \b ");	if (cursor.X != 0) {
		/*if (tempNode == NULL) {			
			tempNode = newStackNode(NULL, NULL, 0, 0, REMOVE);
		}
		if ((float)(lastInsert / CLOCKS_PER_SEC) - (float)(clock() / CLOCKS_PER_SEC) > 1000 || tempNode->type != REMOVE) {
			pushToStack(currStack, tempNode);
			tempNode = newStackNode(NULL, NULL, 0, 0, REMOVE);
		}
		if (tempNode->ini == NULL) {
			tempNode->iniLine = lines.it.pos;
			tempNode->endLine = lines.it.pos;
			tempNode->ini = lines.it.current->str->it.current;
			tempNode->end = lines.it.current->str->it.current;
		} else {
			StringCharacter *aux = lines.it.current->str->it.current;

			if (aux == tempNode->ini->prev && aux != NULL) {				
				tempNode->ini = aux;
			} else if (aux == tempNode->end->next && aux != NULL) {
				tempNode->end = aux;
			} else {
				pushToStack(currStack, tempNode);
				tempNode = newStackNode(NULL, NULL, 0, 0, REMOVE);
				tempNode->iniLine = lines.it.pos;
				tempNode->endLine = lines.it.pos;
				tempNode->ini = lines.it.current->str->it.current;
				tempNode->end = lines.it.current->str->it.current;
			}
		}*/

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
		
		gotoxy(0, outputLine + 1);
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
		tempLineSize = consoleBuffer.X + 10;
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
	registerCallback(currState, VK_F2, false, false, true, 0.5, openHelpMenu);
	registerCallback(currState, 'F', false, true, true, 0.5, openSearchMenu);

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
	registerCallback(currState, 'Z', false, true, true, 0.40, undoAction);
	registerCallback(currState, 'S', false, true, true, 0.50, saveFiled);
	registerCallback(currState, 'O', false, true, true, 0.50, loadFiled);
	registerCallback(currState, 'N', false, true, true, 0.50, newFiled);

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
	if (cursor.X == lines.it.current->str->size || lines.it.current->str->it.current && lines.it.current->str->it.current->next->ch == '\n') {
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
	int pos = lines.it.pos;
	DynamicStringListNode *curr = lines.it.current;
	unwriteLineAfterIterator(curr->str);
	DynamicString *newLine = breakString(curr->str, cursor.X - 1);
	addString(&lines, newLine);
	gotoxy(curr->str->size, cursor.Y);
	outputChar('\n');
	curr = curr->next;
	outputLine = cursor.Y;
	outputLineEnd = lines.size;
	int y = cursor.Y + 1;
	++cursor.Y;
	if(tempNode) pushToStack(currStack, tempNode);
	tempNode = newStackNode(curr->str->end, newLine->ini, pos, pos+1, NEW_LINE);
	pushToStack(currStack, tempNode);
	tempNode = NULL;
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


void onWrite(UndoStackNode * node) {
	int x = cursor.X;
	int y = cursor.Y;

	outputLine = node->iniLine;
	outputLineEnd = lines.size;
	StringCharacter *aux;
	if (node->ini->prev) {
		node->ini->prev->next = node->end->next;
	}
	else {
		gotoxy(0, node->iniLine);
		DynamicString *str = lines.it.current->str;
		if (node->end->next) {
			str->ini = node->end->next;
		}
		else {
			str->ini = NULL;
		}
	}

	if (node->end->next)
		node->end->next->prev = node->ini->prev;
	else {
		gotoxy(0, node->endLine);
		DynamicString *str = lines.it.current->str;		
		if (node->ini->prev) {
			str->end = node->ini->prev;
		}
		else {
			str->end = NULL;
		}
	}
	
	outputLine = node->iniLine;
	outputLineEnd = lines.size + 1;

	int pos = lines.it.pos;
	tempLineSize = 180;
	moveListIterator(&lines, node->iniLine);
	lines.it.current->str->size = dynamicStringLen(lines.it.current->str);
	moveListIterator(&lines, pos);

	gotoxy(x, y);
}

void onNewLine(UndoStackNode * node) {
	gotoxy(0, node->iniLine);
	gotoxy(lines.it.current->str->size, cursor.Y);
	DynamicString *str1 = lines.it.current->str;
	gotoxy(0, node->endLine);
	gotoxy(lines.it.current->str->size, cursor.Y);
	DynamicString *str2 = lines.it.current->str;

	concatString(str1, str2);
	removeString(&lines);
	outputLine = node->iniLine;

	gotoxy(0, node->iniLine);
	gotoxy(lines.it.current->str->size, cursor.Y);
}

void onRemove(UndoStackNode *node) {
	if (node->ini->prev) {
		node->ini->prev->next = node;
	}
	else {
		gotoxy(0, node->iniLine);
		lines.it.current->str->ini = node;
	}

	if (node->ini->next) {
		node->ini->next->prev = node;
	}else{
		gotoxy(0, node->iniLine);
		lines.it.current->str->end = node;
	}

	outputLine = node->iniLine;
	gotoxy(0, node->iniLine);
	gotoxy(lines.it.current->str->size, cursor.Y);
}

void undoAction() {
	UndoStackNode * node;
	if (tempNode != NULL && tempNode->ini != NULL) {
		node = tempNode;
		tempNode = NULL;
	} else {
		node = popStack(currStack);		
	}

	if (!node) return;

	switch (node->type) {
	case WRITE:
		onWrite(node);
		break;
	case NEW_LINE:
		onNewLine(node);
		break;
	case REMOVE:
		onRemove(node);
		break;
	}
}