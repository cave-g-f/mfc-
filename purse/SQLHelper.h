// SQLHelper.h: interface for the SQLHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQLHELPER_H__84479537_ED59_4475_BE16_8C2F6244D35E__INCLUDED_)
#define AFX_SQLHELPER_H__84479537_ED59_4475_BE16_8C2F6244D35E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#define DEFAULTREMAINTIME 20
#define ONTABLE _T("OnTable")
#define REMAINTIMETABLE _T("RemainTimeTable")
#define TIMEFORMAT _T("%Y/%m/%d %H:%M:%S")
#endif // _MSC_VER > 1000

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "ADOEOF")rename("BOF" , "ADOBOF")

// RemainTimeTable
struct RemainTime
{
	CString UID;
	int RemainSeconds;
	RemainTime(CString _UID, int _RemainSeconds){
		UID = _UID;
		RemainSeconds = _RemainSeconds;
	}
};

// OnTable�ϻ����¼�ṹ
struct OnRecord
{
	CString UID;
	int RemainSeconds;
	CString StartTime;
	BOOL isOvertime;
	OnRecord(CString _UID, int _RemainSeconds, CString _StartTime, BOOL _isOverTime = false){
		UID = _UID;
		RemainSeconds = _RemainSeconds;
		StartTime = _StartTime;
		isOvertime = _isOverTime;
	}
};

class SQLHelper  
{
public:
	SQLHelper();
	virtual ~SQLHelper();
	// �������ݿ�
	BOOL MySQL_Connect();
	// �ر����ݿ�
	void MySQL_Close();

	// ���ݿ���ɾ�Ĳ��������
	// �����ݽ��в������
	BOOL MySQL_Insert(RemainTime record);
	BOOL MySQL_Insert(OnRecord record);
	// �����ݽ���ɾ������
	BOOL MySQL_Delete(CString uid, CString table);
	// �����ݿ���и��²���
	BOOL MySQL_UpdateRemainTime(CString uid, int updateTime, CString table);
	// �����ݿ�ִ��sql���
	BOOL MySQL_ExecuteSQL(CString sql);
	// �����ݿ���в�ѯ����, ��ѯ����cond = " UID='xxxx' "
	void* MySQL_Query(CString cond, CString table);
	// �����ݿ���в�ѯ����, ��ѯUID�Ƿ���ڱ���
	BOOL MySQL_QueryByUID(CString uid, CString table);
	// ��ʱɨ��OnTable, ʹ��ɨ������Timer���µ�ǰ�ϻ��û���ʱ����׽��ʱ�û�
	void MySQL_ScanOnTable(int timer);

private:
	// ����򿪵����ݿ���������ָ��
	_ConnectionPtr m_pConnection;
};


#endif // !defined(AFX_SQLHELPER_H__84479537_ED59_4475_BE16_8C2F6244D35E__INCLUDED_)
