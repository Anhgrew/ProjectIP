// CCLientPlayGround.cpp : implementation file
//

#include "pch.h"
#include "GiaoDienClientSocket.h"
#include "CCLientPlayGround.h"
#include "afxdialogex.h"

void HiddenRemain(std::string guessWord, std::string& keyWord, std::string& msg, std::string& disWord);
// CCLientPlayGround dialog

IMPLEMENT_DYNAMIC(CCLientPlayGround, CDialogEx)

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
}


BEGIN_MESSAGE_MAP(CCLientPlayGround, CDialogEx)
	ON_BN_CLICKED(IDC_SENDANSWER, &CCLientPlayGround::OnBnClickedSendanswer)
END_MESSAGE_MAP()

BOOL CCLientPlayGround::OnInitDialog() {
	// Init if required, can add later
	GetDlgItem(IDC_SENDANSWER)->EnableWindow(FALSE);

	char receive_buffer1[256] = { 0, };
	std::vector<std::string> res;

	if (recv(nSocket, receive_buffer1, 256, 0) == -1) {
		MessageBox(_T("Can not receive"));
	}
	else {
		res = split(receive_buffer1, ",");
		index = 0;

		if (res.size() > 0 && res[2].compare("Your turn") == 0) {
			GetDlgItem(IDC_SENDANSWER)->EnableWindow(TRUE);
		}
		else {

			MessageBox(_T("Please wait for your turn..."));
		}

	}

	//set readonly when turn < 2
	/*if (num_turn < 2) {
		txtKeyWord.SetReadOnly(TRUE);
	} else {
		txtKeyWord.SetReadOnly(FALSE);
	}*/
	return TRUE;
}

// CCLientPlayGround message handlers


void CCLientPlayGround::OnBnClickedSendanswer()
{

	char receive_buffer1[256] = { 0, };
	std::vector<std::string> res;





	if (recv(nSocket, receive_buffer1, 256, 0) == -1) {
		MessageBox(_T("Can not receive"));
	}
	else {
		res = split(receive_buffer1, ",");
		index = 0;
		if (res[0].compare("Let's start")) {
			/*		des = res[2];
					name = res[4];
					score = res[5];*/
			if (res.size() > 0 && res[6].compare("Your turn") == 0) {
				GetDlgItem(IDC_SENDANSWER)->EnableWindow(TRUE);
			}
			else {
				MessageBox(_T("Please wait for your turn..."));
			}
		}
		else {
			MessageBox(_T("Please wait ..."));
		}
	}


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
	CString responseMsg;
	CString guessW;
	CString keyW;

	txtGuessWord.GetWindowTextW(guessW);
	txtKeyWord, GetWindowTextW(keyW);

	//Send response to Server, with format: keyword,guessword
	responseMsg = keyW + "," + guessW;
	CT2A buff(responseMsg, CP_UTF8);
	//->send to server

	txtGuessWord.SetWindowTextW(_T(""));
	txtGuessWord.SetWindowTextW(_T(""));

	//Receive from Server

	std::string gW = CT2A(guessW);
	std::string kW = CT2A(keyW);
	std::string msg;
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
