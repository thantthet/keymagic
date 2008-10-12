//Like a operator of Keymagic.
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

#include "MyMenu.h"
#include "MyButton.h"
#include "WndProc.h"
#include "DllUnload.h"
#include "../KeyMagicDll/KeyMagicDll.h"

TCHAR szKBP[]="KeyBoardPaths";
TCHAR szMS[]="MenuDisplays";
TCHAR szSC[]="ShortCuts";
TCHAR szNeedRestart[] = "Application needs to restart to work correctly";
TCHAR szKeymagic[] = "Keymagic";

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];

#pragma data_seg(".keymagic")
HHOOK hKH = NULL;
HHOOK hWPH = NULL;
HHOOK hGM = NULL;
#pragma data_seg()

//Make sure that section can READ WRITE and SHARE
#pragma comment(linker, "/SECTION:.keymagic,RWS")

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
ATOM MyRegisterClass(HINSTANCE hInstance);

HMENU hKeyMenu;
char szINIFile[MAX_PATH];
char szCurDir[MAX_PATH];
BOOL bAdmin;

char szMainClassName[] = "Keymagic";
HWND hwndMain;
HACCEL hAccel;

ULONG_PTR m_gdiplusToken;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	OSVERSIONINFO osvi;
	BOOL bIsWindowsVistaLater;

	hInst = hInstance;

	GetModuleFileName(hInst, szCurDir, MAX_PATH);

	int i;
	for (i=lstrlen(szCurDir); szCurDir[i] != '\\'; i--){}

	szCurDir[i] = NULL;

	lstrcpy(szINIFile,szCurDir);

	PathAppend(szINIFile, "KeyMagic.ini");

	bAdmin = IsAdmin();

	if (WorkOnCommand(lpCmdLine))
		return 0;

	LoadString(hInst, IDS_EN_TITLE, szTitle, MAX_LOADSTRING);

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);

    bIsWindowsVistaLater = osvi.dwMajorVersion >= 6;

	if (bAdmin && bIsWindowsVistaLater)
		lstrcat(szTitle, " (Administrator)");

	if (OpenMutexW(SYNCHRONIZE, NULL, L"\u1000\u102E\u1038\u1019\u1000\u1039\u1002\u103A\u1005\u1039")){
		HWND hPreHandle = FindWindow(NULL, szTitle);
		ShowWindow(hPreHandle, SW_SHOW);
		SetForegroundWindow(hPreHandle);
		return 0;
	}

	HANDLE MtxHANDLE = CreateMutexW(NULL, TRUE, L"\u1000\u102E\u1038\u1019\u1000\u1039\u1002\u103A\u1005\u1039");

	MSG messages;
    WNDCLASSEX wincl;

    hInst = hInstance;
    wincl.hInstance = hInstance;
    wincl.lpszClassName = szMainClassName;
    wincl.lpfnWndProc = WndProc;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

	wincl.hIcon = LoadIcon (hInst, MAKEINTRESOURCE(IDI_KEYMAGIC));
    wincl.hIconSm = LoadIcon (hInst, MAKEINTRESOURCE(IDI_KEYMAGIC));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
	wincl.hbrBackground = CreateSolidBrush(RGB(236, 244, 255));

    RegisterClassEx (&wincl);

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

    hwndMain = CreateWindowEx (
        0,szMainClassName, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,CW_USEDEFAULT,
        580, 480,
        HWND_DESKTOP,NULL,
        hInst,NULL
		);

	if (!hwndMain)
	{
		ReleaseMutex(MtxHANDLE);
		return 0;
	}

	ShowWindow (hwndMain, SW_HIDE);

    hAccel = LoadAccelerators(hInst, "ACCLC"); 

    while (GetMessage (&messages, NULL, 0, 0))
    {
        if (!IsDialogMessage (NULL, &messages))
        {
			if(!TranslateAccelerator(hwndMain, hAccel, &messages)){
				TranslateMessage(&messages);
				DispatchMessage(&messages);
			}
		}
	}

	Gdiplus::GdiplusShutdown(m_gdiplusToken);
	ReleaseMutex(MtxHANDLE);
	return (int) 0;
}

BOOL IsAdmin()
{
   BOOL   fReturn         = FALSE;
   DWORD  dwStatus;
   DWORD  dwAccessMask;
   DWORD  dwAccessDesired;
   DWORD  dwACLSize;
   DWORD  dwStructureSize = sizeof(PRIVILEGE_SET);
   PACL   pACL            = NULL;
   PSID   psidAdmin       = NULL;

   HANDLE hToken              = NULL;
   HANDLE hImpersonationToken = NULL;

   PRIVILEGE_SET   ps;
   GENERIC_MAPPING GenericMapping;

   PSECURITY_DESCRIPTOR     psdAdmin           = NULL;
   SID_IDENTIFIER_AUTHORITY SystemSidAuthority = SECURITY_NT_AUTHORITY;


   /*
      Determine if the current thread is running as a user that is a member of
      the local admins group.  To do this, create a security descriptor that
      has a DACL which has an ACE that allows only local aministrators access.
      Then, call AccessCheck with the current thread's token and the security
      descriptor.  It will say whether the user could access an object if it
      had that security descriptor.  Note: you do not need to actually create
      the object.  Just checking access against the security descriptor alone
      will be sufficient.
   */
   const DWORD ACCESS_READ  = 1;
   const DWORD ACCESS_WRITE = 2;


   __try
   {

      /*
         AccessCheck() requires an impersonation token.  We first get a primary
         token and then create a duplicate impersonation token.  The
         impersonation token is not actually assigned to the thread, but is
         used in the call to AccessCheck.  Thus, this function itself never
         impersonates, but does use the identity of the thread.  If the thread
         was impersonating already, this function uses that impersonation context.
      */
      if (!OpenThreadToken(GetCurrentThread(), TOKEN_DUPLICATE|TOKEN_QUERY, 
		  TRUE, &hToken))
      {
         if (GetLastError() != ERROR_NO_TOKEN)
            __leave;

         if (!OpenProcessToken(GetCurrentProcess(), 
			 TOKEN_DUPLICATE|TOKEN_QUERY, &hToken))
            __leave;
      }

      if (!DuplicateToken (hToken, SecurityImpersonation, 
		  &hImpersonationToken))
		  __leave;


      /*
        Create the binary representation of the well-known SID that
        represents the local administrators group.  Then create the security
        descriptor and DACL with an ACE that allows only local admins access.
        After that, perform the access check.  This will determine whether
        the current user is a local admin.
      */
      if (!AllocateAndInitializeSid(&SystemSidAuthority, 2,
                                    SECURITY_BUILTIN_DOMAIN_RID,
                                    DOMAIN_ALIAS_RID_ADMINS,
                                    0, 0, 0, 0, 0, 0, &psidAdmin))
         __leave;

      psdAdmin = LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
      if (psdAdmin == NULL)
         __leave;

      if (!InitializeSecurityDescriptor(psdAdmin, 
		  SECURITY_DESCRIPTOR_REVISION))
         __leave;

      // Compute size needed for the ACL.
      dwACLSize = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) +
                  GetLengthSid(psidAdmin) - sizeof(DWORD);

      pACL = (PACL)LocalAlloc(LPTR, dwACLSize);
      if (pACL == NULL)
         __leave;

      if (!InitializeAcl(pACL, dwACLSize, ACL_REVISION2))
         __leave;

      dwAccessMask= ACCESS_READ | ACCESS_WRITE;

      if (!AddAccessAllowedAce(pACL, ACL_REVISION2, dwAccessMask, 
		  psidAdmin))
         __leave;

      if (!SetSecurityDescriptorDacl(psdAdmin, TRUE, pACL, FALSE))
         __leave;

      /*
         AccessCheck validates a security descriptor somewhat; set the group
         and owner so that enough of the security descriptor is filled out to
         make AccessCheck happy.
      */
      SetSecurityDescriptorGroup(psdAdmin, psidAdmin, FALSE);
      SetSecurityDescriptorOwner(psdAdmin, psidAdmin, FALSE);

      if (!IsValidSecurityDescriptor(psdAdmin))
         __leave;

      dwAccessDesired = ACCESS_READ;

      /*
         Initialize GenericMapping structure even though you
         do not use generic rights.
      */
      GenericMapping.GenericRead    = ACCESS_READ;
      GenericMapping.GenericWrite   = ACCESS_WRITE;
      GenericMapping.GenericExecute = 0;
      GenericMapping.GenericAll     = ACCESS_READ | ACCESS_WRITE;

      if (!AccessCheck(psdAdmin, hImpersonationToken, dwAccessDesired,
                       &GenericMapping, &ps, &dwStructureSize, &dwStatus,
                       &fReturn))
      {
         fReturn = FALSE;
         __leave;
      }

   }

   __finally
   {
      // Clean up.
      if (pACL) LocalFree(pACL);
      if (psdAdmin) LocalFree(psdAdmin);
      if (psidAdmin) FreeSid(psidAdmin);
      if (hImpersonationToken) CloseHandle (hImpersonationToken);
      if (hToken) CloseHandle (hToken);
   }

   return fReturn;

}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		char* buildTime[50];
		lstrcpyA((LPSTR)buildTime,__DATE__);
		lstrcatA((LPSTR)buildTime," - ");
		lstrcatA((LPSTR)buildTime,__TIME__);
		SendDlgItemMessageA(hDlg, IDC_COMPLIE, WM_SETTEXT, 0, (LPARAM)buildTime);
		SendDlgItemMessage(hDlg, IDC_ATEXT, WM_SETTEXT, 0, (LPARAM)
			"\nCopyright (C) 2008  KeyMagic Project\n"
			"http://keymagic.googlecode.com\n\n"

			"This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation.\n\n"

			"This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n\n"

			"You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA"
			);

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

VOID SetHook (HWND hwnd)
{

	HMODULE hMod = LoadLibrary("KeyMagicDll.dll");
	if (hMod == NULL)   {
			MessageBox(hwnd, "Log", "Error: cannot start dll, KeyMagicDll.dll not found.", 0);
			return ;
	}

	hKH = SetWindowsHookEx(WH_KEYBOARD, &HookKeyProc, hMod, NULL);

	hWPH = SetWindowsHookEx(WH_CALLWNDPROC, &HookWndProc, hMod, NULL);

	hGM = SetWindowsHookEx(WH_GETMESSAGE, &HookGetMsgProc, hMod, NULL);

	HookInit(hwnd,hKH, hWPH, hGM, szCurDir);
}

VOID UnHook ()
{
	UnhookWindowsHookEx(hKH);
	UnhookWindowsHookEx(hWPH);
	UnhookWindowsHookEx(hGM);
}

BOOL WorkOnCommand(LPTSTR lpCmdLine){

	if (!lstrlen(lpCmdLine)){
		return false;
	}
	if (lpCmdLine[0] == '-'){
		switch (lpCmdLine[1]){
		case 'i':
			if (AddKeyBoard(&lpCmdLine[3]))
				MessageBox(GetDesktopWindow(), "The keyboard has been successfully added.", "KeyMagic", MB_ICONINFORMATION | MB_OK);
			return true;
		case 'u':
			HWND hPreHandle = FindWindow(NULL, szTitle);
			if (hPreHandle)
			{
				SendMessage(hPreHandle, WM_CLOSE, 0, 0);
				Sleep(1000);
			}
			UnHook();
			ScannerAndInject();
			return true;
		}
	}
	return false;
}

BOOL AddKeyBoard(char* lpKBPath){

	if (bAdmin)
		MessageBox (GetDesktopWindow(),
			"Sorry! The Keyboard cannot be added.\n"
			"Please turn off UAC or run Keymagic as an Administrator.",
			"Keymagic", MB_OK | MB_ICONEXCLAMATION);

	char lpPath[MAX_PATH];
	char lpName[MAX_PATH];
	char szKBPath[MAX_PATH];
	char szKBP[] = "KeyBoardPaths";
	char szMD[] = "MenuDisplays";
	char szSC[] = "ShortCuts";
	
	GetFileTitle(lpKBPath, lpName, MAX_PATH);

	if (lpName [lstrlen(lpName)-4] != '.')
		lstrcat(lpName, ".km2");

	wsprintf(lpPath, "KeyBoards\\%s", lpName);

	lpName [lstrlen(lpName)-4] = NULL;

	//if (SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, szKBPath))
	//	return false;

	lstrcpy(szKBPath, szCurDir);

	for (int i=lstrlen(szKBPath); szKBPath[i] != '\\'; i--){
		szKBPath[i] = NULL;
		if (szKBPath[i-1] == '\\'){
			szKBPath[i-1] = NULL;
			break;
		}
	}
	//PathAppend(szKBPath, "KeyMagic");
	PathAppend(szKBPath, lpPath);

	if (!CopyFile(lpKBPath, szKBPath, false)){
		MessageBox(GetDesktopWindow(), "File copying fail!", szKeymagic, MB_ICONERROR);
		return false;
	}

	if (!WritePrivateProfileString(szKBP, lpName, lpPath, szINIFile)){
		return false;
	}

	if (!WritePrivateProfileString(szMD, lpName, lpName, szINIFile)){
		return false;
	}
	if (!WritePrivateProfileString(szSC, lpName, "0", szINIFile)){
		return false;
	}

	return true;
}