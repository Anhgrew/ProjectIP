// CCLientPlayGround.cpp : implementation file
//

#include "pch.h"
#include "GiaoDienClientSocket.h"
#include "GiaoDienClientSocketDlg.h"
#include "CCLientPlayGround.h"
#include <windows.h>
#include "LogNoti.h"
#include <thread>

void HiddenRemain(std::string guessWord, std::string keyWord, std::string& msg, std::string& disWord);
// CCLientPlayGround dialog

IMPLEMENT_DYNAMIC(CCLientPlayGround, CDialogEx)
HANDLE refreshThread;

//BOOL stopNow = FALSE;
//Client* play_client2;
//std::string output;
//int num = 0;
//std::vector<std::string> res;
//
//char receive_buffer[256];
//bool lost = false;
//
//string tmp = "";
//
//void processRev(string tmp) {
//	char receive_buffer[256];
//
//	while (1)
//	{
//		/*if (tmp.compare("") == 0) {
//			Sleep(1000);
//		}
//		memset(receive_buffer, 0, sizeof receive_buffer);
//		recv(play_client2->nSocket, receive_buffer, 256, 0);
//		tmp = string(receive_buffer);*/
//		tmp = "AAA";
//		Sleep(1000);
//		
//	}
//}
//
//
//std::vector<std::string> splitString(std::string s, std::string delimiter)
//{
//	std::vector<std::string> res;
//	int last = 0; int next = 0;
//	while ((next = s.find(delimiter, last)) != std::string::npos) {
//		res.push_back(s.substr(last, next - last));
//		last = next + 1;
//	}
//	res.push_back(s.substr(last));
//	return res;
//
//}

//UINT reciveMessThread(LPVOID lParam)
//{
//	Client* k = (Client *)lParam;
//	
//	//while (!stopNow && (num < 10000000000000000))
//	//{
//	//	Sleep(500);
//	//	num++;
//	//	Sleep(50);
//
//	//} 127.0.0.1
//	int i = 0;
//	while (i < 100)
//	{
//
//		i++;
//		/*memset(receive_buffer, 0, sizeof receive_buffer);
//		if (recv(play_client2->nSocket, receive_buffer, 256, 0) < 0) {
//			Sleep(500);
//		}
//		else {
//			output = std::string(receive_buffer);
//			res = splitString(receive_buffer, ",");
//			return 0;
//		}*/
//	}
//	k->res = i;
//	return 0;
//}


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
	UpdateData(TRUE);

	SetTimer(1234, 1000, 0);

	// play_client2 = play_client;

	/*CWinThread* pThread;

	pThread = AfxBeginThread(reciveMessThread, k);*/
	
	// std::thread task(processRev, tmp);
	// task.join();	
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
		if (res.size() > 8 && res[0].compare("Let 's start") == 0 && res[6].compare("Your turn") == 0) {
			GetDlgItem(IDC_SENDANSWER)->EnableWindow(TRUE);
			listPlayGround.AddString(CString("HINT: ") + (LPCTSTR)strconverter.from_bytes(res[2]).c_str());
			disWord = res[8];
			HiddenRemain("", res[7], msg, disWord);
			listPlayGround.AddString(CString("KEYWORD: ") + disWord.c_str());
			listPlayGround.SetCurSel(listPlayGround.GetCount() - 1);
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
	
	CString kk = _T("");
	
	/*SetTimer(1, 2000000, NULL);*/
	//string m = play_client->receive_message;
	//MessageBox(kk);
	UpdateData(FALSE);

	return TRUE;
}

// CCLientPlayGround message handlers

void CCLientPlayGround::OnBnClickedSendanswer()
{
	UpdateData(TRUE);
	CString responseMsg;
	CString guessW;
	CString keyW;

	txtGuessWord.GetWindowTextW(guessW);
	txtKeyWord.GetWindowTextW(keyW);
	if (keyW == _T("")) {
		keyW = string("#").c_str();
	}
	if (guessW == _T("")) {
		guessW = string("#").c_str();
	}
	

	responseMsg = keyW + "," + guessW;

	char receive_buffer[256];
	memset(receive_buffer, 0, sizeof receive_buffer);

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

			}
			else if (res[7].compare("") != 0 && res[7].find("Congratulations") != std::string::npos) {
				MessageBox(_T("Congratulations, you are the winner"));
			}
			else if (res[5].compare("") != 0 && res[5].compare("Correct guess") == 0) {
				MessageBox(_T("Correct guess"));
<<<<<<< HEAD
=======
				MessageBox(_T("3"));
				std::string gW = CT2A(guessW);
				std::string kW = CT2A(keyW);
				disWord = res[8];
				HiddenRemain("", res[7], msg, disWord);

				// Display on list chat result
				listPlayGround.AddString((LPCTSTR)strconverter.from_bytes(msg).c_str());
				listPlayGround.AddString(CString("KEYWORD: ") + disWord.c_str());
				listPlayGround.SetCurSel(listPlayGround.GetCount() - 1);
>>>>>>> f0034d6... update display hint
			}
			else if (res[5].compare("") != 0 && res[5].compare("Wrong guess") == 0) {
				MessageBox(_T("Wrong guess"));
			}


			CString name;
			name = res[3].c_str();
			txtClientName.SetWindowText(name);

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
		if(res.size() < 8 && res.size() >= 6) {
			if (res[5].compare("") != 0 && res[5].compare("Correct guess") == 0) {
			MessageBox(_T("Correct guess"));
			}
			else if (res[5].compare("") != 0 && res[5].compare("Wrong guess") == 0) {
			MessageBox(_T("Wrong guess"));
			}
			else if (res[5].compare("") != 0 && res[5].compare("Not start") == 0) {
				MessageBox(_T("Please wait for game start"));
			}

			// hinh nhu correct thi moi can chay khuc duoi dien vo list
			CString name;
			name = res[3].c_str();
			txtClientName.SetWindowText(name);

			CString score = _T("");;
			score = res[4].c_str();
			txtScore.SetWindowTextW(score);

			CString messTmp = _T("");
			messTmp = res[6].c_str();
			if (res[6].compare("Your turn") == 0) {
				GetDlgItem(IDC_SENDANSWER)->EnableWindow(TRUE);
			}
			else {
				GetDlgItem(IDC_SENDANSWER)->EnableWindow(FALSE);
			}

		}
		else {
			MessageBox(_T("Please wait for game start"));
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

	//// Show Score
<<<<<<< HEAD
	//int score = 10;
	//CString sc;
	//sc.Format(_T("%d"), score);
	//txtScore.SetWindowTextW(sc);
=======
	/*int score = 10;
	CString sc;
	sc.Format(_T("%d"), score);
	txtScore.SetWindowTextW(sc);*/
>>>>>>> f0034d6... update display hint

	//Handle when user make wrong guess word/wrong keyword
	UpdateData(TRUE);
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

void HiddenRemain(std::string guessWord, std::string keyWord, std::string& msg, std::string& disWord) {
	std::vector<size_t> positions;
	size_t pos = keyWord.find(guessWord, 0);
	size_t spc = keyWord.find(" ", 0);

	if (guessWord != "") {
		while (pos != std::string::npos) {
			positions.push_back(pos);
			disWord.replace(pos, 1, guessWord);
			pos = keyWord.find(guessWord, pos + 1);
		}
	}
	while (spc != std::string::npos) {
		disWord.replace(spc, 1, " ");
		spc = keyWord.find(" ", spc + 1);
	}

	if (guessWord == "") {
		msg = "";
	}
	else {
		msg = "Character '" + guessWord + "' has " + std::to_string(positions.size()) + " occurences.";
	};
}











void CCLientPlayGround::OnBnClickedRefresh()
{
	char receive_buffer[256] = { 0, };
	std::vector<std::string> res;
	
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
		if (res.size() < 8) {
			if (res[5].compare("") != 0 && res[5].compare("Correct guess") == 0) {
				MessageBox(_T("Correct guess"));
				MessageBox(_T("5"));
			}
			else if (res[5].compare("") != 0 && res[5].compare("Wrong guess") == 0) {
				MessageBox(_T("Wrong guess"));
				MessageBox(_T("6"));
			}
			CString score = _T("");
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
	
}

void CCLientPlayGround::OnTimer(UINT_PTR nIDEvent)
{
	//UpdateData(FALSE);
	///*CCLientPlayGround::OnBnClickedRefresh();*/


	//
	///*play_client2->receive_message*/
	//if (tmp.compare("") == 0) {
	//	CString score = _T("");
	//	score.Format(L"%s", tmp);
	//	txtScore.SetWindowText(score);
	//}
	//
	///*score = play_client2->receive_message.c_str();*/
	//
	//UpdateData(TRUE);
	//CDialogEx::OnTimer(nIDEvent);
	
}

