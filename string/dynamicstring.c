#include "dynamicstring.h"
#include <stdio.h>

void InitString(DynamicString *str){
	str->ini = NULL;
	str->end = NULL;
	str->it.current = NULL;
	str->it.pos = -1;
	str->size = 0;
}

void moveIterator(DynamicString *str, int pos){
	if(pos < 0){
		str->it.pos = -1;
		str->it.current = NULL;
		return;
	}
	
	if(pos >= str->size){		
		str->it.pos = str->size - 1;
		str->it.current = str->end;
		return;
	}

	if(str->it.current == NULL){
		str->it.current = str->ini;
		str->it.pos = 0;
	}

	while(pos > str->it.pos){
		str->it.current = str->it.current->next;
		str->it.pos++;
	}
	
	while(pos < str->it.pos){
		str->it.current = str->it.current->prev;
		str->it.pos--;
	}
}

void addCharacter(DynamicString *str, int ch){
	StringCharacter *newCh = (StringCharacter*) malloc(sizeof(StringCharacter));
	newCh->next = NULL;
	newCh->prev = NULL;
	newCh->ch = ch;
	
	if(str->size == 0){
		free(newCh);
		pushCharacter(str, ch);
		return;
	}
	
	if(str->it.pos == -1){
		newCh->next = str->ini;
		str->ini->prev = newCh;
		str->ini = newCh;
		str->it.current = newCh;
		str->it.pos++;
		++str->size;
		return;
	}
	
	if(str->it.current->next == NULL){
		free(newCh);
		pushCharacter(str, ch);		
		return;
	}else{
		str->it.current->next->prev = newCh;
		newCh->next = str->it.current->next;
		newCh->prev = str->it.current;
		str->it.current->next = newCh;
		str->it.current = newCh;
		str->it.pos++;
	}
	
	++str->size;
}

void pushCharacter(DynamicString *str, int ch){
	StringCharacter *newCh = (StringCharacter*) malloc(sizeof(StringCharacter));
	newCh->next = NULL;
	newCh->prev = NULL;
	newCh->ch = ch;

	if(str->ini == NULL){
		str->ini = newCh;
	}
	
	if(str->end == NULL){
		str->end = newCh;
	}else{
		newCh->prev = str->end;
		str->end->next = newCh;
		str->end = newCh;
	}
	
	++str->size;
	moveIterator(str, str->size);
}

void insertCharacter(DynamicString *str, int ch){
	if(str->ini == NULL || str->it.pos == SIZE_MAX){
		pushCharacter(str, ch);
		return;
	}
	
	if(str->it.current == NULL){
		str->it.pos = SIZE_MAX;
		pushCharacter(str, ch);
		return;
	}
	
	str->it.current->ch = ch;
	str->it.current = str->it.current->next;
	++str->it.pos;
}

void removeCharacter(DynamicString *str) {
	if (str->size == 0) {
		return;
	}
	
	if(str->it.current->prev) {
		str->it.current->prev->next = str->it.current->next;
	}

	if (str->it.current->next) {
		str->it.current->next->prev = str->it.current->prev;
	}
	
	StringCharacter *aux = str->it.current->prev;
	StringCharacter *aux2 = str->it.current->next;
	free(str->it.current);
	if (str->it.current == str->end) {
		str->end = aux;
	}

	if (str->it.current == str->ini) {
		str->ini = aux2;
	}

	str->it.current = aux;
	--str->it.pos;
	--str->size;
	if (str->size == 0) {
		str->ini = 0;
		str->end = 0;
	}
}

void addCharArray(DynamicString *str, char *ch){
	while(*ch != '\0'){
		addCharacter(str, *ch);
		ch++;
	}
}

void pushCharArray(DynamicString *str, char *ch){
	while(*ch != '\0'){
		pushCharacter(str, *ch);
		ch++;
	}
}

void insertCharArray(DynamicString *str, char *ch){
	while(*ch != '\0'){
		insertCharacter(str, *ch);
		ch++;
	}
}

int dynamicStringLen(DynamicString *str) {
	StringCharacter *aux = str->ini;
	int x = 0;
	while (aux) {
		++x;
		aux = aux->next;
	}

	return x;
}

DynamicString * breakString(DynamicString *str, int pos){
	DynamicString * newStr = newLine();
	
	moveIterator(str, pos);
	if (str->it.current && str->it.current->next && str->it.current->next->ch == '\n') {
		pushCharacter(newStr, '\n');
		return newStr;
	}
	addCharacter(str, '\n');
	if(str->it.pos == str->size-1) return newLine();

	newStr->ini = str->it.current->next;
	newStr->end = str->end;
	newStr->ini->prev = NULL;
	
	str->end = str->it.current;
	str->end->next = NULL;
	str->it.current = NULL;
	str->it.pos = -1;
	

	int aux = str->size;
	if(pos > str->size && pos != -1){
		newStr->size = 0;
	}else{
		str->size = dynamicStringLen(str);
		newStr->size = dynamicStringLen(newStr);
	}
	
	return newStr;
}

void printString(DynamicString *str){
	StringCharacter *ini = str->ini;
	while(ini){
		putchar(ini->ch);
		ini = ini->next;
	}
}

void printStringIt(DynamicString *str) {
	StringCharacter *ini;
	if (str->it.current) {	
		ini = str->it.current->next;
	} else {
		ini = str->ini;
	}

	while (ini) {
		putchar(ini->ch);
		ini = ini->next;
	}
}

DynamicString * newLine(){
	DynamicString * newStr = (DynamicString *) malloc(sizeof(DynamicString));
	InitString(newStr);
	return newStr;
}

void concatString(DynamicString *dest, DynamicString *src) {
	if (dest->end->ch == '\n') {
		moveIterator(dest, dest->size - 1);
		removeCharacter(dest);
	}
	
	if (dest->size > 0) {
		dest->end->next = src->ini;
		src->ini->prev = dest->end;
		dest->size += src->size;
	}else{
		*dest = *src;
	}
}
