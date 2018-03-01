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

// OnTable上机表记录结构
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
	// 连接数据库
	BOOL MySQL_Connect();
	// 关闭数据库
	void MySQL_Close();

	// 数据库增删改查操作函数
	// 对数据进行插入操作
	BOOL MySQL_Insert(RemainTime record);
	BOOL MySQL_Insert(OnRecord record);
	// 对数据进行删除操作
	BOOL MySQL_Delete(CString uid, CString table);
	// 对数据库进行更新操作
	BOOL MySQL_UpdateRemainTime(CString uid, int updateTime, CString table);
	// 对数据库执行sql语句
	BOOL MySQL_ExecuteSQL(CString sql);
	// 对数据库进行查询操作, 查询条件cond = " UID='xxxx' "
	void* MySQL_Query(CString cond, CString table);
	// 对数据库进行查询操作, 查询UID是否存在表中
	BOOL MySQL_QueryByUID(CString uid, CString table);
	// 定时扫描OnTable, 使用扫描周期Timer更新当前上机用户余时，捕捉超时用户
	void MySQL_ScanOnTable(int timer);

private:
	// 保存打开的数据库连接智能指针
	_ConnectionPtr m_pConnection;
};


#endif // !defined(AFX_SQLHELPER_H__84479537_ED59_4475_BE16_8C2F6244D35E__INCLUDED_)
