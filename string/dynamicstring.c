#include "dynamicstring.h"

void InitString(DynamicString *str){
	str->ini = NULL;
	str->end = NULL;
	str->it.current = NULL;
	str->it.pos = 0;
	str->size = 0;
}

void moveIterator(DynamicString *str, size_t pos){
	if(str->it.current == NULL){
		if(pos > str->size){
			str->it.current = str->end;
			str->it.pos = SIZE_MAX;
			return;
		}
		
		if(pos < str->size/2){
			str->it.current = str->ini;
			str->it.pos = 0;
			while(str->it.current && str->it.pos < pos){
				str->it.current = str->it.current->next;
				++str->it.pos;
			}
			--str->it.pos;
		}else{
			str->it.current = str->end;
			str->it.pos = str->size - 1;
			
			if(str->it.current == NULL) return;
			
			while(str->it.pos > pos){
				str->it.current = str->it.current->prev;
				--str->it.pos;
			}			
			
			if(str->it.pos < 0){
				str->it.pos = 0;
			}
		}
	}else{
		if(str->it.pos > pos){
			while(str->it.pos > pos){
				if(str->it.current->prev)
					str->it.current = str->it.current->prev;
				--str->it.pos;
			}
		}else if(str->it.pos < pos){
			while(str->it.current && str->it.pos < pos){
				str->it.current = str->it.current->next;
				++str->it.pos;
			}
			--str->it.pos;	
			if(str->it.pos < 0){
				str->it.pos = 0;
			}
		}
	}	
}

void addCharacter(DynamicString *str, char ch){
	StringCharacter *newCh = (StringCharacter*) malloc(sizeof(StringCharacter));
	newCh->next = NULL;
	newCh->prev = NULL;
	newCh->ch = ch;
	
	if(str->it.pos == SIZE_MAX){
		free(newCh);
		pushCharacter(str, ch);
		return;
	}
	
	if(str->it.current != NULL){
		newCh->next = str->it.current;
		newCh->prev = str->it.current->prev;
		if(str->it.current->prev){
			str->it.current->prev->next = newCh;
		}else{
			str->ini = newCh;
		}
		str->it.current->prev = newCh;
		++str->it.pos;
	}else if(!str->ini){
		str->ini = newCh;
		str->end = newCh;
		str->it.current = newCh;
		str->it.pos = 0;
	}
	
	++str->size;
}

void pushCharacter(DynamicString *str, char ch){
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
}

void insertCharacter(DynamicString *str, char ch){
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

void printString(DynamicString *str){
	StringCharacter *ini = str->ini;
	while(ini){
		putch(ini->ch);
		ini = ini->next;
	}
}


