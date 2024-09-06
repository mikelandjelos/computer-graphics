
// MonaPuzzle.h : main header file for the MonaPuzzle application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMonaPuzzleApp:
// See MonaPuzzle.cpp for the implementation of this class
//

class CMonaPuzzleApp : public CWinApp
{
public:
	CMonaPuzzleApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMonaPuzzleApp theApp;
