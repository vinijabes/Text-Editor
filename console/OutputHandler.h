#ifndef OUTPUT_HANDLER_H
#define OUTPUT_HANDLER_H

#include <windows.h>
#include "InputHandler.h"

COORD consoleBuffer;
bool outputActive;

void initOutput();
void handleOutput();

void setAttributes(WORD attr);
void setDefaultAttributes();
void setBufferSize(int x, int y);

void setOutputState(bool state);

void outputChar(char ch);
void outputCharArray(char *ch);
void outputString(DynamicString *string);

int outputLine;

#endif
