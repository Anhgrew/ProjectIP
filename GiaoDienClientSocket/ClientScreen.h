#pragma once


// ClientScreen dialog

class ClientScreen : public CDialog
{
	DECLARE_DYNAMIC(ClientScreen)

public:
	ClientScreen(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ClientScreen();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ClientScreen };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
