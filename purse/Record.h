// Record.h: interface for the Record class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Record_H__A286A47A_9BCD_4C81_A27C_AE6DE07704FC__INCLUDED_)
#define AFX_Record_H__A286A47A_9BCD_4C81_A27C_AE6DE07704FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SAVEFILENAME _T("./log/records.txt")
#define TIMEFORMAT _T("%Y/%m/%d %H:%M:%S")

class Record
{
public:
	Record(CString _mSaveFile=SAVEFILENAME);
	virtual ~Record();

	// ����Ǯ������ ��ֵ ������¼����
	void SaveRecharges(CString uid, CString accounts, long remainings, CString result);
	void SaveConsumptions(CString uid, CString accounts, long remainings, CString result);
	// �ϻ������ϻ� �˳�������¼����
	void StartNets(CString uid, int remainTime, CString result);
	void ExitNets(CString uid, int remainTime, int overSeconds, CString result);
	// ���ؼ�¼
	CString LoadRecords();
	// ��ռ�¼
	BOOL EmptyRecords();
private:
	// ������ʷ��¼�ļ���
	CString mSaveFile;

};

#endif // !defined(AFX_RECORD_H__A286A47A_9BCD_4C81_A27C_AE6DE07704FC__INCLUDED_)
