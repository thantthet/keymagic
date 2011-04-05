// HookInput.h : main header file for the HookInput application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CHookInputApp:
// See HookInput.cpp for the implementation of this class
//

class CHookInputApp : public CWinApp
{
public:
	CHookInputApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHookInputApp theApp;