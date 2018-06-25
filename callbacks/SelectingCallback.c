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
		} else if ((selectionEnd->charPos <= charPos && selectionEnd->line < line) &&
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
	if (cursor.X != 0) {
		gotoxy(cursor.X - 1, cursor.Y);
	}else{
		if(lines.it.current->prev)
			gotoxy(lines.it.current->prev->str->size, cursor.Y - 1);
	}
}

void shiftRightSelection() {
	if (cursor.X == lines.it.current->str->size - 1) {
		if (lines.it.current == lines.end) {
			gotoxy(cursor.X + 1, cursor.Y);
		}else {
			gotoxy(0, cursor.Y + 1);
		}
	}
	else {
		gotoxy(cursor.X + 1, cursor.Y);
	}
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

    char* output = (char*)malloc(totalSize + 2);
	output[totalSize + 1] = '\0';
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
		while (j <= lineCharPosEnd) {
			gotoxy(j + 1, cursor.Y);
			output[auxiliarIndex + j - lineCharPosIni] = ascii_to_unicode(lines.it.current->str->it.current->ch);
			++j;
		}
		if (i != lineEnd) {
			output[auxiliarIndex + j - lineCharPosIni - 2] = '\n';
		}
		auxiliarIndex += lineCharPosEnd - lineCharPosIni;
		i++;
	}

	const size_t len = totalSize + 2;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(hMem), output, len);
	GlobalUnlock(hMem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();

	freeSelection();
}

void pasteClip() {
	OpenClipboard(NULL);
	char *text = NULL;
	if (IsClipboardFormatAvailable(CF_TEXT)) {
		text = GetClipboardData(CF_TEXT);
	}
	else if (IsClipboardFormatAvailable(CF_UNICODETEXT)) {
		text = GetClipboardData(CF_UNICODETEXT);
	}

	outputLine = lines.it.pos;
	while (*text) {
		if (*text != '\n') {
			if (*text == '\r') {
				++text;
				continue;
			}
			addCharacter(lines.it.current->str, *text);
		}
		else {
			DynamicString *newStr = breakString(lines.it.current->str, lines.it.current->str->it.pos);
			addString(&lines, newStr);
		}
		++text;
	}
	outputLineEnd = lines.it.pos + 2;
	tempY = outputLineEnd;
	tempX = lines.it.current->str->size;

	CloseClipboard();
}

void freeSelection() {
	if (selectionIni == NULL) return;
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
		while (j <= lineCharPosEnd) {
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