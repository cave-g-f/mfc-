// SQLHelper.cpp: implementation of the SQLHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "purse.h"
#include "SQLHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SQLHelper::SQLHelper(){
	this->MySQL_Connect();
}

SQLHelper::~SQLHelper(){
	this->MySQL_Close();
}

BOOL SQLHelper::MySQL_Connect(){
	// 初始化OLE/COM库环境
	CoInitialize(NULL);
	try{
		// 通过名字创建Connection对象
		HRESULT hr = this->m_pConnection.CreateInstance("ADODB.Connection");
		if (FAILED(hr)){
			AfxMessageBox(_T("创建_ConnectionPtr智能指针失败"));
			return false;
		}
		// 设置连接超时时间
		this->m_pConnection->ConnectionTimeout = 600;
		// 设置执行命令超时时间
		this->m_pConnection->CommandTimeout = 120;

		// 连接数据库
		this->m_pConnection->Open("DSN=rfid;Server=localhost;Database=ZD124UE_DEMO",
			"root",
			"729369725",
			adModeUnknown);
	}
	catch (_com_error &e){
		// 若连接打开，需要在异常处理中关闭和释放连接
		if ((NULL != this->m_pConnection) && this->m_pConnection->State){
			this->m_pConnection->Close();
			this->m_pConnection.Release();
			this->m_pConnection = NULL;
		}
		// 非CView和CDialog需要使用全局函数AfxMessageBox
		AfxMessageBox(e.Description());
	}
	return true;
}

void SQLHelper::MySQL_Close(){
	if ((NULL != this->m_pConnection) && (this->m_pConnection->State)){
		this->m_pConnection->Close(); // 关闭连接
		this->m_pConnection.Release();// 释放连接
		this->m_pConnection = NULL;
	}

	// 访问完COM库后，卸载COM库
	CoUninitialize();
}

BOOL SQLHelper::MySQL_Insert(RemainTime record){
	_CommandPtr m_pCommand;
	try{
		m_pCommand.CreateInstance("ADODB.Command");

		_variant_t vNULL;
		vNULL.vt = VT_ERROR;
		// 定义为无参数
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		// 将建立的连接赋值给它
		m_pCommand->ActiveConnection = this->m_pConnection;

		// 将余时字符串化
		CString temp;
		temp.Format(_T("%d"), record.RemainSeconds);
		// SQL语句
		m_pCommand->CommandText = "insert into RemainTimeTable values(\'"
			+ (_bstr_t)record.UID + "\',"
			+ (_bstr_t)temp + ")";
		// 执行SQL语句
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
	}
	catch (_com_error &e){
		// 需要在异常处理中释放命令对象
		if ((NULL != m_pCommand) && (m_pCommand->State)){
			m_pCommand.Release();
			m_pCommand = NULL;
		}
		// 非CView和CDialog需要使用全局函数AfxMessageBox
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

BOOL SQLHelper::MySQL_Insert(OnRecord record){
	_CommandPtr m_pCommand;
	try{
		m_pCommand.CreateInstance("ADODB.Command");

		_variant_t vNULL;
		vNULL.vt = VT_ERROR;
		// 定义为无参数
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		// 将建立的连接赋值给它
		m_pCommand->ActiveConnection = this->m_pConnection;

		// 将余时字符串化
		CString temp;
		temp.Format(_T("%d"), record.RemainSeconds);
		// 将超时字符串化
		CString isOverTime;
		isOverTime = (record.isOvertime) ? "TRUE" : "FALSE";
		// SQL语句
		m_pCommand->CommandText = "insert into OnTable values(\'"
			+ (_bstr_t)record.UID + "\',"
			+ (_bstr_t)temp + ",\'"
			+ (_bstr_t)record.StartTime + "\',"
			+ (_bstr_t)isOverTime + ")";
		// 执行SQL语句
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
	}
	catch (_com_error &e){
		// 需要在异常处理中释放命令对象
		if ((NULL != m_pCommand) && (m_pCommand->State)){
			m_pCommand.Release();
			m_pCommand = NULL;
		}
		// 非CView和CDialog需要使用全局函数AfxMessageBox
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

BOOL SQLHelper::MySQL_Delete(CString uid, CString table){
	_CommandPtr m_pCommand;
	try{
		m_pCommand.CreateInstance("ADODB.Command");

		_variant_t vNULL;
		vNULL.vt = VT_ERROR;
		// 定义为无参数
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		// 将建立的连接赋值给它
		m_pCommand->ActiveConnection = this->m_pConnection;

		// SQL语句
		m_pCommand->CommandText = "SET FOREIGN_KEY_CHECKS = 0";
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
		m_pCommand->CommandText = "delete from " + (_bstr_t)table + " where UID=\'" + (_bstr_t)uid + "\'";
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
		m_pCommand->CommandText = "SET FOREIGN_KEY_CHECKS = 1";
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
	}
	catch (_com_error &e){
		// 需要在异常处理中释放命令对象
		if ((NULL != m_pCommand) && (m_pCommand->State)){
			m_pCommand.Release();
			m_pCommand = NULL;
		}
		// 非CView和CDialog需要使用全局函数AfxMessageBox
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

BOOL SQLHelper::MySQL_UpdateRemainTime(CString uid, int updateTime, CString table){
	_CommandPtr m_pCommand;
	try{
		m_pCommand.CreateInstance("ADODB.Command");

		_variant_t vNULL;
		vNULL.vt = VT_ERROR;
		// 定义为无参数
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		// 将建立的连接赋值给它
		m_pCommand->ActiveConnection = this->m_pConnection;

		// 将更新时间字符串化
		CString str_updateTime;
		str_updateTime.Format(_T("%d"), updateTime);
		// SQL语句
		m_pCommand->CommandText = "update " + (_bstr_t)table 
			+ " set RemainTime=" + (_bstr_t)str_updateTime 
			+ " where UID=\'" + (_bstr_t)uid + "\'";
		// 执行SQL语句
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
		// 如果余时为0，同时设置超时位为true
		if (updateTime == 0) {
			m_pCommand->CommandText = "update " + (_bstr_t)table
				+ " set isOvertime=true"
				+ " where UID=\'" + (_bstr_t)uid + "\'";
			m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
		}
		else {
			m_pCommand->CommandText = "update " + (_bstr_t)table
				+ " set isOvertime=false"
				+ " where UID=\'" + (_bstr_t)uid + "\'";
			m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
		}
	}
	catch (_com_error &e){
		// 需要在异常处理中释放命令对象
		if ((NULL != m_pCommand) && (m_pCommand->State)){
			m_pCommand.Release();
			m_pCommand = NULL;
		}
		// 非CView和CDialog需要使用全局函数AfxMessageBox
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

BOOL SQLHelper::MySQL_ExecuteSQL(CString sql){
	_CommandPtr m_pCommand;
	try{
		m_pCommand.CreateInstance("ADODB.Command");

		_variant_t vNULL;
		vNULL.vt = VT_ERROR;
		// 定义为无参数
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		// 将建立的连接赋值给它
		m_pCommand->ActiveConnection = this->m_pConnection;

		// SQL语句
		m_pCommand->CommandText = (_bstr_t)sql;
		// 执行SQL语句
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
	}
	catch (_com_error &e){
		// 需要在异常处理中释放命令对象
		if ((NULL != m_pCommand) && (m_pCommand->State)){
			m_pCommand.Release();
			m_pCommand = NULL;
		}
		// 非CView和CDialog需要使用全局函数AfxMessageBox
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

void* SQLHelper::MySQL_Query(CString cond, CString table){
	// 打开数据集SQL语句
	_variant_t sql = "SELECT * FROM " + (_bstr_t)table + " WHERE " + (_bstr_t)cond;
	OnRecord* pOnRecord = NULL;
	RemainTime* pRemainTime = NULL;
	try{
		// 定义_RecordsetPtr智能指针
		_RecordsetPtr m_pRecordset;
		HRESULT hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if (FAILED(hr)){
			AfxMessageBox(_T("创建_RecordsetPtr智能指针失败"));
			return (void*)false;
		}
		//  打开连接，获取数据集
		m_pRecordset->Open(sql,
			_variant_t((IDispatch*)(this->m_pConnection), true),
			adOpenForwardOnly,
			adLockReadOnly,
			adCmdText);
		// 确定表不为空
		if (!m_pRecordset->ADOEOF){
			// 移动游标到最前
			m_pRecordset->MoveFirst();
			// 循环遍历数据集
			while (!m_pRecordset->ADOEOF){
				if (table == ONTABLE) {
					/********* Get UID ********/
					_variant_t varUID = m_pRecordset->Fields->GetItem(_T("UID"))->GetValue();
					varUID.ChangeType(VT_BSTR);
					CString strUID = varUID.bstrVal;
					/********* Get RemainSeconds ********/
					_variant_t varRemainTime = m_pRecordset->Fields->GetItem(_T("RemainTime"))->GetValue();
					varRemainTime.ChangeType(VT_INT);
					int intRemainTime = varRemainTime.intVal;
					/********** Get StartTime ******************/
					//AfxMessageBox("1");
					_variant_t varStartTime = m_pRecordset->GetCollect(_T("StartTime"));
					//AfxMessageBox("2");
					COleDateTime varDateTime = (COleDateTime)varStartTime;
					//AfxMessageBox("3");
					CString strStartTime = varDateTime;
					//AfxMessageBox("4");
					/*********** Get isOverTime ***********/
					_variant_t varIsOverTime = m_pRecordset->Fields->GetItem(_T("isOverTime"))->GetValue();
					varIsOverTime.ChangeType(VT_BOOL);
					bool boolIsOverTime = varIsOverTime.boolVal;
					/************ Generate OnRecord ****************/
					pOnRecord = new OnRecord(strUID, intRemainTime, strStartTime, boolIsOverTime);
				}
				else if(table == REMAINTIMETABLE) {
					/********* Get UID ********/
					_variant_t varUID = m_pRecordset->Fields->GetItem(_T("UID"))->GetValue();
					varUID.ChangeType(VT_BSTR);
					CString strUID = varUID.bstrVal;
					/********* Get RemainSeconds ********/
					_variant_t varRemainTime = m_pRecordset->Fields->GetItem(_T("RemainTime"))->GetValue();
					varRemainTime.ChangeType(VT_INT);
					int intRemainTime = varRemainTime.intVal;
					/************ Generate RemainTime ****************/
					pRemainTime = new RemainTime(strUID, intRemainTime);
				}
				
				break; // Only Return one struct
				// BUG: Let another get more seconds to live
				m_pRecordset->MoveNext();
			}
		}
	}
	catch (_com_error &e){
		AfxMessageBox(e.Description());
	}
	if (table == ONTABLE)
	{
		return (void*)pOnRecord;
	}
	else
		return (void*)pRemainTime;
}

BOOL SQLHelper::MySQL_QueryByUID(CString uid, CString table){
	// 打开数据集SQL语句
	_variant_t sql = "SELECT * FROM " + (_bstr_t)table
				+ " WHERE UID=\'" + (_bstr_t)uid + "\'";
	bool isExist = false;
	try{
		// 定义_RecordsetPtr智能指针
		_RecordsetPtr m_pRecordset;
		HRESULT hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if (FAILED(hr)){
			AfxMessageBox(_T("创建_RecordsetPtr智能指针失败"));
			return false;
		}
		//  打开连接，获取数据集
		m_pRecordset->Open(sql,
						   _variant_t((IDispatch*)(this->m_pConnection), true),
						   adOpenForwardOnly,
						   adLockReadOnly,
						   adCmdText);
		// 确定表不为空
		if (!m_pRecordset->ADOEOF){
			// 移动游标到最前
			m_pRecordset->MoveFirst();
			// 循环遍历数据集
			while (!m_pRecordset->ADOEOF) {
				/********* Get UID ********/
				_variant_t varUID = m_pRecordset->Fields->GetItem(_T("UID"))->GetValue();
				varUID.ChangeType(VT_BSTR);
				CString strUID = varUID.bstrVal;
				// 查找匹配UID是否存在
				if (strUID == uid) {
					isExist = true;
					break;
				}
				m_pRecordset->MoveNext();
			}
		}
	}
	catch (_com_error &e){
		AfxMessageBox(e.Description());
	}
	return isExist;
}

void SQLHelper::MySQL_ScanOnTable(int timer) {
	// 设置sql语句更新余时
	CString strTimer;
	strTimer.Format(_T("%d"), timer);
	CString table = REMAINTIMETABLE;
	CString sql = _T("update ") + table + _T(" set RemainTime=RemainTime-") + strTimer;
	// 更新余时
	if (MySQL_ExecuteSQL(sql)) {
		// 设置查询条件余时小于等于0
		CString cond = _T("RemainTime<=0");
		// 根据条件查询
		RemainTime* pRemainTime = (RemainTime*)MySQL_Query(cond, REMAINTIMETABLE);
		// 当查询到有超时的
		if (pRemainTime != NULL) {
			// 从余时表中删除
			MySQL_Delete(pRemainTime->UID, REMAINTIMETABLE);
			// 更新Ontable中的记录余时为0，并设置为超时（函数内部实现）
			MySQL_UpdateRemainTime(pRemainTime->UID, 0, ONTABLE);
			// 提示用户
			CString message;
			message.Format(_T("您已超时，请及时充值"), pRemainTime->UID);
			delete(pRemainTime); // Important!
			AfxMessageBox(message);
		}
	}
	else {
		AfxMessageBox(_T("定时器执行sql语句不成功"));
	}
}
