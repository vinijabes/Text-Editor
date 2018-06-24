#include "../console/consoleio.h"
#include <stdio.h>

DynamicStringList prevLines;
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
}
void closeFileMenu() {
	closeMenu(fileMenu);
}

void loadFile() {
	CHAR filename[MAX_PATH+10];
	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
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
		while ((ch = fgetc(f)) != EOF) {
			if (ch != '\n') {
				pushCharacter(str, ch);
			}else{
				pushCharacter(str, ch);
				str = newLine();
				pushString(&fileMenu->lines, str);
			}
		}
		prevLines = fileMenu->lines;
		closeFileMenu();
	}

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
	ofn.lpstrTitle = "Select a File, yo!";
	ofn.Flags = OFN_DONTADDTORECENT;

	if (GetOpenFileNameA(&ofn))
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
void newFile() {

}