#include <conio.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>
#include "InputHandler.h"
#include "../callbacks/InputCallbacks.h"


InputCallbackList callbacks[CALLBACK_LIST_COUNT];

HANDLE hStdin;
DWORD fdwMode, fdwSaveOldMode, cNumRead;
INPUT_RECORD irInBuf[128];
COORD cursor;

void initInput(){
	setlocale(LC_ALL, NULL);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if(!GetConsoleMode(hStdin, &fdwSaveOldMode)){
		printf("ERRO");
	}
	
	fdwMode = fdwSaveOldMode & ~ENABLE_QUICK_EDIT_MODE & ~ENABLE_PROCESSED_INPUT & ~ENABLE_ECHO_INPUT & ~ENABLE_WINDOW_INPUT & ~ENABLE_AUTO_POSITION;
	
	if(!SetConsoleMode(hStdin, fdwMode)){
		printf("ERRO");
	}
	
	GetConsoleMode(hStdin, &fdwMode);
	SetConsoleCtrlHandler(NULL, 0);
	int i = 0;
	currState = newState();
	
	cursor.X = 0;
	cursor.Y = 0;
	
	InitStringList(&lines);
	addString(&lines, newLine());
}

bool keyPressed(int key){
	GetKeyState(VK_F12);
	return GetKeyState(key) & 0x8000;
}

bool keyReleased(int key){
	return GetKeyState(key) & 0x0001;
}

StateCallbacks *newState(){
	int i = 0;
	StateCallbacks *ns = (StateCallbacks*) malloc(sizeof(StateCallbacks));
	for(; i < CALLBACK_LIST_COUNT; i++){
		initInputCallbackList(&(ns->callbacks[i]));
	}
	
	return ns;
}

void initInputCallbackList(InputCallbackList* list){
	list->end = list->init = NULL;
}

void registerCallback(StateCallbacks* state, int key, bool shift, bool cntrl, bool keyDown, float interval, void (*cb)()){
	if(state == NULL){
		state = currState;
	}
	
	if(shift && cntrl){
		pushCallback(&state->callbacks[SHIFT_CNTRL_CALLBACK], key, shift, cntrl, keyDown, interval, cb);
	}else if(shift){
		pushCallback(&state->callbacks[SHIFT_CALLBACK], key, shift, cntrl, keyDown, interval, cb);
	}else if(cntrl){
		pushCallback(&state->callbacks[CNTRL_CALLBACK], key, shift, cntrl, keyDown, interval, cb);
	}else{
		pushCallback(&(state->callbacks[KEYSTROKE_CALLBACK]), key, shift, cntrl, keyDown, interval, cb);
	}
	
}

void pushCallback(InputCallbackList* list, int key, bool shift, bool cntrl, bool keyDown, float interval, void (*cb)()){
	InputCallbackListNode *node = (InputCallbackListNode*)malloc(sizeof(InputCallbackListNode));
	node->callback.key = key;
	node->callback.shift = shift;
	node->callback.cntrl = cntrl;
	node->callback.keyDown = keyDown;
	node->callback.interval = interval;
	node->callback.lastCall = 0;
	node->callback.cb = cb;
	node->next = NULL;
	
	if(list->init == NULL){
		list->init = node;
	}
		
	if(list->end != NULL){
		list->end->next = node;
		list->end = node;
	}else{
		list->end = node;
	}
}

void runAllCallbacks(InputCallbackList *list){
	InputCallbackListNode *ini = list->init;
	
	while(ini){
		if(ini->callback.keyDown && keyPressed(ini->callback.key) && (float) clock()/CLOCKS_PER_SEC - (float) ini->callback.lastCall/CLOCKS_PER_SEC > ini->callback.interval){
			ini->callback.cb();
			ini->callback.lastCall = clock();
		}else if(!ini->callback.keyDown && keyReleased(ini->callback.key)){
			ini->callback.cb();
		}
		ini = ini->next;
	}
	
}

void handleInput(){
	int x = 0;
	DWORD pendingEvents;
	
	if(!GetNumberOfConsoleInputEvents(hStdin, &pendingEvents)){
		printf("%s\n", strerror(GetLastError()));
	}
	
	if(!pendingEvents || handleKeyboard()){	
		return;
	}else{
		if (!ReadConsoleInput( 
		    hStdin,      // input buffer handle 
		    irInBuf,     // buffer to read into 
		    128,         // size of read buffer 
		    &cNumRead) ) // number of records read 
			printf("%s\n", strerror(GetLastError()));
		
		for(x = 0; x < cNumRead; x++){
			switch(irInBuf[x].EventType){
				case MOUSE_EVENT:
					handleMouse(irInBuf[x].Event.MouseEvent);
					break;    		
					
			}
		}	
	}
}

bool handleKeyboard(){
	int c;
	if(keyPressed(VK_SHIFT)){
		if(keyPressed(VK_CONTROL)){
			runAllCallbacks(&currState->callbacks[SHIFT_CNTRL_CALLBACK]);
			while(_kbhit()){
				_getch();
			}
		}else{
			runAllCallbacks(&currState->callbacks[SHIFT_CALLBACK]);		
		}
		
		handleKeystroke();
		return true;
	}else if(keyPressed(VK_CONTROL)){
		runAllCallbacks(&currState->callbacks[CNTRL_CALLBACK]);
		clearInputBuffer();
		return true;
	}else{
		runAllCallbacks(&(currState->callbacks[KEYSTROKE_CALLBACK]));
		return handleKeystroke();
	}
	
	return false;
}

bool handleKeystroke(){
	int c;
	if(_kbhit()){		
		c = _getch();
		if(c == 224 || c == 0 || c == 8 || c == 27 || c == 13){
			clearInputBuffer();
			return false;
		}

		unwriteLineAfterIterator(lines.it.current->str);
		addCharacter(lines.it.current->str, c);
		gotoxy(cursor.X, cursor.Y);
		putchar(c);
		gotoxy(cursor.X, cursor.Y);
		printStringIt(lines.it.current->str);
		++cursor.X;
		gotoxy(cursor.X, cursor.Y);
		return true;
	}
	return false;
}

void clearInputBuffer(){
	while(_kbhit()){
		_getch();
	}
}

void handleMouse(MOUSE_EVENT_RECORD mer){
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif	

	switch(mer.dwEventFlags)
    {
        case 0:
            if(mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED){
            	//printf("X: %d\n", mer.dwMousePosition.X);
                gotoxy(mer.dwMousePosition.X, mer.dwMousePosition.Y);
            }
            else if(mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED){
            }
            else{                
            }
            break;
        case DOUBLE_CLICK:            
            break;
        case MOUSE_HWHEELED:            
            break;
        case MOUSE_MOVED:            
            break;
        case MOUSE_WHEELED:        
            break;
        default:
        	printf("UNKNOWN\n");
            break;
    }
}

void gotoxy(int x, int y){
  COORD coord;
  coord.X = (x > 0) ? ((x > lines.it.current->str->size) ? lines.it.current->str->size : x) : 0;
  coord.Y = (y > 0) ? ((y >= lines.size) ? lines.size - 1: y ): 0;
  cursor = coord;
  moveListIterator(&lines, coord.Y);
  moveIterator(lines.it.current->str, x - 1);
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int ascii_to_unicode(unsigned short ch) {
	if (ch < 128) return ch;
	switch (ch) {
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
	case 162: return 243; //ó
	case 161: return 237; //í
	case 198: return 227; //ã
	}
}

char *consoleScan() {
	SetConsoleMode(hStdin, fdwSaveOldMode);
	char *str = (char *) malloc(50);
	scanf("%s", str);
	SetConsoleMode(hStdin, fdwMode);
	return str;
}
