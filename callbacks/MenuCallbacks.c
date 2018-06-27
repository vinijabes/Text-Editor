#include "../console/consoleio.h"
#include <stdio.h>
#include <ctype.h>

DynamicStringList prevLines;
extern int outputLine;
void openFileMenu() {
	fileMenu = newMenu();
	StateCallbacks *prev = currState;
	prevLines = lines;
	currState = fileMenu->state;
	lines = fileMenu->lines;
	system("cls");
	printf("--Arquivo--\n");
	printf("1 - Novo arquivo\n");
	printf("2 - Carregar arquivo\n");
	printf("3 - Salvar arquivo\n");

	registerCallback(currState, VK_ESCAPE, false, false, true, 0.00, closeFileMenu);
	registerCallback(currState, '1', false, false, true, 0.00, newFile);
	registerCallback(currState, '2', false, false, true, 0.00, loadFile);
	registerCallback(currState, '3', false, false, true, 0.00, saveFile);

	setOutputState(false);
	openMenu(fileMenu);
	while (isOpenMenu(*fileMenu)) {
		updateMenu(*fileMenu);
	}
	setOutputState(true);
	system("cls");

	lines = prevLines;
	currState = prev;

	outputLine = 0;
	outputLineEnd = lines.size;
	setConsoleDefaultInputMode();
}
void closeFileMenu() {
	closeMenu(fileMenu);
}

void loadFile() {
	CHAR filename[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Selecione um arquivo!";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn))
	{
		FILE *f = fopen(filename, "r");
		DynamicStringList aux = fileMenu->lines;
		DynamicString *str = aux.ini->str;
		short ch;
		int maxSize = 0;
		while ((ch = unicode_to_ascii(fgetc(f))) != EOF) {
			if (ch != '\n' && ch != '\t') {
				pushCharacter(str, ch);
			} else if (ch == '\t') {
				pushCharacter(str, ' ');
				pushCharacter(str, ' ');
				pushCharacter(str, ' ');
				pushCharacter(str, ' ');
			} else {
				pushCharacter(str, ch);
				maxSize = max(str->size, maxSize);
				str = newLine();
				pushString(&fileMenu->lines, str);
			}
		}
		prevLines = fileMenu->lines;
		setBufferSize(maxSize, prevLines.size);
		closeFileMenu();
		outputLine = 0;
		outputLineEnd = prevLines.size + 1;
		freeStack(currStack);
		currStack = initStack();
		free(tempNode);
		tempNode = NULL;
	}

}

void loadFiled() {
	fileMenu = newMenu();
	prevLines = lines;
	lines = fileMenu->lines;

	loadFile();

	lines = prevLines;

	outputLine = 0;
	setConsoleDefaultInputMode();
	freeStack(currStack);
	currStack = initStack();
	free(tempNode);
	tempNode = NULL;
}

//char lastfilename[MAX_PATH];

void saveFile() {
	CHAR filename[MAX_PATH];
	//if (strlen(lastfilename)) strcpy(filename, lastfilename);
	//else strcpy(lastfilename, filename);

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Selecione o local do arquivo";
	ofn.Flags = OFN_DONTADDTORECENT;

	if (GetSaveFileNameA(&ofn))
	{
		FILE *f = fopen(filename, "w");
		DynamicStringListNode *aux = prevLines.ini;
		while (aux) {
			StringCharacter *aux2 = aux->str->ini;
			while (aux2) {				
				fputc(ascii_to_unicode(aux2->ch), f);
				aux2 = aux2->next;
			}
			aux = aux->next;
		}
		fclose(f);
	}
}

void saveFiled() {
	fileMenu = newMenu();
	prevLines = lines;
	lines = fileMenu->lines;
	saveFile();
	lines = prevLines;
	setConsoleDefaultInputMode();	
	freeStack(currStack);
	currStack = initStack();
}

void newFile() {
	freeDynamicStringList(&lines);
	InitStringList(&lines);
	addString(&lines, newLine());
	system("cls");
	setConsoleDefaultInputMode();
	outputLine = 0;
	outputLineEnd = 1;
	gotoxy(0, 0);
	freeStack(currStack);
	currStack = initStack();
	free(tempNode);
	tempNode = NULL;
	closeFileMenu();
}

void newFiled() {
	freeDynamicStringList(&lines);
	InitStringList(&lines);
	addString(&lines, newLine());
	system("cls");
	setConsoleDefaultInputMode();
	outputLine = 0;
	outputLineEnd = 1;
	gotoxy(0, 0);
	free(tempNode);
	tempNode = NULL;
}

void openSearchMenu() {
	searchMenu = newMenu();
	StateCallbacks *prev = currState;
	currState = searchMenu->state;
	prevLines = lines;
	lines = searchMenu->lines;
	clearInputBuffer();
	system("cls");
	printf("Pesquisar por: ");
	char * str = consoleScan();
	system("cls");
	setConsoleDefaultInputMode();

	registerCallback(currState, VK_ESCAPE, false, false, true, 0.5, closeSearchMenu);

	registerCallback(currState, VK_RIGHT, false, false, true, 0.00, MoveRightCB);
	registerCallback(currState, VK_LEFT, false, false, true, 0.00, MoveLeftCB);

	registerCallback(currState, VK_HOME, false, false, true, 0.05, MoveToLineInit);
	registerCallback(currState, VK_END, false, false, true, 0.05, MoveToLineEnd);
	registerCallback(currState, VK_LEFT, true, false, true, 0.00, shiftLeftSelection);
	registerCallback(currState, VK_RIGHT, true, false, true, 0.00, shiftRightSelection);
	registerCallback(currState, VK_UP, true, false, true, 0.00, shiftUpSelection);
	registerCallback(currState, VK_DOWN, true, false, true, 0.00, shiftDownSelection);
	registerCallback(currState, 'C', false, true, true, 0.00, copySelection);
	registerCallback(currState, 'V', false, true, true, 0.50, pasteClip);
	registerCallback(currState, 'Z', false, true, true, 0.40, undoAction);
	registerCallback(currState, 'S', false, true, true, 0.50, saveFiled);
	registerCallback(currState, 'O', false, true, true, 0.50, loadFiled);

	//registerCallback(currState, VK_LEFT, true, false, true, 1.00, SelectLeftCB);
	registerCallback(currState, VK_UP, false, false, true, 0.05, MoveUpCB);
	registerCallback(currState, VK_DOWN, false, false, true, 0.05, MoveDownCB);

	registerCallback(currState, VK_BACK, false, false, true, 0.05, Backspace);
	registerCallback(currState, VK_DELETE, false, false, true, 0.05, Delete);
	registerCallback(currState, 'T', false, true, true, 0.05, clean);
	registerCallback(currState, 'Y', false, true, true, 0.05, printList);
	registerCallback(currState, 'U', false, true, true, 0.05, writeToFile);
	registerCallback(currState, 13, false, false, true, 0.05, breakLineCB);

	int posAux = 0;
	int line = 0;
	int ocurrences = 0;
	int size = strlen(str);
	DynamicStringListNode *aux = prevLines.ini;
	while (aux) {
		line++;
		StringCharacter *ch = aux->str->ini;
		while (ch) {
			if (tolower(ch->ch) == tolower(str[posAux])) {
				++posAux;
			}else{
				posAux = 0;
			}
			if (posAux == size) {
				DynamicString *str = newLine();
				copyString(str, aux->str);
				moveIterator(str, -1);
				char strAux[20];
				sprintf(strAux, "Linha: %d - ", line);
				addCharArray(str, strAux);
				addString(&lines, str);
				++ocurrences;
				posAux = 0;
			}
			ch = ch->next;
		}
		aux = aux->next;
	}

	moveListIterator(&lines, -1);
	int strSize = strlen(str);
	char *buff = (char*)malloc(strSize + 30);
	buff[strSize + 29] = '\0';
	sprintf(buff, "%d ocorrencias de '%s'", ocurrences, str);
	addCharArray(lines.ini->str, buff);
	int prevOutputLine = outputLine;
	int prevOutputLineEnd = outputLineEnd;

	openMenu(searchMenu);
	gotoxy(0, 0);
	outputLine = 0;
	outputLineEnd = lines.size;
	while (isOpenMenu(*searchMenu)) {
		handleInput();
		handleOutput();
	}

	system("cls");

	lines = prevLines;
	currState = prev;
	outputLine = 0;
	outputLineEnd = lines.size;
	freeMenu(searchMenu);
}

void closeSearchMenu() {
	closeMenu(searchMenu);
}

void openHelpMenu() {
	helpMenu = newMenu();
	StateCallbacks *prev = currState;
	currState = helpMenu->state;

	registerCallback(currState, VK_ESCAPE, false, false, true, 0.00, closeHelpMenu);

	system("cls");
	printf("Editor de texto Sublimerson 1.0\n");
	printf("Lista de comandos:\nF1-Menu de arquivos\nF2-Menu de ajuda\nCntrl-C: Copia\nCntrl-V: Cola\nCntrl-Z: Desfazer\nCntrl-F: Pesquisar por\nEscape- Sair");

	setOutputState(false);
	openMenu(helpMenu);
	while (isOpenMenu(*helpMenu)) {
		updateMenu(*helpMenu);
	}
	setOutputState(true);
	
	system("cls");

	outputLine = 0;
	currState = prev;
	freeMenu(helpMenu);
}
void closeHelpMenu() {
	closeMenu(helpMenu);
}