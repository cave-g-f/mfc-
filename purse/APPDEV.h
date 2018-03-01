#if !defined(AFX_APPDEV_H__0559FD1E_4323_41E0_87A3_89FF97857423__INCLUDED_)
#define AFX_APPDEV_H__0559FD1E_4323_41E0_87A3_89FF97857423__INCLUDED_

#include "RecordHelper.h"	// Added by ClassView
#pragma comment(lib, "./libs/ZM124U.lib")
#include "./libs/ZM124U.h"
#include "SQLHelper.h"	// Added by ClassView

#define SCANTIMER 1
#define SCANTIMER_ID 1

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// APPDEV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAPPDEV dialog

class CAPPDEV : public CDialog
{
// Construction
public:
	bool canweb;
	bool canpurse;
	CTime time1;
	RecordHelper fileRecord;
	SQLHelper sqlhelper;
	CString GetCardUID();
	UINT_PTR m_ActiveTimer;
	bool writing;
	void HexStr2CharArray(const CString& hexStr, unsigned char* asc, int* asc_len);
	CAPPDEV(CWnd* pParent = NULL);   // standard constructor
	~CAPPDEV();

// Dialog Data
	//{{AFX_DATA(CAPPDEV)
	enum { IDD = IDD_APPDEV };
	CButton	m_B_load;
	CButton	m_B_clear;
	CButton	m_B_checktime;
	CButton	m_B_timedefault;
	CButton	m_B_exit;
	CButton	m_B_login;
	CButton	m_B_purchase;
	CButton	m_B_recharge;
	CButton	m_B_checkbalance;
	CButton	m_B_initpurse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAPPDEV)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAPPDEV)
	afx_msg void OnButton_initpurse();
	afx_msg void OnButton_checkbalance();
	afx_msg void OnButton_recharge();
	afx_msg void OnButton_purchase();
	afx_msg void OnButton_login();
	afx_msg void OnButton_exit();
	afx_msg void OnButton_timedefault();
	afx_msg void OnButton_checktime();
	afx_msg void OnButton_clear();
	afx_msg void OnButton_load();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPDEV_H__0559FD1E_4323_41E0_87A3_89FF97857423__INCLUDED_)
