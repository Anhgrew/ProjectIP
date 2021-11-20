#pragma once


// LogNoti dialog

class LogNoti : public CDialog
{
	DECLARE_DYNAMIC(LogNoti)

public:
	LogNoti(CWnd* pParent = nullptr);   // standard constructor
	virtual ~LogNoti();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NotiText };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	//CStatic notiText;
	//string text;
};
