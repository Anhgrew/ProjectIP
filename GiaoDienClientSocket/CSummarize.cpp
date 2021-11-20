// CSummarize.cpp : implementation file
//

#include "pch.h"
#include "GiaoDienClientSocket.h"
#include "CSummarize.h"
#include "afxdialogex.h"


// CSummarize dialog

IMPLEMENT_DYNAMIC(CSummarize, CDialogEx)

CSummarize::CSummarize(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SUMMARIZE, pParent)
{

}

CSummarize::~CSummarize()
{
}

void CSummarize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUMMARIZE, mListSummarize);
}


BEGIN_MESSAGE_MAP(CSummarize, CDialogEx)
END_MESSAGE_MAP()


BOOL CSummarize::OnInitDialog()
{
	//CGiaoDienClientSocketDlg::OnInitDialog();
	CDialog::OnInitDialog();
	//Init header for table
	mListSummarize.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 90);
	mListSummarize.InsertColumn(1, _T("Total Score"), LVCFMT_LEFT, 120);
	mListSummarize.InsertColumn(2, _T("Rank"), LVCFMT_LEFT, 150);

	int nItem;
	//Receive Records
	records = { {"A", "20", "2"}, {"B", "10", "3"}, {"C", "5", "4"} };
	for (int i = 0; i < records.size(); ++i) {
		nItem = mListSummarize.InsertItem(i, (LPCTSTR)strconverter.from_bytes(i).c_str());
		mListSummarize.SetItemText(nItem, 0, (LPCTSTR)strconverter.from_bytes(records[i][0]).c_str());
		mListSummarize.SetItemText(nItem, 1, (LPCTSTR)strconverter.from_bytes(records[i][1]).c_str());
		mListSummarize.SetItemText(nItem, 2, (LPCTSTR)strconverter.from_bytes(records[i][2]).c_str());
	}
	return TRUE;
}

// CSummarize message handlers
