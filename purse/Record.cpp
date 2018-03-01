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
	// 打开文件并获取时间
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	CTime curTime = CTime::GetCurrentTime();

	// 输出记录
	CString contents;
	contents.Format(_T("卡号：%s\r\n时间：%s\r\n结果：%s\r\n内容：用户充值\r\n金额：%s\r\n余额：%d\r\n\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, accounts, remainings);

	//指针指向文件末尾并写入
	mFile.SeekToEnd();
	mFile.Write(contents, contents.GetLength());
	//关闭文件
	mFile.Close();
}

void Record::SaveConsumptions(CString uid, CString accounts, long remainings, CString result){
	// 打开文件并获取时间
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	CTime curTime = CTime::GetCurrentTime();

	// 输出记录
	CString contents;
	contents.Format(_T("卡号：%s\r\n时间：%s\r\n结果：%s\r\n内容：用户消费\r\n金额：%s\r\n余额：%d\r\n\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, accounts, remainings);

	//指针指向文件末尾并写入
	mFile.SeekToEnd();
	mFile.Write(contents, contents.GetLength());
	// 关闭文件
	mFile.Close();
}

void Record::StartNets(CString uid, int remainTime, CString result){
	// 打开文件并获取时间
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	CTime curTime = CTime::GetCurrentTime();

	// 格式化余时
	CString sRemainTime;
	sRemainTime.Format(_T("%d时%d分%d秒"), remainTime / 3600, (remainTime % 3600) / 60, remainTime % 60);
	// 格式化输出
	CString contents;
	contents.Format(_T("卡号：%s\r\n时间：%s\r\n结果：%s\r\n内容：用户上机\r\n余时：%s\r\n\r\n"), 
					uid, curTime.Format(TIMEFORMAT), result, sRemainTime);

	//指针指向文件末尾并写入
	mFile.SeekToEnd();
	mFile.Write(contents, contents.GetLength());
	// 关闭文件
	mFile.Close();
}

void Record::ExitNets(CString uid, int remainTime, int overSeconds, CString result){
	// 打开文件并获取时间
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	CTime curTime = CTime::GetCurrentTime();

	// 格式化上机时间
	CString sOverTime;
	sOverTime.Format(_T("%d时%d分%d秒"), overSeconds / 3600, (overSeconds % 3600) / 60, overSeconds % 60);
	// 格式化余时
	CString sRemainTime;
	sRemainTime.Format(_T("%d时%d分%d秒"), remainTime / 3600, (remainTime % 3600) / 60, remainTime % 60);
	//格式化在线时间
	CTime t1(0,0,0, overSeconds / 3600,(overSeconds % 3600) / 60, overSeconds % 60);
	CTimeSpan ct = curTime-t1;
	CString onlineTime;
	onlineTime.Format(_T("%d时%d分%d秒"),ct.GetHours(), ct.GetMinutes(), ct.GetSeconds());
	// 格式化输出
	CString contents;
	contents.Format(_T("卡号：%s\r\n时间：%s\r\n结果：%s\r\n 内容：用户退出,上机时间：%s\r\n余时：%s\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, onlineTime,sRemainTime);

	// 指向文件末尾并写入
	mFile.SeekToEnd();
	mFile.Write(contents, contents.GetLength());
	// 关闭文件
	mFile.Close();
}

CString Record::LoadRecords(){
	// 打开文件
	CStdioFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead);
	// 指向开头并循环读入
	mFile.SeekToBegin();
	CString contents, line, multiLine;
	contents.Empty();
	multiLine.Empty();
	//  倒序分段读取
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
	// 关闭文件并返回结果
	mFile.Close();
	return contents;
}

BOOL Record::EmptyRecords(){
	// 清空文件
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeReadWrite);
	mFile.Close();
	return true;
}
