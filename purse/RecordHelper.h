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

	// ����Ǯ������ ��ֵ ������¼����
	void SaveRecharges(CString uid, CString accounts, long remainings, CString result);
	void SaveConsumptions(CString uid, CString accounts, long remainings, CString result);
	// �ϻ������ϻ� �˳�������¼����
	void StartNets(CString uid, int remainTime, CString result);
	void ExitNets(CString uid, int remainTime, int overSeconds, CString result,CTime lastTime);
	// ���ؼ�¼
	CString LoadRecords();
	// ��ռ�¼
	BOOL EmptyRecords();
private:
	// ������ʷ��¼�ļ���
	CString mSaveFile;

};

#endif // !defined(AFX_RECORDHELPER_H__169FE363_23FD_442A_95AE_69835E690FDE__INCLUDED_)
