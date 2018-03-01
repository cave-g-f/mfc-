// purseDlg.h : header file
//

#if !defined(AFX_PURSEDLG_H__1CEC4B5A_7F0F_434C_8B12_28DD9BFCECB3__INCLUDED_)
#define AFX_PURSEDLG_H__1CEC4B5A_7F0F_434C_8B12_28DD9BFCECB3__INCLUDED_

#pragma comment(lib, "./libs/ZM124U.lib")
#include "./libs/ZM124U.h"

#include "Debugger.h"	// Added by ClassView
#include "APPDEV.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPurseDlg dialog

class CPurseDlg : public CDialog
{
// Construction
public:
	CPurseDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPurseDlg)
	enum { IDD = IDD_PURSE_DIALOG };
	CTabCtrl	m_MainFrame;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPurseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPurseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CAPPDEV m_Appdev;
	CDebugger m_Debugger;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PURSEDLG_H__1CEC4B5A_7F0F_434C_8B12_28DD9BFCECB3__INCLUDED_)
