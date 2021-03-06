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
	int pos;
} StringIterator;

typedef struct str{
	StringCharacter *ini;
	StringCharacter *end;
	size_t size;
	StringIterator it;
} DynamicString;


void InitString(DynamicString *str);
void moveIterator(DynamicString *str, int pos);
void addCharacter(DynamicString *str, int ch);
void pushCharacter(DynamicString *str, int ch);
void insertCharacter(DynamicString *str, int ch);

void addCharArray(DynamicString *str, char *ch);
void pushCharArray(DynamicString *str, char *ch);
void insertCharArray(DynamicString *str, char *ch);
void removeCharArray(DynamicString *str);

void removeCharacter(DynamicString *str);

DynamicString * breakString(DynamicString *str, int pos);
DynamicString * newLine();
int dynamicStringLen(DynamicString *str);

void concatString(DynamicString *dest, DynamicString *src);

void printString(DynamicString *str);
void printStringIt(DynamicString *str);


#endif
