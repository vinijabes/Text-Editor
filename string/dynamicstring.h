#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

#include <stdlib.h>

typedef struct ch{
	unsigned short ch;
	struct ch *next;
	struct ch *prev;
} StringCharacter;

typedef struct it{
	StringCharacter *current;
	size_t pos;
} StringIterator;

typedef struct str{
	StringCharacter *ini;
	StringCharacter *end;
	size_t size;
	StringIterator it;
} DynamicString;


void InitString(DynamicString *str);
void moveIterator(DynamicString *str, size_t pos);
void addCharacter(DynamicString *str, char ch);
void pushCharacter(DynamicString *str, char ch);
void insertCharacter(DynamicString *str, char ch);

void addCharArray(DynamicString *str, char *ch);
void pushCharArray(DynamicString *str, char *ch);
void insertCharArray(DynamicString *str, char *ch);



void printString(DynamicString *str);


#endif
