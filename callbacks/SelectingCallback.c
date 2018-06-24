#include "../console/consoleio.h"
#include <stdio.h>
#include <Windows.h>

void handleSelection() {
	if (selectionIni == NULL) {
		setAttributes(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		selectionIni = (SelectionNode*)malloc(sizeof(SelectionNode));
		selectionEnd = (SelectionNode*)malloc(sizeof(SelectionNode));
		
		selectionIni->line = -1;
		selectionIni->charPos = -1;

		selectionEnd->line = -1;
		selectionEnd->charPos = -1;

		selectionIni->line = lines.it.pos;
		selectionIni->charPos = lines.it.current->str->it.pos;

		*selectionEnd = *selectionIni;
		if (lines.it.current->str->it.current)
			printf("\b \b%c", lines.it.current->str->it.current->ch);
		return;
	} else {
		int line = lines.it.pos;
		int charPos = lines.it.current->str->it.pos;

		if ((selectionEnd->charPos >= charPos || selectionEnd->line > line) &&
			(selectionIni->charPos < charPos && selectionIni->line <= line)) {
			setDefaultAttributes();
		} else if ((selectionEnd->charPos <= charPos || selectionEnd->line < line) &&
				   (selectionIni->charPos > charPos && selectionIni->line >= line)) {
			setDefaultAttributes();
		} else {
			setAttributes(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		}

		selectionEnd->line = lines.it.pos;
		selectionEnd->charPos = lines.it.current->str->it.pos;

		if (selectionIni->line == selectionEnd->line &&
			selectionIni->charPos == selectionEnd->charPos){
			setDefaultAttributes();
			freeSelection();
		}
		if (lines.it.current->str->it.current)
			printf("\b \b%c", lines.it.current->str->it.current->ch);
	}
}

void shiftLeftSelection() {
	handleSelection();
	gotoxy(cursor.X - 1, cursor.Y);
}

void shiftRightSelection() {
	gotoxy(cursor.X + 1, cursor.Y);
	handleSelection();
}

void shiftUpSelection() {
	int x = cursor.X, aux = x;
	while (x--) {
		handleSelection();
		gotoxy(x, cursor.Y);
	}
	gotoxy(0, cursor.Y - 1);
	gotoxy(lines.it.current->str->size, cursor.Y);
	x = cursor.X;
	while (x-- > aux) {
		handleSelection();
		gotoxy(x, cursor.Y);
	}
}

void shiftDownSelection() {
	int x = cursor.X, aux = x;
	while (x <= lines.it.current->str->size) {
		handleSelection();
		gotoxy(x, cursor.Y);
		++x;
	}
	gotoxy(0, cursor.Y + 1);
	x = cursor.X;
	while (x <= aux) {
		handleSelection();
		gotoxy(x, cursor.Y);
		++x;
	}
}

void copySelection() {
	int totalSize = 0;
	
	int lineStart = min(selectionIni->line, selectionEnd->line);
	int lineEnd = max(selectionIni->line, selectionEnd->line);
	int posStart;
	int posEnd;

	if (selectionIni->line <= selectionEnd->line) {
		lineStart = selectionIni->line;
		lineEnd = selectionEnd->line;

		if (selectionIni->line == selectionEnd->line) {
			posStart = min(selectionIni->charPos, selectionEnd->charPos);
			posEnd = max(selectionIni->charPos, selectionEnd->charPos);
		}else{
			posStart = selectionIni->charPos;
			posEnd = selectionEnd->charPos;
		}
	}else{
		lineStart = selectionEnd->line;
		lineEnd = selectionIni->line;
		posStart = selectionEnd->charPos;
		posEnd = selectionIni->charPos;
	}

	int i = lineStart;
	while (i <= lineEnd) {
		gotoxy(0, i);
		if (i == lineStart) {
			gotoxy(posStart, cursor.Y);
			if (lineStart != lineEnd)
				totalSize += lines.it.current->str->size - posStart;
			else
				totalSize = posEnd - posStart + 1;
		}else if(i == lineEnd){
			totalSize += posEnd;
		}else{
			totalSize += lines.it.current->str->size;
		}
		++i;
	}

    char* output = (char*)malloc(totalSize + 1);
	output[totalSize] = '\0';
	i = lineStart;
	
	int lineCharPosIni;
	int lineCharPosEnd;
	int auxiliarIndex = 0;
	int j;
	while (i <= lineEnd) {
		gotoxy(0, i);
		if (i == lineStart) {
			lineCharPosIni = posStart;
			lineCharPosEnd = lines.it.current->str->size;
		} else if (i == lineEnd) {
			lineCharPosIni = 0;
			lineCharPosEnd = posEnd;
		} else {
			lineCharPosIni = 0;
			lineCharPosEnd = lines.it.current->str->size;
		}

		j = lineCharPosIni;
		while (j < lineCharPosEnd) {
			gotoxy(j + 1, cursor.Y);
			output[auxiliarIndex + j - lineCharPosIni] = lines.it.current->str->it.current->ch;
			++j;
		}
		auxiliarIndex += lineCharPosEnd - lineCharPosIni;
		i++;
	}

	const size_t len = totalSize + 1;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(hMem), output, len);
	GlobalUnlock(hMem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();

	freeSelection();
}

void freeSelection() {
	int x = cursor.X;
	int y = cursor.Y;
	int lineStart = min(selectionIni->line, selectionEnd->line);
	int lineEnd = max(selectionIni->line, selectionEnd->line);
	int posStart;
	int posEnd;

	if (selectionIni->line <= selectionEnd->line) {
		lineStart = selectionIni->line;
		lineEnd = selectionEnd->line;

		if (selectionIni->line == selectionEnd->line) {
			posStart = min(selectionIni->charPos, selectionEnd->charPos);
			posEnd = max(selectionIni->charPos, selectionEnd->charPos);
		}
		else {
			posStart = selectionIni->charPos;
			posEnd = selectionEnd->charPos;
		}
	}
	else {
		lineStart = selectionEnd->line;
		lineEnd = selectionIni->line;
		posStart = selectionEnd->charPos;
		posEnd = selectionIni->charPos;
	}

	int lineCharPosIni;
	int lineCharPosEnd;
	int auxiliarIndex = 0;
	int i = lineStart;
	int j;
	while (i <= lineEnd) {
		gotoxy(0, i);
		if (i == lineStart) {
			lineCharPosIni = posStart;
			lineCharPosEnd = lines.it.current->str->size;
		}
		else if (i == lineEnd) {
			lineCharPosIni = 0;
			lineCharPosEnd = posEnd;
		}
		else {
			lineCharPosIni = 0;
			lineCharPosEnd = lines.it.current->str->size;
		}
		setDefaultAttributes();
		j = lineCharPosIni;
		while (j < lineCharPosEnd) {
			gotoxy(j + 1, cursor.Y);
			printf("\b%c",lines.it.current->str->it.current->ch);
			++j;
		}
		auxiliarIndex += lineCharPosEnd - lineCharPosIni;
		i++;
	}

	if (selectionIni) {
		free(selectionIni);
		selectionIni = NULL;
	}

	if (selectionEnd) {
		free(selectionEnd);
		selectionEnd = NULL;
	}

	setDefaultAttributes();
	gotoxy(x, y);	
}