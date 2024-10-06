
// MetafileOps.h : main header file for the MetafileOps application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMetafileOpsApp:
// See MetafileOps.cpp for the implementation of this class
//

class CMetafileOpsApp : public CWinApp
{
public:
	CMetafileOpsApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMetafileOpsApp theApp;
