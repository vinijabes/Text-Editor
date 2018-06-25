#ifndef SELECTING_CALLBACK_H
#define SELECTING_CALLBACK_H

#include <stdlib.h>

typedef struct selectNode {
	int line;
	int charPos;
} SelectionNode;


void handleSelection();
void shiftLeftSelection();
void shiftRightSelection();
void shiftUpSelection();
void shiftDownSelection();

void copySelection();
void pasteClip();

void freeSelection();

SelectionNode *selectionIni;
SelectionNode *selectionEnd;


#endif // !SELECTING_CALLBACK_H

