
// HelloWorld.h : main header file for the HelloWorld application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CHelloWorldApp:
// See HelloWorld.cpp for the implementation of this class
//

class CHelloWorldApp : public CWinApp
{
public:
	CHelloWorldApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHelloWorldApp theApp;
