
// Tangram.h : main header file for the Tangram application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTangramApp:
// See Tangram.cpp for the implementation of this class
//

class CTangramApp : public CWinApp
{
public:
	CTangramApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTangramApp theApp;
