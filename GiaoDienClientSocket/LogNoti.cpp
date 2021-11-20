// LogNoti.cpp : implementation file
//

#include "pch.h"
#include "GiaoDienClientSocket.h"
#include "LogNoti.h"
#include "afxdialogex.h"


// LogNoti dialog

IMPLEMENT_DYNAMIC(LogNoti, CDialog)

LogNoti::LogNoti(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_NotiText, pParent)
{

}

LogNoti::~LogNoti()
{
}

void LogNoti::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LogNoti, CDialog)
END_MESSAGE_MAP()


// LogNoti message handlers
