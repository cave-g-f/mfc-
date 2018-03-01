// Record.cpp: implementation of the Record class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "purse.h"
#include "Record.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Record::Record(CString _mSaveFile)
{
	this->mSaveFile = _mSaveFile;
}

Record::~Record()
{

}



void Record::SaveRecharges(CString uid, CString accounts, long remainings, CString result){
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

void Record::SaveConsumptions(CString uid, CString accounts, long remainings, CString result){
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

void Record::StartNets(CString uid, int remainTime, CString result){
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

void Record::ExitNets(CString uid, int remainTime, int overSeconds, CString result){
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
	CTime t1(0,0,0, overSeconds / 3600,(overSeconds % 3600) / 60, overSeconds % 60);
	CTimeSpan ct = curTime-t1;
	CString onlineTime;
	onlineTime.Format(_T("%dʱ%d��%d��"),ct.GetHours(), ct.GetMinutes(), ct.GetSeconds());
	// ��ʽ�����
	CString contents;
	contents.Format(_T("���ţ�%s\r\nʱ�䣺%s\r\n�����%s\r\n ���ݣ��û��˳�,�ϻ�ʱ�䣺%s\r\n��ʱ��%s\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, onlineTime,sRemainTime);

	// ָ���ļ�ĩβ��д��
	mFile.SeekToEnd();
	mFile.Write(contents, contents.GetLength());
	// �ر��ļ�
	mFile.Close();
}

CString Record::LoadRecords(){
	// ���ļ�
	CStdioFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead);
	// ָ��ͷ��ѭ������
	mFile.SeekToBegin();
	CString contents, line, multiLine;
	contents.Empty();
	multiLine.Empty();
	//  ����ֶζ�ȡ
	while (mFile.ReadString(line)) {
		line.TrimLeft();
		line.TrimRight();
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

BOOL Record::EmptyRecords(){
	// ����ļ�
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeReadWrite);
	mFile.Close();
	return true;
}
