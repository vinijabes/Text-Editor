#include "../console/consoleio.h"
#include <stdio.h>

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
		while ((ch = fgetc(f)) != EOF) {
			if (ch != '\n') {
				pushCharacter(str, ch);
			}else{
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
}

void saveFile() {
	CHAR filename[MAX_PATH];
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
}

void newFile() {

}