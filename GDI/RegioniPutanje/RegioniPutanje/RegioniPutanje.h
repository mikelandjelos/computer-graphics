
// RegioniPutanje.h : main header file for the RegioniPutanje application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRegioniPutanjeApp:
// See RegioniPutanje.cpp for the implementation of this class
//

class CRegioniPutanjeApp : public CWinApp
{
public:
	CRegioniPutanjeApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRegioniPutanjeApp theApp;
