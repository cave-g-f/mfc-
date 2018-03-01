// RecordHelper.h: interface for the RecordHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECORDHELPER_H__169FE363_23FD_442A_95AE_69835E690FDE__INCLUDED_)
#define AFX_RECORDHELPER_H__169FE363_23FD_442A_95AE_69835E690FDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SAVEFILENAME _T("records.txt")
#define TIMEFORMAT _T("%Y/%m/%d %H:%M:%S")

class RecordHelper  
{
public:
	RecordHelper(CString _mSaveFile=SAVEFILENAME);
	virtual ~RecordHelper();

	// 电子钱包消费 充值 操作记录保存
	void SaveRecharges(CString uid, CString accounts, long remainings, CString result);
	void SaveConsumptions(CString uid, CString accounts, long remainings, CString result);
	// 上机网管上机 退出操作记录保存
	void StartNets(CString uid, int remainTime, CString result);
	void ExitNets(CString uid, int remainTime, int overSeconds, CString result,CTime lastTime);
	// 加载记录
	CString LoadRecords();
	// 清空记录
	BOOL EmptyRecords();
private:
	// 保存历史记录文件名
	CString mSaveFile;

};

#endif // !defined(AFX_RECORDHELPER_H__169FE363_23FD_442A_95AE_69835E690FDE__INCLUDED_)
