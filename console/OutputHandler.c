#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "OutputHandler.h"
#include "InputHandler.h"

CONSOLE_SCREEN_BUFFER_INFO oldCsbiInfo;
DWORD outputOldMode;
WORD wOldColorAttrs;
HANDLE hStdout;

extern COORD cursor;

void initOutput(){
	hStdout = GetStdHandle ( STD_OUTPUT_HANDLE );
	GetConsoleScreenBufferInfo(hStdout, &oldCsbiInfo);
	wOldColorAttrs = oldCsbiInfo.wAttributes;
	consoleBuffer.X = oldCsbiInfo.srWindow.Right - oldCsbiInfo.srWindow.Left + 1;
	consoleBuffer.Y = oldCsbiInfo.srWindow.Bottom - oldCsbiInfo.srWindow.Top + 1;

	//COORD newSize = {9999,9999};
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), consoleBuffer);
}

void handleOutput(){
	if(consoleBuffer.X - cursor.X <= 1){
		setBufferSize(cursor.X + 5, consoleBuffer.Y);
	}
}

void setAttributes(WORD attr){
	SetConsoleTextAttribute(hStdout, attr);
}

void setDefaultAttributes(){
	SetConsoleTextAttribute(hStdout, wOldColorAttrs);
}

void setBufferSize(int x, int y){
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(hStdout, &csbiInfo);
	consoleBuffer.X = x;
	consoleBuffer.Y = csbiInfo.srWindow.Bottom - csbiInfo.srWindow.Top + 1;;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), consoleBuffer);
}
