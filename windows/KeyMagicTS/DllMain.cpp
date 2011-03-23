//////////////////////////////////////////////////////////////////////
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//
//  Copyright (C) 2003  Microsoft Corporation.  All rights reserved.
//
//  DllMain.cpp
//
//          DllMain module entry point.
//
//////////////////////////////////////////////////////////////////////

#include "Globals.h"

//+---------------------------------------------------------------------------
//
// DllMain
//
//----------------------------------------------------------------------------
ULONG_PTR m_gdiplusToken;

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID pvReserved)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;

    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:

            g_hInst = hInstance;
			gdiplusStartupInput.GdiplusVersion = 1;
			gdiplusStartupInput.SuppressBackgroundThread = false;
			gdiplusStartupInput.SuppressExternalCodecs = false;
			Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

            if (!InitializeCriticalSectionAndSpinCount(&g_cs, 0))
                return FALSE;

            break;

        case DLL_PROCESS_DETACH:

			//Gdiplus::GdiplusShutdown(m_gdiplusToken);

            DeleteCriticalSection(&g_cs);

            break;
    }

    return TRUE;
}
