// APPDEV.cpp : implementation file
//

#include "stdafx.h"
#include "purse.h"
#include "APPDEV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RED RGB(255, 0, 0)
#define BLUE RGB(0, 255, 0)
#define BLACK RGB(0, 0, 0)

/////////////////////////////////////////////////////////////////////////////
// CAPPDEV dialog


CAPPDEV::CAPPDEV(CWnd* pParent /*=NULL*/)
	: CDialog(CAPPDEV::IDD, pParent)
{

	//{{AFX_DATA_INIT(CAPPDEV)
	//}}AFX_DATA_INIT
}

CAPPDEV::~CAPPDEV()
{
	KillTimer(SCANTIMER);
}



void CAPPDEV::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAPPDEV)
	DDX_Control(pDX, IDC_BUTTON18, m_B_load);
	DDX_Control(pDX, IDC_BUTTON17, m_B_clear);
	DDX_Control(pDX, IDC_BUTTON16, m_B_checktime);
	DDX_Control(pDX, IDC_BUTTON15, m_B_timedefault);
	DDX_Control(pDX, IDC_BUTTON14, m_B_exit);
	DDX_Control(pDX, IDC_BUTTON13, m_B_login);
	DDX_Control(pDX, IDC_BUTTON12, m_B_purchase);
	DDX_Control(pDX, IDC_BUTTON4, m_B_recharge);
	DDX_Control(pDX, IDC_BUTTON11, m_B_checkbalance);
	DDX_Control(pDX, IDC_BUTTON10, m_B_initpurse);
	//}}AFX_DATA_MAP
		SetTimer(SCANTIMER_ID, 1000, NULL);
}


BEGIN_MESSAGE_MAP(CAPPDEV, CDialog)
	//{{AFX_MSG_MAP(CAPPDEV)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton_initpurse)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton_checkbalance)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton_recharge)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton_purchase)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton_login)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton_exit)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton_timedefault)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton_checktime)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton_clear)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton_load)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAPPDEV message handlers
void CAPPDEV::HexStr2CharArray(const CString& hexStr, unsigned char* asc, int* asc_len)
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

//获取卡号uid
CString CAPPDEV::GetCardUID()
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
		return s;
	}
	else {
		return "NoCard";
	}
}

//钱包初始化
void CAPPDEV::OnButton_initpurse() 
{
	//初始化
	int section_num = 15;
	int block_num = 0;
	CString key;
	unsigned char ckey[8];
	unsigned char keytype = 0x0A;
	int len = 0;
	key = _T("FFFFFFFFFFFF");
	HexStr2CharArray(key, ckey, &len);

	//获得输入金额
	CString balance;
	long money;
	((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowText(balance);
	if(balance.IsEmpty()) {
		MessageBox(_T("请输入正确的初始化余额！"));
		return;
	}
	//CString转long
	money = _ttol(balance);
	if(money < 0) {
		MessageBox(_T("输入信息有误，请重新输入！"));
		return;
	}

	//写入金额并显示状态
	if(write_account(section_num, block_num, keytype, ckey, money) == IFD_OK) {
		canpurse = true;
		((CEdit*)GetDlgItem(IDC_EDIT16))->SetWindowText(_T("钱包初始化成功"));
	}
	else {
		canpurse = false;
		((CEdit*)GetDlgItem(IDC_EDIT16))->SetWindowText(_T("钱包初始化异常，请重新操作！"));
	}
}

//查询余额
void CAPPDEV::OnButton_checkbalance() 
{
	long money;
	CString balance;
	int section_num = 15;
	int block_num = 0;
	CString key;
	unsigned char ckey[8];
	unsigned char keytype = 0x0A;
	int len = 0;
	key = _T("FFFFFFFFFFFF");
	HexStr2CharArray(key, ckey, &len);

	if(read_account(section_num, block_num, keytype, ckey, &money) == IFD_OK) {
		balance.Format(_T("%d"), money);
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(balance);
		canpurse = true;
		((CEdit*)GetDlgItem(IDC_EDIT16))->SetWindowText(_T("查询余额成功"));
	}
	else {
		canpurse = false;
		((CEdit*)GetDlgItem(IDC_EDIT16))->SetWindowText(_T("查询余额失败"));
	}	
}

//充值
void CAPPDEV::OnButton_recharge() 
{
	//获取充值金额
	CString add;
	((CEdit*)GetDlgItem(IDC_EDIT5))->GetWindowText(add);
	if(add.IsEmpty()) {
		MessageBox(_T("请输入充值金额！"));
		return;
	}

	CString balance;

	//获取余额
	((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowText(balance);
	long money = _ttol(add);
	if(money < 0){
		MessageBox(_T("充值金额有误"));
		return;
	}
	int section_num = 15;
	int block_num = 0;
	CString key;
	unsigned char ckey[8];
	unsigned char keytype = 0x0A;
	int len = 0;
	key = _T("FFFFFFFFFFFF");
	HexStr2CharArray(key, ckey, &len);

	// 获取卡号
	CString uid = GetCardUID();
	// 账户充值
	if(add_account(section_num, block_num, keytype, ckey, money) == IFD_OK) {
		//充值后显示余额
		OnButton_checkbalance();
		canpurse = true;
		((CEdit*)GetDlgItem(IDC_EDIT16))->SetWindowText(_T("充值成功"));
		fileRecord.SaveRecharges(uid, add, money + _ttol(balance), _T("成功"));
	}
	else {
		canpurse = false;
		((CEdit*)GetDlgItem(IDC_EDIT16))->SetWindowText(_T("充值失败"));
		fileRecord.SaveRecharges(uid, add, _ttol(balance), _T("失败"));
	}
	// 更新历史记录显示
	OnButton_load();	
}

//消费
void CAPPDEV::OnButton_purchase() 
{
	CString sub;
	((CEdit*)GetDlgItem(IDC_EDIT6))->GetWindowText(sub);
	if(sub.IsEmpty()) {
		MessageBox(_T("消费金额为空！"));
		return;
	}
	
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowText(balance);
	long money = _ttol(sub);
	if(money < 0){
		MessageBox(_T("消费金额有误"));
		return;
	}
	if(money > _ttol(balance)) {
		MessageBox(_T("余额不足！"));
		return;
	}
	int section_num = 15;
	int block_num = 0;
	CString key;
	unsigned char ckey[8];
	unsigned char keytype = 0x0A;
	int len = 0;
	key = _T("FFFFFFFFFFFF");
	HexStr2CharArray(key, ckey, &len);
	// 获取卡号
	CString uid = GetCardUID();
	// 消费函数
	if(sub_account(section_num, block_num, keytype, ckey, money) == IFD_OK) {
		OnButton_checkbalance();
		canpurse = true;
		((CEdit*)GetDlgItem(IDC_EDIT16))->SetWindowText(_T("消费成功"));
		fileRecord.SaveConsumptions(uid, sub, _ttol(balance) - money, _T("成功"));
	}
	else {
		canpurse = false;
		((CEdit*)GetDlgItem(IDC_EDIT16))->SetWindowText(_T("消费失败"));
		fileRecord.SaveConsumptions(uid, sub, _ttol(balance), _T("失败"));
	}
	// 更新历史记录显示
	OnButton_load();
}


//开始上机
void CAPPDEV::OnButton_login() 
{
	
	//获取卡号uid
	CString uid = GetCardUID();

	if (uid != "NoCard") {
		//OnTable表中没有用户
		if (!sqlhelper.MySQL_QueryByUID(uid, ONTABLE)) {
			MessageBox(_T("请先初始化余时"));
		}
		//存在用户
		else {
			//用户存在RemainTimeTable
			if (sqlhelper.MySQL_QueryByUID(uid, REMAINTIMETABLE)){
				canweb = true;
				((CEdit*)GetDlgItem(IDC_EDIT20))->SetWindowText(_T("当前用户正在上机"));
			}
			// 当用户不存在RemainTimeTable
			else{
				CString cond = _T("UID=\'") + uid + _T("\'");
				OnRecord* pRecord = (OnRecord*)sqlhelper.MySQL_Query(cond, ONTABLE);

				// 当用户已经超时
				if (pRecord->isOvertime) {
					canweb = false;
					((CEdit*)GetDlgItem(IDC_EDIT20))->SetWindowText(_T("您已经超时，请及时充值"));
					fileRecord.StartNets(uid, pRecord->RemainSeconds, _T("失败"));
				}
				// 用户没有超时
				else {
					canweb = true;
					sqlhelper.MySQL_Insert(RemainTime(pRecord->UID, pRecord->RemainSeconds));
					((CEdit*)GetDlgItem(IDC_EDIT20))->SetWindowText(_T("上机成功"));
					time1 = CTime::GetCurrentTime();
					fileRecord.StartNets(uid, pRecord->RemainSeconds, _T("成功"));
				}

				// 更新历史记录显示
				OnButton_load();
				delete(pRecord);
				
			}
		}
	}
	else {
		canweb = false;
		((CEdit*)GetDlgItem(IDC_EDIT20))->SetWindowText(_T("获取卡号异常"));
	}	
}

//用户下机
void CAPPDEV::OnButton_exit() 
{
	CString uid = GetCardUID();

	if (uid != "NoCard") {
		// 用户没有在上机
		if (!sqlhelper.MySQL_QueryByUID(uid, REMAINTIMETABLE)) {
			canweb = false;
			((CEdit*)GetDlgItem(IDC_EDIT20))->SetWindowText(_T("用户当前不在上机"));
		}
		// 用户正在上机
		else {
			CString cond = _T("UID=\'") + uid + _T("\'");
			while (this->writing) { Sleep(100); }
			RemainTime* pRemainTime = (RemainTime*)sqlhelper.MySQL_Query(cond, REMAINTIMETABLE);
			sqlhelper.MySQL_Delete(uid, REMAINTIMETABLE);
			sqlhelper.MySQL_UpdateRemainTime(uid, pRemainTime->RemainSeconds, ONTABLE); // 更新OnTable
			canweb = true;
			((CEdit*)GetDlgItem(IDC_EDIT20))->SetWindowText(_T("用户已下机"));
			fileRecord.ExitNets(uid, pRemainTime->RemainSeconds, DEFAULTREMAINTIME, _T("成功"),time1); 
			// 更新历史记录显示
			OnButton_load();
			delete(pRemainTime); 
		}
	}
	else {
		canweb = false;
		((CEdit*)GetDlgItem(IDC_EDIT20))->SetWindowText(_T("获取卡号异常"));
	}
}

//余时默认初始化
void CAPPDEV::OnButton_timedefault() 
{
	CString uid = GetCardUID();

	CString hour,minute,second;
			((CEdit*)GetDlgItem(IDC_EDIT17))->GetWindowText(hour);
			int h = _ttoi(hour);
			((CEdit*)GetDlgItem(IDC_EDIT18))->GetWindowText(minute);
			int m = _ttoi(minute);
			((CEdit*)GetDlgItem(IDC_EDIT19))->GetWindowText(second);
			int s = _ttoi(minute);

	// 成功获取
	if (uid != "NoCard") {
		// 当用户不在OnTable中，插入新记录
		if (!sqlhelper.MySQL_QueryByUID(uid, ONTABLE)) {
			CTime curTime = CTime::GetCurrentTime();
			sqlhelper.MySQL_Insert(RemainTime(uid, h*3600+m*60+s));
			sqlhelper.MySQL_Insert(OnRecord(uid, h*3600+m*60+s, curTime.Format(TIMEFORMAT)));
			canweb = true;
			((CEdit*)GetDlgItem(IDC_EDIT20))->SetWindowText(_T("初始化余时成功"));
		}
		// 当用户存在OnTable中，更新OnTable
		else {
			sqlhelper.MySQL_UpdateRemainTime(uid, h*3600+m*60+s, ONTABLE);
			if (sqlhelper.MySQL_QueryByUID(uid, REMAINTIMETABLE)) {
				while (this->writing) { Sleep(100); }
				sqlhelper.MySQL_UpdateRemainTime(uid, h*3600+m*60+s, REMAINTIMETABLE);
			}
			canweb = true;
			((CEdit*)GetDlgItem(IDC_EDIT20))->SetWindowText(_T("重设余时成功"));
		}
	}
	else {
		canweb = false;
		((CEdit*)GetDlgItem(IDC_EDIT20))->SetWindowText(_T("获取卡号异常"));
	}	
}


//查询余时
void CAPPDEV::OnButton_checktime() 
{
	CString uid = GetCardUID();

	// 成功获取
	if (uid != "NoCard") {
		while (this->writing) { Sleep(100); }
		// 用户没有在上机
		if (!sqlhelper.MySQL_QueryByUID(uid, REMAINTIMETABLE)) {
			canweb = false;
			((CEdit*)GetDlgItem(IDC_EDIT20))->SetWindowText(_T("用户没有上机"));
		}
		// 用户正在上机
		else {
			CString cond = _T("UID=\'") + uid + _T("\'");
			RemainTime* pRemainTime = (RemainTime*)sqlhelper.MySQL_Query(cond, REMAINTIMETABLE);
			CString remainHours;
			remainHours.Format(_T("%d"), pRemainTime->RemainSeconds / 3600);
			CString remainMinutes;
			remainMinutes.Format(_T("%d"), (pRemainTime->RemainSeconds % 3600) / 60);
			CString remainSeconds;
			remainSeconds.Format(_T("%d"), pRemainTime->RemainSeconds  % 60);
			((CEdit*)GetDlgItem(IDC_EDIT17))->SetWindowText(remainHours);
			((CEdit*)GetDlgItem(IDC_EDIT18))->SetWindowText(remainMinutes);
			((CEdit*)GetDlgItem(IDC_EDIT19))->SetWindowText(remainSeconds);
			canweb = true;
			((CEdit*)GetDlgItem(IDC_EDIT20))->SetWindowText(_T("查询余时成功"));
			delete(pRemainTime); 
		}
	}
	else {
		canweb = false;
		((CEdit*)GetDlgItem(IDC_EDIT20))->SetWindowText(_T("获取卡号异常"));
	}
}


//清空记录
void CAPPDEV::OnButton_clear() 
{
	((CEdit*)GetDlgItem(IDC_EDIT21))->SetWindowText(_T(""));	
}

//加载记录
void CAPPDEV::OnButton_load() 
{
	((CEdit*)GetDlgItem(IDC_EDIT21))->SetWindowText(fileRecord.LoadRecords());	
}



void CAPPDEV::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent){
		case SCANTIMER_ID:
			//MessageBox(_T("1"));
			this->writing = true;
			sqlhelper.MySQL_ScanOnTable(SCANTIMER);
			this->writing = false;
			break;
		default:
			break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CAPPDEV::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch(pWnd->GetDlgCtrlID()) {
		case IDC_EDIT16:
			if(canpurse)
				pDC->SetTextColor(BLUE);
			else
				pDC->SetTextColor(RED);
			break;
		case IDC_EDIT20:
			if (canweb)
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
