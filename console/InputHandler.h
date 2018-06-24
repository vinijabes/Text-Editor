#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#define NO_INPUT_KEY -1
#define KEY_STROKE_INTERVAL 10

#define SHIFT_CALLBACK 0
#define CNTRL_CALLBACK 1
#define SHIFT_CNTRL_CALLBACK 2
#define KEYSTROKE_CALLBACK 3
#define CALLBACK_LIST_COUNT 4

#include <windows.h>
#include <time.h>
#include "../string/dynamicstring.h"
#include "../string/dynamicstringlist.h"

COORD cursor;

DynamicStringList lines;

typedef enum{
	false, true
} bool;

typedef struct{
	bool shift;
	bool cntrl;
	bool keyDown;
	float interval;
	clock_t lastCall;
	int key;
	void (*cb)();
} InputCallback;

typedef struct inputcbnode{
	InputCallback callback;
	struct inputcbnode * next;
} InputCallbackListNode;

typedef struct{
	InputCallbackListNode * init;
	InputCallbackListNode * end;
} InputCallbackList;

typedef struct{
	InputCallbackList callbacks[CALLBACK_LIST_COUNT];
} StateCallbacks;

StateCallbacks *currState;

void initInput();

bool keyPressed(int key);
bool keyReleased(int key);
void registerCallback(StateCallbacks* state, int key, bool shift, bool cntrl, bool keyDown, float interval, void (*cb)());

StateCallbacks *newState();
void initInputCallbackList(InputCallbackList* list);
void pushCallback(InputCallbackList* list, int key, bool shift, bool cntrl, bool keyDown, float interval, void (*cb)());
void runAllCallbacks();
void handleInput();
bool handleKeyboard();
bool handleKeystroke();
void clearInputBuffer();
void handleMouse(MOUSE_EVENT_RECORD mer);

void gotoxy(int x, int y);

int ascii_to_unicode(unsigned short ch);

char *consoleScan();
#endif
