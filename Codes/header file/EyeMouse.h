// EyeMouse.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CEyeMouseApp:
// See EyeMouse.cpp for the implementation of this class
//

class CEyeMouseApp : public CWinApp
{
public:
	CEyeMouseApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CEyeMouseApp theApp;