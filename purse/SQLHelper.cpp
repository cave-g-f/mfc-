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
	// ��ʼ��OLE/COM�⻷��
	CoInitialize(NULL);
	try{
		// ͨ�����ִ���Connection����
		HRESULT hr = this->m_pConnection.CreateInstance("ADODB.Connection");
		if (FAILED(hr)){
			AfxMessageBox(_T("����_ConnectionPtr����ָ��ʧ��"));
			return false;
		}
		// �������ӳ�ʱʱ��
		this->m_pConnection->ConnectionTimeout = 600;
		// ����ִ�����ʱʱ��
		this->m_pConnection->CommandTimeout = 120;

		// �������ݿ�
		this->m_pConnection->Open("DSN=rfid;Server=localhost;Database=ZD124UE_DEMO",
			"root",
			"729369725",
			adModeUnknown);
	}
	catch (_com_error &e){
		// �����Ӵ򿪣���Ҫ���쳣�����йرպ��ͷ�����
		if ((NULL != this->m_pConnection) && this->m_pConnection->State){
			this->m_pConnection->Close();
			this->m_pConnection.Release();
			this->m_pConnection = NULL;
		}
		// ��CView��CDialog��Ҫʹ��ȫ�ֺ���AfxMessageBox
		AfxMessageBox(e.Description());
	}
	return true;
}

void SQLHelper::MySQL_Close(){
	if ((NULL != this->m_pConnection) && (this->m_pConnection->State)){
		this->m_pConnection->Close(); // �ر�����
		this->m_pConnection.Release();// �ͷ�����
		this->m_pConnection = NULL;
	}

	// ������COM���ж��COM��
	CoUninitialize();
}

BOOL SQLHelper::MySQL_Insert(RemainTime record){
	_CommandPtr m_pCommand;
	try{
		m_pCommand.CreateInstance("ADODB.Command");

		_variant_t vNULL;
		vNULL.vt = VT_ERROR;
		// ����Ϊ�޲���
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		// �����������Ӹ�ֵ����
		m_pCommand->ActiveConnection = this->m_pConnection;

		// ����ʱ�ַ�����
		CString temp;
		temp.Format(_T("%d"), record.RemainSeconds);
		// SQL���
		m_pCommand->CommandText = "insert into RemainTimeTable values(\'"
			+ (_bstr_t)record.UID + "\',"
			+ (_bstr_t)temp + ")";
		// ִ��SQL���
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
	}
	catch (_com_error &e){
		// ��Ҫ���쳣�������ͷ��������
		if ((NULL != m_pCommand) && (m_pCommand->State)){
			m_pCommand.Release();
			m_pCommand = NULL;
		}
		// ��CView��CDialog��Ҫʹ��ȫ�ֺ���AfxMessageBox
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
		// ����Ϊ�޲���
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		// �����������Ӹ�ֵ����
		m_pCommand->ActiveConnection = this->m_pConnection;

		// ����ʱ�ַ�����
		CString temp;
		temp.Format(_T("%d"), record.RemainSeconds);
		// ����ʱ�ַ�����
		CString isOverTime;
		isOverTime = (record.isOvertime) ? "TRUE" : "FALSE";
		// SQL���
		m_pCommand->CommandText = "insert into OnTable values(\'"
			+ (_bstr_t)record.UID + "\',"
			+ (_bstr_t)temp + ",\'"
			+ (_bstr_t)record.StartTime + "\',"
			+ (_bstr_t)isOverTime + ")";
		// ִ��SQL���
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
	}
	catch (_com_error &e){
		// ��Ҫ���쳣�������ͷ��������
		if ((NULL != m_pCommand) && (m_pCommand->State)){
			m_pCommand.Release();
			m_pCommand = NULL;
		}
		// ��CView��CDialog��Ҫʹ��ȫ�ֺ���AfxMessageBox
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
		// ����Ϊ�޲���
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		// �����������Ӹ�ֵ����
		m_pCommand->ActiveConnection = this->m_pConnection;

		// SQL���
		m_pCommand->CommandText = "SET FOREIGN_KEY_CHECKS = 0";
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
		m_pCommand->CommandText = "delete from " + (_bstr_t)table + " where UID=\'" + (_bstr_t)uid + "\'";
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
		m_pCommand->CommandText = "SET FOREIGN_KEY_CHECKS = 1";
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
	}
	catch (_com_error &e){
		// ��Ҫ���쳣�������ͷ��������
		if ((NULL != m_pCommand) && (m_pCommand->State)){
			m_pCommand.Release();
			m_pCommand = NULL;
		}
		// ��CView��CDialog��Ҫʹ��ȫ�ֺ���AfxMessageBox
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
		// ����Ϊ�޲���
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		// �����������Ӹ�ֵ����
		m_pCommand->ActiveConnection = this->m_pConnection;

		// ������ʱ���ַ�����
		CString str_updateTime;
		str_updateTime.Format(_T("%d"), updateTime);
		// SQL���
		m_pCommand->CommandText = "update " + (_bstr_t)table 
			+ " set RemainTime=" + (_bstr_t)str_updateTime 
			+ " where UID=\'" + (_bstr_t)uid + "\'";
		// ִ��SQL���
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
		// �����ʱΪ0��ͬʱ���ó�ʱλΪtrue
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
		// ��Ҫ���쳣�������ͷ��������
		if ((NULL != m_pCommand) && (m_pCommand->State)){
			m_pCommand.Release();
			m_pCommand = NULL;
		}
		// ��CView��CDialog��Ҫʹ��ȫ�ֺ���AfxMessageBox
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
		// ����Ϊ�޲���
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		// �����������Ӹ�ֵ����
		m_pCommand->ActiveConnection = this->m_pConnection;

		// SQL���
		m_pCommand->CommandText = (_bstr_t)sql;
		// ִ��SQL���
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
	}
	catch (_com_error &e){
		// ��Ҫ���쳣�������ͷ��������
		if ((NULL != m_pCommand) && (m_pCommand->State)){
			m_pCommand.Release();
			m_pCommand = NULL;
		}
		// ��CView��CDialog��Ҫʹ��ȫ�ֺ���AfxMessageBox
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

void* SQLHelper::MySQL_Query(CString cond, CString table){
	// �����ݼ�SQL���
	_variant_t sql = "SELECT * FROM " + (_bstr_t)table + " WHERE " + (_bstr_t)cond;
	OnRecord* pOnRecord = NULL;
	RemainTime* pRemainTime = NULL;
	try{
		// ����_RecordsetPtr����ָ��
		_RecordsetPtr m_pRecordset;
		HRESULT hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if (FAILED(hr)){
			AfxMessageBox(_T("����_RecordsetPtr����ָ��ʧ��"));
			return (void*)false;
		}
		//  �����ӣ���ȡ���ݼ�
		m_pRecordset->Open(sql,
			_variant_t((IDispatch*)(this->m_pConnection), true),
			adOpenForwardOnly,
			adLockReadOnly,
			adCmdText);
		// ȷ����Ϊ��
		if (!m_pRecordset->ADOEOF){
			// �ƶ��α굽��ǰ
			m_pRecordset->MoveFirst();
			// ѭ���������ݼ�
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
	// �����ݼ�SQL���
	_variant_t sql = "SELECT * FROM " + (_bstr_t)table
				+ " WHERE UID=\'" + (_bstr_t)uid + "\'";
	bool isExist = false;
	try{
		// ����_RecordsetPtr����ָ��
		_RecordsetPtr m_pRecordset;
		HRESULT hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if (FAILED(hr)){
			AfxMessageBox(_T("����_RecordsetPtr����ָ��ʧ��"));
			return false;
		}
		//  �����ӣ���ȡ���ݼ�
		m_pRecordset->Open(sql,
						   _variant_t((IDispatch*)(this->m_pConnection), true),
						   adOpenForwardOnly,
						   adLockReadOnly,
						   adCmdText);
		// ȷ����Ϊ��
		if (!m_pRecordset->ADOEOF){
			// �ƶ��α굽��ǰ
			m_pRecordset->MoveFirst();
			// ѭ���������ݼ�
			while (!m_pRecordset->ADOEOF) {
				/********* Get UID ********/
				_variant_t varUID = m_pRecordset->Fields->GetItem(_T("UID"))->GetValue();
				varUID.ChangeType(VT_BSTR);
				CString strUID = varUID.bstrVal;
				// ����ƥ��UID�Ƿ����
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
	// ����sql��������ʱ
	CString strTimer;
	strTimer.Format(_T("%d"), timer);
	CString table = REMAINTIMETABLE;
	CString sql = _T("update ") + table + _T(" set RemainTime=RemainTime-") + strTimer;
	// ������ʱ
	if (MySQL_ExecuteSQL(sql)) {
		// ���ò�ѯ������ʱС�ڵ���0
		CString cond = _T("RemainTime<=0");
		// ����������ѯ
		RemainTime* pRemainTime = (RemainTime*)MySQL_Query(cond, REMAINTIMETABLE);
		// ����ѯ���г�ʱ��
		if (pRemainTime != NULL) {
			// ����ʱ����ɾ��
			MySQL_Delete(pRemainTime->UID, REMAINTIMETABLE);
			// ����Ontable�еļ�¼��ʱΪ0��������Ϊ��ʱ�������ڲ�ʵ�֣�
			MySQL_UpdateRemainTime(pRemainTime->UID, 0, ONTABLE);
			// ��ʾ�û�
			CString message;
			message.Format(_T("���ѳ�ʱ���뼰ʱ��ֵ"), pRemainTime->UID);
			delete(pRemainTime); // Important!
			AfxMessageBox(message);
		}
	}
	else {
		AfxMessageBox(_T("��ʱ��ִ��sql��䲻�ɹ�"));
	}
}
