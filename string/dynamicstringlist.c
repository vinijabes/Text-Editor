#include "dynamicstringlist.h"

void InitStringList(DynamicStringList *list){
	list->ini = NULL;
	list->end = NULL;
	list->it.current = NULL;
	list->it.pos = -1;
	list->size = 0;
}

void moveListIterator(DynamicStringList *list, size_t pos){

	if (pos < 0) {
		list->it.pos = -1;
		list->it.current = NULL;
		return;
	}

	if (pos >= list->size) {
		list->it.pos = list->size - 1;
		list->it.current = list->end;
		return;
	}

	if (list->it.current == NULL) {
		list->it.current = list->ini;
		list->it.pos = 0;
	}

	while (pos > list->it.pos) {
		list->it.current = list->it.current->next;
		list->it.pos++;
	}

	while (pos < list->it.pos) {
		list->it.current = list->it.current->prev;
		list->it.pos--;
	}

}

void addString(DynamicStringList *list, DynamicString *string){
	DynamicStringListNode *newNode = (DynamicStringListNode*)malloc(sizeof(DynamicStringListNode));
	newNode->next = NULL;
	newNode->prev = NULL;
	newNode->str= string;

	if (list->size == 0) {
		free(newNode);
		pushString(list, string);
		return;
	}

	if (list->it.pos == -1) {
		newNode->next = list->ini;
		list->ini->prev = newNode;
		list->ini = newNode;
		list->it.current = newNode;
		list->it.pos++;
		++list->size;
		return;
	}

	if (list->it.current->next == NULL) {
		free(newNode);
		pushString(list, string);
		return;
	} else {
		list->it.current->next->prev = newNode;
		newNode->next = list->it.current->next;
		newNode->prev = list->it.current;
		list->it.current->next = newNode;
		list->it.current = newNode;
		list->it.pos++;
	}

	++list->size;
}

void pushString(DynamicStringList *list, DynamicString *string){
	DynamicStringListNode *newNode = (DynamicStringListNode*)malloc(sizeof(DynamicStringListNode));
	newNode->next = NULL;
	newNode->prev = NULL;
	newNode->str = string;

	if (list->ini == NULL) {
		list->ini = newNode;
	}

	if (list->end == NULL) {
		list->end = newNode;
	} else {
		newNode->prev = list->end;
		list->end->next = newNode;
		list->end = newNode;
	}

	++list->size;
	moveListIterator(list, list->size);
}

void printAll(DynamicStringList *list){
	DynamicStringListNode *curr = list->ini;
	while(curr){
		printString(curr->str);
		curr = curr->next;
	}
	
}

void removeString(DynamicStringList *list) {
	if (list->size == 0) {
		return;
	}

	if (list->it.current->prev) {
		list->it.current->prev->next = list->it.current->next;
	}

	if (list->it.current->next) {
		list->it.current->next->prev = list->it.current->prev;
	}

	DynamicStringListNode *aux = list->it.current->prev;
	DynamicStringListNode *aux2 = list->it.current->next;
	free(list->it.current);
	if (list->it.current == list->end) {
		list->end = aux;
	}

	if (list->it.current == list->ini) {
		list->ini = aux2;
	}

	list->it.current = aux;
	--list->it.pos;
	--list->size;
	if (list->size == 0) {
		list->ini = 0;
		list->end = 0;
	}
}