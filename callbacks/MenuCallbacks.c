#include "../console/consoleio.h"
#include <stdio.h>

DynamicStringList prevLines;
void openFileMenu() {
	fileMenu = newMenu();
	StateCallbacks *prev = currState;
	prevLines = lines;
	currState = fileMenu->state;
	lines = fileMenu->lines;

	printf("--Arquivo--\n");
	printf("1 - Novo arquivo\n");
	printf("2 - Carregar arquivo\n");
	printf("3 - Salvar arquivo\n");

	registerCallback(currState, VK_ESCAPE, false, false, true, 0.00, closeFileMenu);
	registerCallback(currState, '1', false, false, true, 0.00, newFile);
	registerCallback(currState, '2', false, false, true, 0.00, loadFile);
	registerCallback(currState, '3', false, false, true, 0.00, saveFile);

	openMenu(fileMenu);
	while (isOpenMenu(*fileMenu)) {
		updateMenu(*fileMenu);
	}

	lines = prevLines;
	currState = prev;
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
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a File, yo!";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn))
	{
		printf("%s", filename);
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
		printf("%s", filename);
	}
}
void newFile() {

}