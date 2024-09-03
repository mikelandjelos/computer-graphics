
// CrtanjePrimitiva.h : main header file for the CrtanjePrimitiva application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CCrtanjePrimitivaApp:
// See CrtanjePrimitiva.cpp for the implementation of this class
//

class CCrtanjePrimitivaApp : public CWinApp
{
public:
	CCrtanjePrimitivaApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCrtanjePrimitivaApp theApp;
