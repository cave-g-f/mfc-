#if !defined(AFX_DEBUGGER_H__AB53ED91_6533_4E6E_AC8D_3BC4747AB8C1__INCLUDED_)
#define AFX_DEBUGGER_H__AB53ED91_6533_4E6E_AC8D_3BC4747AB8C1__INCLUDED_

#pragma comment(lib, "./libs/ZM124U.lib")
#include "./libs/ZM124U.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Debugger.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDebugger dialog

class CDebugger : public CDialog
{
// Construction
public:
	bool led;
	bool info;
	bool device;
	bool card;
	void HexStr2CharArray(const CString& hexStr, unsigned char* asc, int* asc_len);
	CDebugger(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDebugger)
	enum { IDD = IDD_DEBUGGER };
	CButton	m_B_writeblock;
	CButton	m_B_readsection;
	CButton	m_B_readblock;
	CButton	m_B_defaultkey;
	CButton	m_B_ledset;
	CButton	m_B_getuid;
	CButton	m_B_checkinfo;
	CButton	m_B_opendevice;
	CString	m_Static1;
	CString	m_Edit1;
	int		m_RatioStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDebugger)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDebugger)
	afx_msg void OnButton_opendevice();
	afx_msg void OnButton2_checkinfo();
	afx_msg void OnButton_getuid();
	afx_msg void OnButton_ledset();
	afx_msg void OnRadio_typeA();
	afx_msg void OnRadio_typeB();
	afx_msg void OnButton_defaultkey();
	afx_msg void OnButton_reakblock();
	afx_msg void OnButton_readsection();
	afx_msg void OnButton_writeblock();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEBUGGER_H__AB53ED91_6533_4E6E_AC8D_3BC4747AB8C1__INCLUDED_)
