
// AnisotropicMappingMode.h : main header file for the AnisotropicMappingMode application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CAnisotropicMappingModeApp:
// See AnisotropicMappingMode.cpp for the implementation of this class
//

class CAnisotropicMappingModeApp : public CWinApp
{
public:
	CAnisotropicMappingModeApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAnisotropicMappingModeApp theApp;
