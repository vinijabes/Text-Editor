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
}

void handleOutput(){
	if(consoleBuffer.X - cursor.X <= 1){
		setBufferSize(cursor.X + 5, consoleBuffer.Y);
	}

	if (consoleBuffer.Y - cursor.Y <= 1) {
		setBufferSize(consoleBuffer.X, consoleBuffer.Y + 1);
	}

	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(hStdout, &csbiInfo);
	if (csbiInfo.srWindow.Bottom > outputLine && outputLine < lines.size) {
		int x = cursor.X;
		int y = cursor.Y;		
		while (outputLine < lines.size) {
			gotoxy(0, outputLine);
			outputString(lines.it.current->str);
			++outputLine;
		}
		gotoxy(x, y);
	}
}

void setAttributes(WORD attr){
	SetConsoleTextAttribute(hStdout, attr);
}

void setDefaultAttributes(){
	SetConsoleTextAttribute(hStdout, wOldColorAttrs);
}

void setBufferSize(int x, int y){	
	consoleBuffer.X = x;
	consoleBuffer.Y = y;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), consoleBuffer);
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