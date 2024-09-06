
// Bitmape.h : main header file for the Bitmape application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CBitmapeApp:
// See Bitmape.cpp for the implementation of this class
//

class CBitmapeApp : public CWinApp
{
public:
	CBitmapeApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBitmapeApp theApp;
