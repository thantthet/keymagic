// HookInput.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HookInput.h"
#include "MainFrm.h"

#include "InputProcessor.h"

#ifndef DllName
#ifdef _M_IX86
#define DllName L"InputProcessor.x32.dll"
#else
#define DllName L"InputProcessor.x64.dll"
#endif
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CCustomCommandLineInfo : public CCommandLineInfo
{
public:
  CCustomCommandLineInfo()
  {
    m_hWnd = 0;
  }
private:
  //for convenience maintain 3 variables to indicate the param passed. 
  HWND m_hWnd;
 
  //public methods for checking these.
public:
  HWND GetHWnd() { return m_hWnd; };
   
  virtual void ParseParam(LPCTSTR pszParam, BOOL bFlag, BOOL bLast)
  {
	  swscanf(pszParam, TEXT("%x"), &m_hWnd);
  }
};

// CHookInputApp

BEGIN_MESSAGE_MAP(CHookInputApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CHookInputApp::OnAppAbout)
END_MESSAGE_MAP()


bool Hook(HWND hWnd)
{
	WCHAR dirName[MAX_PATH];

	SetMainWindowsHandle(hWnd);

	GetModuleFileNameW(NULL, dirName, MAX_PATH);
	PathRemoveFileSpecW(dirName);
	SetMainDir(dirName);

	HMODULE hModule = GetModuleHandle(DllName);
	SetWindowsHooks(hModule);

	if (GetKeyProcHook() == 0 || GetWndProcHook() == 0 || GetGetMsgProcHook() == 0) {
		printf("Hooked Unsccessful.");
		MessageBoxA(NULL, "Hooked Unsccessful.", "Error", MB_OK);
		return false;
	}

	printf("Hooked Successfully.");

	return true;
}

// CHookInputApp construction

CHookInputApp::CHookInputApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	CCustomCommandLineInfo ccInfo;
	ParseCommandLine(ccInfo);

	Hook(ccInfo.GetHWnd());
}


// The one and only CHookInputApp object

CHookInputApp theApp;


// CHookInputApp initialization

BOOL CHookInputApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("KeyMagic"));
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;

	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}


// CHookInputApp message handlers




// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CHookInputApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CHookInputApp message handlers

