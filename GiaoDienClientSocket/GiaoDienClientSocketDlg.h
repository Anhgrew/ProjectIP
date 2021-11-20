
// GiaoDienClientSocketDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "string.h"
#include<string>
#include<iostream>
#include "CConnectedSocket.h"
#include "IMySocketListener.h"
#include<vector>
#include <winsock.h>
#define PORT 9090
#pragma warning(disable:4996) 
using namespace std;



// CGiaoDienClientSocketDlg dialog
class CGiaoDienClientSocketDlg : public CDialogEx, public IMySocketListener
{
// Construction
public:
	CGiaoDienClientSocketDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GIAODIENCLIENTSOCKET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit txtIP;
	CEdit userName;
	CListBox listUserLog;
	CStatic txtTest;
	CStatic staticLoginResult;
	SOCKET server;

	char receive_buffer[256] = { 0 };

	///

	WSADATA w;
	int res = 0;
	int nSocket;
	sockaddr_in srv;
	int index;

public:
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedConnect();

	//implements
	vector<string> split(string s, string delimiter);
	virtual void OnAccepted(CString ipAddress, int port);
	virtual void OnRecept(CString msg);
	void CloseAllButtons();


private:
	CConnectedSocket mClientSocket;
	CString mIpAddress;
	CString mUserName;
public:
	afx_msg void OnBnClickedRegister();
};
