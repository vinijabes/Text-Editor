#ifndef OUTPUT_HANDLER_H
#define OUTPUT_HANDLER_H

#include <windows.h>

COORD consoleBuffer;

void initOutput();
void handleOutput();

void setAtributtes(WORD attr);
void setDefaultAttributes();
void setBufferSize(int x, int y);

#endif
