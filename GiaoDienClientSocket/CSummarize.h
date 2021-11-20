#pragma once
#include <string>
#include <sstream>
#include<iterator>
#include <vector>
#include <codecvt>
#include <locale>

// CSummarize dialog

class CSummarize : public CDialogEx
{
	DECLARE_DYNAMIC(CSummarize)

public:
	CSummarize(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSummarize();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUMMARIZE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl mListSummarize;
	using convert_t = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_t, wchar_t> strconverter;
	std::vector< std::vector<std::string> > records;
};
