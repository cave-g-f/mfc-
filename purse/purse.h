// purse.h : main header file for the PURSE application
//

#if !defined(AFX_PURSE_H__524B9F8D_4C3F_4F7F_8810_7D852B38D329__INCLUDED_)
#define AFX_PURSE_H__524B9F8D_4C3F_4F7F_8810_7D852B38D329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPurseApp:
// See purse.cpp for the implementation of this class
//

class CPurseApp : public CWinApp
{
public:
	CPurseApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPurseApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPurseApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PURSE_H__524B9F8D_4C3F_4F7F_8810_7D852B38D329__INCLUDED_)
