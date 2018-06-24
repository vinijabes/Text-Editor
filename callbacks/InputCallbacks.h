#ifndef INPUT_CALLBACKS_H
#define INPUT_CALLBACKS_H

void RegisterAllInputCallbacks();

void MoveRightCB();
void MoveLeftCB();
void MoveUpCB();
void MoveDownCB();
void MoveToLineInit();
void Highlight();

void clean();
void printList();
void breakLineCB();
void writeToFile();

void unwriteLineAfterIterator(DynamicString *str);
#endif
