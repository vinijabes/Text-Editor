#ifndef DYNAMIC_STRING_LIST_H
#define DYNAMIC_STRING_LIST_H

#include "dynamicstring.h"

typedef struct stringListNode{
	struct stringListNode *next;
	struct stringListNode *prev;
	DynamicString *str;
} DynamicStringListNode;

typedef struct{
	DynamicStringListNode *current;
	int pos;
} StringListIterator;

typedef struct{
	DynamicStringListNode * ini;
	DynamicStringListNode * end;
	size_t size;
	StringListIterator it;
} DynamicStringList;

void InitStringList(DynamicStringList *list);
void moveListIterator(DynamicStringList *list, size_t pos);

void addString(DynamicStringList *list, DynamicString *string);
void pushString(DynamicStringList *list, DynamicString *string);

void removeString(DynamicStringList *list);

void printAll(DynamicStringList *list);
void freeDynamicStringList(DynamicStringList *list);

#endif
