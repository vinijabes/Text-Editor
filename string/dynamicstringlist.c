#include "dynamicstringlist.h"

void InitStringList(DynamicStringList *list){
	list->ini = NULL;
	list->end = NULL;
	list->it.current = NULL;
	list->it.pos = 0;
	list->size = 0;
}

void moveListIterator(DynamicStringList *list, size_t pos){
	if(pos == 0){
		list->it.pos = 0;
		list->it.current = list->ini;
	}
	
	if(pos > list->size-1){
		list->it.pos = SIZE_MAX;
		list->it.current = list->end;
		return;
	}
	
	if(list->it.current == NULL){
		if(list->size > pos/2){
			
			list->it.current = list->end;
			list->it.pos = list->size-1;
			
			while(list->it.current && list->it.pos > pos){
				list->it.current = list->it.current->prev;
				--list->it.pos;
			}
			
		}else if(list->size < pos/2){
			
			list->it.current = list->ini;
			list->it.pos = 0;
			
			while(list->it.current && list->it.pos < pos){
				list->it.current = list->it.current->next;
				++list->it.pos;
			}
			
		}else{
			if(list->it.pos < 0) list->it.pos = 0;
			return;
		}
	}else{
		if(list->it.pos == SIZE_MAX){
			list->it.pos = list->size - 1;
		}
		
		if(list->it.pos < pos){
			while(list->it.current && list->it.pos < pos){
				list->it.current = list->it.current->next;
				++list->it.pos;
			}
		}else if(list->it.pos > pos){
			while(list->it.current && list->it.pos > pos){
				list->it.current = list->it.current->prev;
				--list->it.pos;
			}
		}else{
			if(list->it.pos < 0) list->it.pos = 0;
			return;
		}
	}
	
	if(list->it.pos < 0) list->it.pos = 0;
}

void addString(DynamicStringList *list, DynamicString *string){
	DynamicStringListNode *newNode = (DynamicStringListNode*) malloc(sizeof(DynamicStringListNode));
	newNode->next = NULL;
	newNode->prev = NULL;
	newNode->str = string;
	
	
	if(list->it.pos == SIZE_MAX){
		free(newNode);
		pushString(list, string);
		return;
	}
	
	if(list->it.current != NULL){
		newNode->next = list->it.current;
		newNode->prev = list->it.current->prev;
		if(list->it.current->prev){
			list->it.current->prev->next = newNode;
		}else{
			list->ini = newNode;
		}
		list->it.current->prev = newNode;
		++list->it.pos;
	}else if(!list->ini){
		list->ini = newNode;
		list->end = newNode;
		list->it.current = newNode;
		list->it.pos = SIZE_MAX;
	}
	
	++list->size;
}

void pushString(DynamicStringList *list, DynamicString *string){
	DynamicStringListNode *newNode = (DynamicStringListNode*) malloc(sizeof(DynamicStringListNode));
	newNode->next = NULL;
	newNode->prev = NULL;
	newNode->str = string;

	if(list->ini == NULL){
		list->ini = newNode;
	}
	
	if(list->end == NULL){
		list->end = newNode;
	}else{
		newNode->prev = list->end;
		list->end->next = newNode;
		list->end = newNode;
	}
	
	if(list->it.current == NULL){
		list->it.current = list->end;
		list->it.pos = SIZE_MAX;
	}
	
	++list->size;
}

void printAll(DynamicStringList *list){
	DynamicStringListNode *curr = list->ini;
	while(curr){
		printString(curr->str);
		curr = curr->next;
	}
	
}
