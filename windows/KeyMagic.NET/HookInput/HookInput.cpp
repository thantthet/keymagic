// HookInput.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <InputProcessor.h>

#ifndef DllName
#ifdef WIN32
#define DllName "InputProcessor.x32.dll"
#else
#define DllName "InputProcessor.x64.dll"
#endif
#endif

bool Hook(HWND hWnd)
{
	WCHAR dirName[MAX_PATH];

	SetMainWindowsHandle(hWnd);

	GetModuleFileNameW(NULL, dirName, MAX_PATH);
	PathRemoveFileSpecW(dirName);
	SetMainDir(dirName);

	HMODULE hModule = LoadLibrary(DllName);
	//SetWindowsHooks(hModule);

	if (GetKeyProcHook() == 0 || GetWndProcHook() == 0 || GetGetMsgProcHook() == 0) {
		return false;
	}
}

void myFunction(int i)
{
	printf("Program was trying to abort or terminate.");
}

int _tmain(int argc, _TCHAR* argv[])
{
	HWND hwndMain;

	signal(SIGINT, myFunction);

	if (argc < 2) {
		return 1;
	}

	sscanf_s(argv[1], "%x", &hwndMain);
	if (hwndMain == 0) {
		return 1;
	}

	Hook(hwndMain);

	getchar();

	return 0;
}

