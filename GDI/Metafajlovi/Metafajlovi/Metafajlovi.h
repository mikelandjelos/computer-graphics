
// Metafajlovi.h : main header file for the Metafajlovi application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMetafajloviApp:
// See Metafajlovi.cpp for the implementation of this class
//

class CMetafajloviApp : public CWinApp
{
public:
	CMetafajloviApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMetafajloviApp theApp;
