#pragma once

#include <vector>
#include <codecvt>
#include <locale>
#include <algorithm>
#include <string>
#include<winsock.h>
#include <iostream>

// CCLientPlayGround dialog

class CCLientPlayGround : public CDialogEx
{
	DECLARE_DYNAMIC(CCLientPlayGround)

public:
	CCLientPlayGround(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCLientPlayGround();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENTPLAYGROUND };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSendanswer();
	CListBox listPlayGround;
	CEdit txtGuessWord;
	CEdit txtKeyWord;
	CEdit txtScore;

	using convert_t = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_t, wchar_t> strconverter;
	std::vector< std::vector<std::string> > records;
	std::vector<std::string> split(std::string s, std::string delimiter);
	//

	WSADATA w;
	int res = 0;
	int nSocket;
	sockaddr_in srv;
	int index;

	std::string disWord;
	CEdit txtClientName;
};
