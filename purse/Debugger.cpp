// Debugger.cpp : implementation file
//

#include "stdafx.h"
#include "purse.h"
#include "Debugger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RED RGB(255, 0, 0)
#define BLUE RGB(0, 255, 0)
#define BLACK RGB(0, 0, 0)

/////////////////////////////////////////////////////////////////////////////
// CDebugger dialog


CDebugger::CDebugger(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugger::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDebugger)
	m_Static1 = _T("状态: ");
	m_Edit1 = _T("");
	m_RatioStatus = -1;
	//}}AFX_DATA_INIT
}


void CDebugger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebugger)
	DDX_Control(pDX, IDC_BUTTON9, m_B_writeblock);
	DDX_Control(pDX, IDC_BUTTON7, m_B_readsection);
	DDX_Control(pDX, IDC_BUTTON8, m_B_readblock);
	DDX_Control(pDX, IDC_BUTTON6, m_B_defaultkey);
	DDX_Control(pDX, IDC_BUTTON5, m_B_ledset);
	DDX_Control(pDX, IDC_BUTTON3, m_B_getuid);
	DDX_Control(pDX, IDC_BUTTON2, m_B_checkinfo);
	DDX_Control(pDX, IDC_BUTTON1, m_B_opendevice);
	DDX_Text(pDX, IDC_STATIC1, m_Static1);
	DDX_Text(pDX, IDC_EDIT1, m_Edit1);
	DDX_Radio(pDX, IDC_RADIO1, m_RatioStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDebugger, CDialog)
	//{{AFX_MSG_MAP(CDebugger)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton_opendevice)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2_checkinfo)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton_getuid)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton_ledset)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio_typeA)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio_typeB)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton_defaultkey)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton_reakblock)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton_readsection)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton_writeblock)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDebugger message handlers


void CDebugger::HexStr2CharArray(const CString& hexStr, unsigned char* asc, int* asc_len)
{
	*asc_len = 0;
	int len = hexStr.GetLength();

	char temp[200];
	char tmp[3] = { 0 };
	char* Hex;
	unsigned char* p;

	for(int i = 0; i < len; i++){
		temp[i] = hexStr[i];
	}
	temp[len] = '\0';
	Hex = temp;
	p = asc;

	while (*Hex != '\0') {
		tmp[0] = *Hex;
		Hex++;
		tmp[1] = *Hex;
		Hex++;
		tmp[2] = '\0';
		*p = (unsigned char)strtol(tmp, NULL, 16);
		p++;
		(*asc_len)++;
	}
	*p = '\0';
	return;
}

//开启设备
void CDebugger::OnButton_opendevice() 
{
	if(IDD_PowerOn() == IFD_OK) {
		device = true;
        //开启设备成功
        ((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(_T("开启设备成功"));
    }
    else {
		device = false;
        //开启设备失败
        ((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(_T("开启设备失败"));
    }	
}

//查看版本信息
void CDebugger::OnButton2_checkinfo() 
{
	unsigned char info[1024];
	CString temp,s;
	int len;

	if(Reader_Version(info, &len) == IFD_OK){
		//unsigned char转cstring
		s.Empty();
		for(int i = 0; i< len; i++){
			temp.Format(_T("%02x"), info[i]);
			s += temp;
		}
		//更新
		card = true;
		((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText(s);
		((CEdit*)GetDlgItem(IDC_EDIT4))->SetWindowText(_T("查看版本信息成功"));
	}
	else {
		card = false;
		((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText(_T(""));
		((CEdit*)GetDlgItem(IDC_EDIT4))->SetWindowText(_T("查看版本信息失败"));
	}
}

//获取卡号更新状态
void CDebugger::OnButton_getuid() 
{
	CString temp,s;
	unsigned char uid[2000];
	int len;

	if(find_14443(uid, &len) == IFD_OK) {
		//unsigned char转cstring
		s.Empty();
		for(int i = 0; i < len; i++){
			//将获得的UID数据转为16进制
			temp.Format(_T("%02x"), uid[i]);
			s += temp;
		}
		//更新
		card = true;
		((CEdit*)GetDlgItem(IDC_EDIT3))->SetWindowText(s);
		((CEdit*)GetDlgItem(IDC_EDIT4))->SetWindowText(_T("获取卡号成功"));
	}
	else {
		card = false;
		((CEdit*)GetDlgItem(IDC_EDIT3))->SetWindowText(_T(""));
		((CEdit*)GetDlgItem(IDC_EDIT4))->SetWindowText(_T("获取卡号失败"));
	}
}

//LED设置
void CDebugger::OnButton_ledset() 
{
	CString info,number;
	int info_len,num_len;
	unsigned char point;
	char cinfo[1024];
	//读取信息和小数位数
	((CEdit*)GetDlgItem(IDC_EDIT5))->GetWindowText(info);
	((CEdit*)GetDlgItem(IDC_EDIT6))->GetWindowText(number);
	
	info_len = info.GetLength();
	//CString转int
	num_len = _ttoi(number);
	//小数位数比信息位数大要补零
	if (num_len >= info_len) {
		for (int i = 0; i <= num_len - info_len; i++){
			info = _T("0") + info;
		}
		info_len = info.GetLength();
	}
	//确定小数点位置point
	point = (unsigned char)_ttoi(number) + 1;
	//没有小数时
	if (number == _T("0")){
		point = 0x00;
	}

	//CString转char*
	for (int i = 0; i < info_len; i++) {
        cinfo[i] = info[i];
    }
    cinfo[info_len] = '\0';

	//LED设置和更新
	
	if(LED(cinfo, info_len, point) == IFD_OK) {
		led = true;
		((CEdit*)GetDlgItem(IDC_EDIT7))->SetWindowText(_T("设置成功"));
	}
	else {
		led = false;
		((CEdit*)GetDlgItem(IDC_EDIT7))->SetWindowText(_T("设置失败"));
	}
}

//密钥设置
void CDebugger::OnRadio_typeA() 
{
	m_RatioStatus = 0x0A;
}
void CDebugger::OnRadio_typeB() 
{
	m_RatioStatus = 0x0B;
	
}
void CDebugger::OnButton_defaultkey() 
{
	((CEdit*)GetDlgItem(IDC_EDIT8))->SetWindowText(_T("FFFFFFFFFFFF"));	
}

//读取块数据
void CDebugger::OnButton_reakblock() 
{
	CString key;
	unsigned char ckey[8];
	int len = 0;

	//获取读块需要的密钥和密钥类型
	if(m_RatioStatus == -1) {
		MessageBox(_T("请选择密钥类型！"));
		return;
	}
	((CEdit*)GetDlgItem(IDC_EDIT8))->GetWindowText(key);
	if(key.IsEmpty()) {
		MessageBox(_T("请输入密钥！"));
		return;
	}
	//CString转unsigned char*
	key.MakeUpper();
	HexStr2CharArray(key, ckey, &len);

	//获取选择的扇区和块
	int section_num = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	if(section_num == CB_ERR) {
		MessageBox(_T("请选择扇区！"));
		return;
	}
	int block_num = ((CComboBox*)GetDlgItem(IDC_COMBO4))->GetCurSel();
	if(block_num == CB_ERR) {
		MessageBox(_T("请选择块区"));
		return;
	}

	//读取块
	unsigned char des_data[1024];
	int des_len;
	if(read_block(section_num, block_num, m_RatioStatus, ckey, des_data, &des_len) == IFD_OK) {
		CString block_data, temp;
		block_data.Empty();
		for(int i = 0; i < des_len; i++) {
			temp.Format(_T("%02x"), des_data[i]);
			block_data += temp;
		}
		// 根据块号设置不同的块
		switch(block_num) {
			case 0:
				((CEdit*)GetDlgItem(IDC_EDIT9))->SetWindowText(block_data);
				break;
			case 1:
				((CEdit*)GetDlgItem(IDC_EDIT10))->SetWindowText(block_data);
				break;
			case 2:
				((CEdit*)GetDlgItem(IDC_EDIT11))->SetWindowText(block_data);
				break;
			default:
				((CEdit*)GetDlgItem(IDC_EDIT12))->SetWindowText(block_data.Left(12));
				((CEdit*)GetDlgItem(IDC_EDIT13))->SetWindowText(block_data.Mid(12, 8));
				((CEdit*)GetDlgItem(IDC_EDIT14))->SetWindowText(block_data.Right(12));
				break;

		}
		// 更新状态栏，成功
		info = true;
		((CEdit*)GetDlgItem(IDC_EDIT15))->SetWindowText(_T("读取块成功"));
	}
	else {
		// 置空全部，更新状态栏，失败
		switch (block_num){
		case 0:
			((CEdit*)GetDlgItem(IDC_EDIT9))->SetWindowText(_T(""));
			break;
		case 1:
			((CEdit*)GetDlgItem(IDC_EDIT10))->SetWindowText(_T(""));
			break;
		case 2:
			((CEdit*)GetDlgItem(IDC_EDIT11))->SetWindowText(_T(""));
			break;
		default:
			((CEdit*)GetDlgItem(IDC_EDIT12))->SetWindowText(_T(""));
			((CEdit*)GetDlgItem(IDC_EDIT13))->SetWindowText(_T(""));
			((CEdit*)GetDlgItem(IDC_EDIT14))->SetWindowText(_T(""));
			break;
		}
		info = false;
		((CEdit*)GetDlgItem(IDC_EDIT15))->SetWindowText(_T("读取块异常"));
	}	
}

//读取扇区
void CDebugger::OnButton_readsection() 
{
	CString key;
	unsigned char ckey[8];
	int len = 0;

	//获取读块需要的密钥和密钥类型
	if(m_RatioStatus == -1) {
		MessageBox(_T("请选择密钥类型！"));
		return;
	}
	((CEdit*)GetDlgItem(IDC_EDIT8))->GetWindowText(key);
	if(key.IsEmpty()) {
		MessageBox(_T("请输入密钥！"));
		return;
	}
	//CString转unsigned char*
	key.MakeUpper();
	HexStr2CharArray(key, ckey, &len);

	//获取选择的扇区和块
	int section_num = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	if(section_num == CB_ERR) {
		MessageBox(_T("请选择扇区！"));
		return;
	}

	//读取块
	unsigned char des_data[1024];
	int des_len;
	//循环遍历4个块
	for(int block_num = 0; block_num < 4; block_num++) {
		if(read_block(section_num, block_num, m_RatioStatus, ckey, des_data, &des_len) == IFD_OK) {
			CString block_data, temp;
			block_data.Empty();
			for(int i = 0; i < des_len; i++) {
				temp.Format(_T("%02x"), des_data[i]);
				block_data += temp;
			}
			// 根据块号设置不同的块
			switch(block_num) {
				case 0:
					((CEdit*)GetDlgItem(IDC_EDIT9))->SetWindowText(block_data);
					break;
				case 1:
					((CEdit*)GetDlgItem(IDC_EDIT10))->SetWindowText(block_data);
					break;
				case 2:
					((CEdit*)GetDlgItem(IDC_EDIT11))->SetWindowText(block_data);
					break;
				default:
					((CEdit*)GetDlgItem(IDC_EDIT12))->SetWindowText(block_data.Left(12));
					((CEdit*)GetDlgItem(IDC_EDIT13))->SetWindowText(block_data.Mid(12, 8));
					((CEdit*)GetDlgItem(IDC_EDIT14))->SetWindowText(block_data.Right(12));
					break;
				}
			info = true;
			((CEdit*)GetDlgItem(IDC_EDIT15))->SetWindowText(_T("读取扇区成功"));
			}
			else {
				switch (block_num){
				case 0:
					((CEdit*)GetDlgItem(IDC_EDIT9))->SetWindowText(_T(""));
					break;
				case 1:
					((CEdit*)GetDlgItem(IDC_EDIT10))->SetWindowText(_T(""));
					break;
				case 2:
					((CEdit*)GetDlgItem(IDC_EDIT11))->SetWindowText(_T(""));
					break;
				default:
					((CEdit*)GetDlgItem(IDC_EDIT12))->SetWindowText(_T(""));
					((CEdit*)GetDlgItem(IDC_EDIT13))->SetWindowText(_T(""));
					((CEdit*)GetDlgItem(IDC_EDIT14))->SetWindowText(_T(""));
					break;
				}
				info = false;
				((CEdit*)GetDlgItem(IDC_EDIT15))->SetWindowText(_T("读取扇区异常"));
				return;
			}	
	}
}

//写块数据


void CDebugger::OnButton_writeblock() 
{
	CString key;
	unsigned char ckey[8];
	int len = 0;

	//获取读块需要的密钥和密钥类型
	if(m_RatioStatus == -1) {
		MessageBox(_T("请选择密钥类型！"));
		return;
	}
	((CEdit*)GetDlgItem(IDC_EDIT8))->GetWindowText(key);
	if(key.IsEmpty()) {
		MessageBox(_T("请输入密钥！"));
		return;
	}
	//CString转unsigned char*
	key.MakeUpper();
	HexStr2CharArray(key, ckey, &len);

	//获取选择的扇区和块
	int section_num = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	if(section_num == CB_ERR) {
		MessageBox(_T("请选择扇区！"));
		return;
	}
	int block_num = ((CComboBox*)GetDlgItem(IDC_COMBO4))->GetCurSel();
	if(block_num == CB_ERR) {
		MessageBox(_T("请选择块区"));
		return;
	}

	//获取编辑框中写入块的数据并进行处理
	CString block_data;
	if(block_num == 0) {
		((CEdit*)GetDlgItem(IDC_EDIT9))->GetWindowText(block_data);
		if (block_data.GetLength() % 2 == 1)	block_data += _T("0");
	}
	else if(block_num == 1) {
		((CEdit*)GetDlgItem(IDC_EDIT10))->GetWindowText(block_data);
		if (block_data.GetLength() % 2 == 1)	block_data += _T("0");
	}
	else if(block_num == 2) {
		((CEdit*)GetDlgItem(IDC_EDIT11))->GetWindowText(block_data);
		if (block_data.GetLength() % 2 == 1)	block_data += _T("0");
	}
	else if(block_num == 3) {
		((CEdit*)GetDlgItem(IDC_EDIT12))->GetWindowText(block_data);
		if(block_data.GetLength() < 12) {
			for(int i = block_data.GetLength(); i<12; i++) {
				block_data += _T("0");
			}
		}
		block_data = block_data + _T("FF078069") + _T("FFFFFFFFFFFF");
	}

	//写入数据
	unsigned char cdata[200];
	int block_len = 0;
	HexStr2CharArray(block_data, cdata, &block_len);
	// 写入块
	if(write_block(block_num, section_num, m_RatioStatus, ckey, cdata, block_len) == IFD_OK) {
		info = true;
		((CEdit*)GetDlgItem(IDC_EDIT15))->SetWindowText(_T("写入块成功"));
	}
	else {
		info = false;
		((CEdit*)GetDlgItem(IDC_EDIT15))->SetWindowText(_T("写入块失败"));
	}

}

HBRUSH CDebugger::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch(pWnd->GetDlgCtrlID()) {
		case IDC_EDIT4:
			if(card)
				pDC->SetTextColor(BLUE);
			else
				pDC->SetTextColor(RED);
			break;
		case IDC_EDIT7:
			if(led)
				pDC->SetTextColor(BLUE);
			else
				pDC->SetTextColor(RED);
			break;
		case IDC_EDIT15:
			if(info)
				pDC->SetTextColor(BLUE);
			else
				pDC->SetTextColor(RED);
			break;
			case IDC_EDIT1:
			if(device)
				pDC->SetTextColor(BLUE);
			else
				pDC->SetTextColor(RED);
			break;
		default:
			pDC->SetTextColor(BLACK);
			break;
	}	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
