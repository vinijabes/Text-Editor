#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "OutputHandler.h"

CONSOLE_SCREEN_BUFFER_INFO oldCsbiInfo;
DWORD outputOldMode;
WORD wOldColorAttrs;
HANDLE hStdout;

extern COORD cursor;

void initOutput(){
	outputActive = true;
	hStdout = GetStdHandle ( STD_OUTPUT_HANDLE );
	GetConsoleScreenBufferInfo(hStdout, &oldCsbiInfo);
	wOldColorAttrs = oldCsbiInfo.wAttributes;
	consoleBuffer.X = oldCsbiInfo.srWindow.Right - oldCsbiInfo.srWindow.Left + 1;
	consoleBuffer.Y = oldCsbiInfo.srWindow.Bottom - oldCsbiInfo.srWindow.Top + 1;

	//COORD newSize = {9999,9999};
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), consoleBuffer);
	outputLine = 0;
	outputLineEnd = 0;
	tempLineSize = -1;
	tempX = -1;
	tempY = -1;
}

void handleOutput(){
	if(consoleBuffer.X - cursor.X <= 1 || (lines.it.current && consoleBuffer.X - lines.it.current->str->size <= 1)){
		setBufferSize(consoleBuffer.X + 5, consoleBuffer.Y);
	}

	if (consoleBuffer.Y - cursor.Y <= 1 || consoleBuffer.Y - lines.size <= 1) {
		setBufferSize(consoleBuffer.X, consoleBuffer.Y + 1);
	}

	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo)) {
		DWORD dw = GetLastError();
		printf("%lu", dw);
	}

	if (min(csbiInfo.srWindow.Bottom, outputLineEnd)> outputLine && outputLine <= lines.size) {
		int x = cursor.X;
		int y = cursor.Y;		
		int i = 0;
		int j = max(cursor.Y, outputLine);
		int maxSize = 0;
		DynamicStringListNode * aux = lines.ini;
		if (tempLineSize == -1) {
			while (aux) {
				maxSize = max(maxSize, aux->str->size);
				aux = aux->next;
			}
		}
		else {
			maxSize = tempLineSize;
			tempLineSize = -1;
		}

		gotoxy(0, j);


		int end = min(outputLineEnd, csbiInfo.srWindow.Bottom + 1);
		for (; j < end; j++) {
			for (i = 0; i < maxSize; i++) {
				outputChar(' ');
			}
			if(j != end) outputChar('\n');
		}

		while (outputLine < end) {
			gotoxy(0, outputLine);			
			outputString(lines.it.current->str);
			++outputLine;
		}
		x = tempX != -1 ? tempX : x;
		y = tempY != -1 ? tempY : y;

		tempX = tempY = -1;
		gotoxy(x, y);
	}
}

void setAttributes(WORD attr){
	SetConsoleTextAttribute(hStdout, attr);
}

void setDefaultAttributes(){
	SetConsoleTextAttribute(hStdout, wOldColorAttrs);
}

#include <strsafe.h>

void setBufferSize(int x, int y){	
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
	consoleBuffer.X = max(x, csbiInfo.dwSize.X);
	consoleBuffer.Y = max(y, csbiInfo.dwSize.Y);
	if (!SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), consoleBuffer)) {
		DWORD dw = GetLastError();
		printf("%lu", dw);
	}
}

void setOutputState(bool state) {
	outputActive = state;
}

void outputChar(char ch) {
	if (outputActive) {
		if (ch == '\t')
			putchar(' ');
		else
			putchar(ch);
	}
}
void outputCharArray(char *ch) {
	while (ch != '\0') {
		outputChar(ch);
	}
}
void outputString(DynamicString *string) {
	StringCharacter *pos = string->ini;
	while (pos) {
		if (consoleBuffer.X - cursor.X <= 1) {
			setBufferSize(cursor.X + 5, consoleBuffer.Y);
		}

		if (consoleBuffer.Y - cursor.Y <= 1) {
			setBufferSize(consoleBuffer.X, consoleBuffer.Y + 1);
		}
		outputChar(pos->ch);
		++cursor.X;
		pos = pos->next;
	}
}