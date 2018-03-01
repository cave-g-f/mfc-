// purseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "purse.h"
#include "purseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPurseDlg dialog

CPurseDlg::CPurseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPurseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPurseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPurseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPurseDlg)
	DDX_Control(pDX, IDC_TAB1, m_MainFrame);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPurseDlg, CDialog)
	//{{AFX_MSG_MAP(CPurseDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPurseDlg message handlers

BOOL CPurseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//插入tab选项卡标签
	m_MainFrame.InsertItem(0,"调试助手");
	m_MainFrame.InsertItem(1,"应用开发");

	//关联对话框，将TAB控件设为选项卡对应对话框的父窗口
    m_Debugger.Create(IDD_DEBUGGER, GetDlgItem(IDC_TAB1));
	m_Appdev.Create(IDD_APPDEV, GetDlgItem(IDC_TAB1));

	//获取TAB控件客户区大小，用于调整选项卡对话框在父窗口中的位置
    CRect rect;
    m_MainFrame.GetClientRect(&rect);
    rect.top += 28;
    rect.right -= 5;
    rect.bottom -= 5;
    rect.left += 5;

	//设置子对话框尺寸并移动到指定位置
    m_Debugger.MoveWindow(&rect);
	m_Appdev.MoveWindow(&rect);
	//设置默认选项卡，对选项卡对话框进行隐藏和显示
	m_Debugger.ShowWindow(SW_SHOWNORMAL);
	m_Appdev.ShowWindow(SW_HIDE);
    m_MainFrame.SetCurSel(0);
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPurseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPurseDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPurseDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPurseDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	// 获取当前点击选项卡标签下标
    int cursel = m_MainFrame.GetCurSel();

	// 根据下标将相应的对话框显示，其余隐藏
    switch(cursel) {
        case 0:
            m_Debugger.ShowWindow(SW_SHOWNORMAL);
            m_Appdev.ShowWindow(SW_HIDE);
            break;
        case 1:
            m_Debugger.ShowWindow(SW_HIDE);
            m_Appdev.ShowWindow(SW_SHOWNORMAL);
            break;
        default:
            break;
	}
}
