// ClientScreen.cpp : implementation file
//

#include "pch.h"
#include "GiaoDienClientSocket.h"
#include "ClientScreen.h"
#include "afxdialogex.h"


// ClientScreen dialog

IMPLEMENT_DYNAMIC(ClientScreen, CDialog)

ClientScreen::ClientScreen(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ClientScreen, pParent)
{

}

ClientScreen::~ClientScreen()
{
}

void ClientScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ClientScreen, CDialog)
END_MESSAGE_MAP()


// ClientScreen message handlers
