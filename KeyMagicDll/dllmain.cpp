//Dll entrypoint
//Copyright (C) 2008  KeyMagic Project
//http://keymagic.googlecode.com
//
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include "common.h"

#include "kbdext.h"

HINSTANCE kbdLibrary;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hkl = LoadKeyboardLayout(TEXT("00000409"), 0);
		kbdLibrary = loadKeyboardLayout();
		GetShortCuts();
		break;
	case DLL_PROCESS_DETACH:
		bool ret = UnhookWindowsHookEx(hGetMsgHook);
		Debug(L"UnhookWindowsHookEx(GetMsgHook)=>%x\n", ret);
		ret = UnhookWindowsHookEx(hKeyHook);
		Debug(L"UnhookWindowsHookEx(hKeyHook)=>%x\n", ret);
		ret = UnhookWindowsHookEx(hWndProcHook);
		Debug(L"UnhookWindowsHookEx(hWndProcHook)=>%x\n", ret);
		unloadKeyboardLayout(kbdLibrary);
		//LocalFree(vtSC);
		vtSC.clear();
		//CloseMapping();
		break;
	}
	return TRUE;
}