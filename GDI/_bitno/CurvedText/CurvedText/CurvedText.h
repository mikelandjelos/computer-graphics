
// CurvedText.h : main header file for the CurvedText application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CCurvedTextApp:
// See CurvedText.cpp for the implementation of this class
//

class CCurvedTextApp : public CWinApp
{
public:
	CCurvedTextApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCurvedTextApp theApp;
