// CCLientPlayGround.cpp : implementation file
//

#include "pch.h"
#include "GiaoDienClientSocket.h"
#include "GiaoDienClientSocketDlg.h"
#include "CCLientPlayGround.h"
#include "afxdialogex.h"
#include "LogNoti.h"

void HiddenRemain(std::string guessWord, std::string& keyWord, std::string& msg, std::string& disWord);
// CCLientPlayGround dialog

IMPLEMENT_DYNAMIC(CCLientPlayGround, CDialogEx)
HANDLE refreshThread;

CCLientPlayGround::CCLientPlayGround(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENTPLAYGROUND, pParent)
{

}

CCLientPlayGround::~CCLientPlayGround()
{
}

void CCLientPlayGround::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTPG, listPlayGround);
	DDX_Control(pDX, IDC_GUESSWORD, txtGuessWord);
	DDX_Control(pDX, IDC_KEYWORD, txtKeyWord);
	DDX_Control(pDX, IDC_SCORE, txtScore);
	DDX_Control(pDX, IDC_CLIENTNAME, txtClientName);
}


BEGIN_MESSAGE_MAP(CCLientPlayGround, CDialogEx)
	ON_BN_CLICKED(IDC_SENDANSWER, &CCLientPlayGround::OnBnClickedSendanswer)
	ON_BN_CLICKED(BTN_REFRESH, &CCLientPlayGround::OnBnClickedRefresh)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CCLientPlayGround::OnInitDialog() {
	CDialogEx::OnInitDialog();
	// Init if required, can add later
	GetDlgItem(IDC_SENDANSWER)->EnableWindow(FALSE);
	char receive_buffer[256];
	memset(receive_buffer, 0, sizeof receive_buffer);
	std::vector<std::string> res;

	if (recv(nSocket, receive_buffer, 256, 0) == -1) {
		MessageBox(_T("Can not receive"));
	}
	else {
		res = split(receive_buffer, ",");

		//CString str;
		//str.Format(_T("Hi %d"), res.size());
		//MessageBoxW(str);
		if (res.size() > 6 && res[0].compare("Let 's start") == 0 && res[6].compare("Your turn") == 0) {
			GetDlgItem(IDC_SENDANSWER)->EnableWindow(TRUE);

		}
		else {	
			GetDlgItem(IDC_SENDANSWER)->EnableWindow(FALSE);
			MessageBox(_T("Please wait for your turn..."));
		}

	}

	//set readonly when turn < 2

	txtKeyWord.SetReadOnly(FALSE);

	CString name;
	name = res[4].c_str();
	txtClientName.SetWindowText(name);
	CString score;
	score = res[5].c_str();
	txtScore.SetWindowTextW(score);

	SetTimer(1, 2000, NULL);

	return TRUE;
}

// CCLientPlayGround message handlers


void CCLientPlayGround::OnBnClickedSendanswer()
{

	CString responseMsg;
	CString guessW;
	CString keyW;



	txtGuessWord.GetWindowTextW(guessW);
	txtKeyWord.GetWindowTextW(keyW);

	responseMsg = keyW + "," + guessW;

	char receive_buffer[256] = { 0, };
	std::vector<std::string> res;

	index = 0;



	GetDlgItem(IDC_SENDANSWER)->EnableWindow(TRUE);
		
	send(nSocket, CStringA(responseMsg), 256, 0);

	if (recv(nSocket, receive_buffer, 256, 0) == -1) {
		MessageBox(_T("Can not send answer"));
	}
	else {
		res = split(receive_buffer, ",");
		if (res.size() >= 8) {
			if (res[7].compare("") != 0 && res[7].find("Lost") != std::string::npos) {
				MessageBox(_T("You lost"));
				MessageBox(_T("1"));

			}
			else if (res[7].compare("") != 0 && res[7].find("Congratulations") != std::string::npos) {
				MessageBox(_T("Congratulations, you are the winner"));
				MessageBox(_T("2"));
			}
			else if (res[5].compare("") != 0 && res[5].compare("Correct guess") == 0) {
				MessageBox(_T("Correct guess"));
				MessageBox(_T("3"));
			}
			else if (res[5].compare("") != 0 && res[5].compare("Wrong guess") == 0) {
				MessageBox(_T("Wrong guess"));
				MessageBox(_T("4"));
			}
			CString score = _T("");;
			score = res[4].c_str();
			txtScore.SetWindowTextW(score);
			CString messTmp = _T("");
			messTmp = res[6].c_str();
			MessageBox(messTmp);
			if (res[6].compare("Your turn") == 0) {
				GetDlgItem(IDC_SENDANSWER)->EnableWindow(TRUE);
			}
			else {
				GetDlgItem(IDC_SENDANSWER)->EnableWindow(FALSE);
			}
			
		}
		if(res.size() < 8) {
			if (res[5].compare("") != 0 && res[5].compare("Correct guess") == 0) {
			MessageBox(_T("Correct guess"));
			MessageBox(_T("5"));
			}
			else if (res[5].compare("") != 0 && res[5].compare("Wrong guess") == 0) {
			MessageBox(_T("Wrong guess"));
			MessageBox(_T("6"));
			}
			CString score = _T("");;
			score = res[4].c_str();
			txtScore.SetWindowTextW(score);

			CString messTmp = _T("");
			messTmp = res[6].c_str();
			MessageBox(messTmp);
			MessageBox(_T("5"));
			if (res[6].compare("Your turn") == 0) {
				GetDlgItem(IDC_SENDANSWER)->EnableWindow(TRUE);
			}
			else {
				GetDlgItem(IDC_SENDANSWER)->EnableWindow(FALSE);
			}

		}
				
	}
	//else {
	//	GetDlgItem(IDC_SENDANSWER)->EnableWindow(FALSE);
	//	MessageBox(_T("Please wait for your turn..."));
	//}
	


	//if (turn.compare("Your turn") == 0) {
	//	std::cout << "Input:" << std::endl;
	//	/*getline(std::cin, send_buffer);
	//	send_buffer.append(",").append("1");
	//	send(nSocket, send_buffer.c_str(), 256, 0);
	//	cout << "Sended.." << endl;*/
	//}

	/*else {
		if (res.size() >= 8 && res[7] != "" && res[7].find("Lost") != std::string::npos) {
			cout << "Wait..." << endl;
			break;
		}
		if (res.size() >= 8 && res[7] != "" && res[7].find("Congratulations") != std::string::npos) {
			cout << "~~~ You win. End game ~~~" << endl;
			break;
		}
	}*/

	// TODO: Add your control notification handler code here
	

	//Send response to Server, with format: keyword,guessword
	
	//CT2A buff(responseMsg, CP_UTF8);
	//->send to server

	txtGuessWord.SetWindowTextW(_T(""));
	txtKeyWord.SetWindowTextW(_T(""));

	//Receive from Server

	std::string gW = CT2A(guessW);
	std::string kW = CT2A(keyW);
	//HiddenRemain(gW, keyword, msg, disWord);
	
	// Display on list chat result
	listPlayGround.AddString((LPCTSTR)strconverter.from_bytes(msg).c_str());
	listPlayGround.AddString(CString("The Remain words: ") + disWord.c_str());
	listPlayGround.SetCurSel(listPlayGround.GetCount() - 1);

	//// Show Score
	int score = 10;
	CString sc;
	sc.Format(_T("%d"), score);
	txtScore.SetWindowTextW(sc);

	//Handle when user make wrong guess word/wrong keyword

}

std::vector<std::string> CCLientPlayGround::split(std::string s, std::string delimiter)
{
	std::vector<std::string> res;
	int last = 0; int next = 0;
	while ((next = s.find(delimiter, last)) != std::string::npos) {
		res.push_back(s.substr(last, next - last));
		last = next + 1;
	}
	res.push_back(s.substr(last));
	return res;

}

void HiddenRemain(std::string guessWord, std::string& keyWord, std::string& msg, std::string& disWord) {
	std::vector<size_t> positions;
	size_t pos = keyWord.find(guessWord, 0);
	size_t spc = keyWord.find(" ", 0);

	while (pos != std::string::npos) {
		positions.push_back(pos);
		disWord.replace(pos, 1, guessWord);
		pos = keyWord.find(guessWord, pos + 1);
	}
	while (spc != std::string::npos) {
		disWord.replace(spc, 1, " ");
		spc = keyWord.find(" ", spc + 1);
	}

	msg = "Character '" + guessWord + "' has " + std::to_string(positions.size()) + " occurences.";
}

void CCLientPlayGround::OnBnClickedRefresh()
{
	// TODO: Add your control notification handler code here
	LogNoti noti;
	noti.DoModal();
}

void CCLientPlayGround::OnTimer(UINT_PTR nIDEvent)
{
	CCLientPlayGround::OnBnClickedRefresh();
	CDialogEx::OnTimer(nIDEvent);
}

