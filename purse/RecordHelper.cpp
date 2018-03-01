// RecordHelperHelper.cpp: implementation of the RecordHelperHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RecordHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RecordHelper::RecordHelper(CString _mSaveFile)
{
	this->mSaveFile = _mSaveFile;
}

RecordHelper::~RecordHelper()
{

}



void RecordHelper::SaveRecharges(CString uid, CString accounts, long remainings, CString result){
	// ���ļ�����ȡʱ��
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	CTime curTime = CTime::GetCurrentTime();

	// �����¼
	CString contents;
	contents.Format(_T("���ţ�%s\r\nʱ�䣺%s\r\n�����%s\r\n���ݣ��û���ֵ\r\n��%s\r\n��%d\r\n\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, accounts, remainings);

	//ָ��ָ���ļ�ĩβ��д��
	mFile.SeekToEnd();
	mFile.Write(contents, contents.GetLength());
	//�ر��ļ�
	mFile.Close();
}

void RecordHelper::SaveConsumptions(CString uid, CString accounts, long remainings, CString result){
	// ���ļ�����ȡʱ��
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	CTime curTime = CTime::GetCurrentTime();

	// �����¼
	CString contents;
	contents.Format(_T("���ţ�%s\r\nʱ�䣺%s\r\n�����%s\r\n���ݣ��û�����\r\n��%s\r\n��%d\r\n\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, accounts, remainings);

	//ָ��ָ���ļ�ĩβ��д��
	mFile.SeekToEnd();
	mFile.Write(contents, contents.GetLength());
	// �ر��ļ�
	mFile.Close();
}

void RecordHelper::StartNets(CString uid, int remainTime, CString result){
	// ���ļ�����ȡʱ��
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	CTime curTime = CTime::GetCurrentTime();

	// ��ʽ����ʱ
	CString sRemainTime;
	sRemainTime.Format(_T("%dʱ%d��%d��"), remainTime / 3600, (remainTime % 3600) / 60, remainTime % 60);
	// ��ʽ�����
	CString contents;
	contents.Format(_T("���ţ�%s\r\nʱ�䣺%s\r\n�����%s\r\n���ݣ��û��ϻ�\r\n��ʱ��%s\r\n\r\n"), 
					uid, curTime.Format(TIMEFORMAT), result, sRemainTime);

	//ָ��ָ���ļ�ĩβ��д��
	mFile.SeekToEnd();
	mFile.Write(contents, contents.GetLength());
	// �ر��ļ�
	mFile.Close();
}

void RecordHelper::ExitNets(CString uid, int remainTime, int overSeconds, CString result,CTime lastTime){
	// ���ļ�����ȡʱ��
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	CTime curTime = CTime::GetCurrentTime();

	// ��ʽ���ϻ�ʱ��
	CString sOverTime;
	sOverTime.Format(_T("%dʱ%d��%d��"), overSeconds / 3600, (overSeconds % 3600) / 60, overSeconds % 60);
	// ��ʽ����ʱ
	CString sRemainTime;
	sRemainTime.Format(_T("%dʱ%d��%d��"), remainTime / 3600, (remainTime % 3600) / 60, remainTime % 60);
	//��ʽ������ʱ��
	CTimeSpan time3 = curTime - lastTime;
	CString online;
	online.Format(_T("%dʱ%d��%d��"), time3.GetHours(),time3.GetMinutes(), time3.GetSeconds());
	// ��ʽ�����
	CString contents;
	contents.Format(_T("���ţ�%s\r\nʱ�䣺%s\r\n�����%s\r\n���ݣ��û��˳�,�ϻ�ʱ�䣺%s\r\n��ʱ��%s\r\n\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, online,sRemainTime);

	// ָ���ļ�ĩβ��д��
	mFile.SeekToEnd();
	mFile.Write(contents, contents.GetLength());
	// �ر��ļ�
	mFile.Close();
}

CString RecordHelper::LoadRecords(){
	// ���ļ�
	CStdioFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead);
	// ָ��ͷ��ѭ������
	mFile.SeekToBegin();
	CString contents, line, multiLine;
	contents.Empty();
	multiLine.Empty();
	//  ����ֶζ�ȡ
	while (mFile.ReadString(line)) {
		line.TrimRight();
		line.TrimLeft();
		if (line.IsEmpty()) {
			contents = multiLine + _T("\r\n") + contents;
			multiLine.Empty();
		}
		else {
			multiLine += (line + _T("\r\n"));
		}
	}
	contents = multiLine + _T("\r\n") + contents;
	// �ر��ļ������ؽ��
	mFile.Close();
	return contents;
}

BOOL RecordHelper::EmptyRecords(){
	// ����ļ�
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeReadWrite);
	mFile.Close();
	return true;
}
