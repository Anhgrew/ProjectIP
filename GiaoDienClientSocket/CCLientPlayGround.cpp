// CCLientPlayGround.cpp : implementation file
//

#include "pch.h"
#include "GiaoDienClientSocket.h"
#include "GiaoDienClientSocketDlg.h"
#include "CCLientPlayGround.h"
#include <windows.h>
#include "LogNoti.h"
#include <thread>

void HiddenRemain(std::string guessWord, std::string& keyWord, std::string& msg, std::string& disWord);
// CCLientPlayGround dialog

IMPLEMENT_DYNAMIC(CCLientPlayGround, CDialogEx)
// HANDLE refreshThread;

CCLientPlayGround::CCLientPlayGround(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENTPLAYGROUND, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
	DDX_Control(pDX, IDC_CLOCK, clock);
}


BEGIN_MESSAGE_MAP(CCLientPlayGround, CDialogEx)
	ON_BN_CLICKED(IDC_SENDANSWER, &CCLientPlayGround::OnBnClickedSendanswer)
	ON_WM_TIMER()

END_MESSAGE_MAP()


BOOL CCLientPlayGround::OnInitDialog() {


	CDialogEx::OnInitDialog();
	// Init if required, can add later



	UpdateData(TRUE);

	// set initial timer

	clock_time.Format(L"%d", time);
	
	clock.SetWindowTextW(clock_time);

	GetDlgItem(IDC_SENDANSWER)->EnableWindow(FALSE);
	char receive_buffer[256];
	std::memset(receive_buffer, 0, sizeof receive_buffer);
	std::vector<std::string> res;

	if (recv(nSocket, receive_buffer, 256, 0) == -1) {
		MessageBox(_T("Can not receive"));
	}
	else {
		res = split(receive_buffer, ",");

		if (res.size() >= 9 && res[0].compare("Let 's start") == 0 && res[6].compare("Your turn") == 0) {
			// start timer thread;
			
			time = TIME;

			clock_time.Format(L"%d", time);
			clock.SetWindowTextW(clock_time);

			thread_timer = AfxBeginThread(staticThreadHandleTimer, this);
			handle_timer = thread_timer->m_hThread;

			GetDlgItem(IDC_SENDANSWER)->EnableWindow(TRUE);
			listPlayGround.AddString(CString("HINT: ") + (LPCTSTR)strconverter.from_bytes(res[2]).c_str());
			disWord = res[8];
			HiddenRemain("", res[7], msg, disWord);
			listPlayGround.AddString(CString("KEYWORD: ") + disWord.c_str());
			listPlayGround.SetCurSel(listPlayGround.GetCount() - 1);
		}
		else {

			if (thread_timer != NULL) {
				TerminateThread(handle_timer, 0);
				CloseHandle(handle_timer);
				thread_timer = NULL;
				time = TIME;
			}
			time = TIME;
			clock_time.Format(L"%d", time);
			clock.SetWindowTextW(clock_time);

			GetDlgItem(IDC_SENDANSWER)->EnableWindow(FALSE);
			listPlayGround.AddString(CString("HINT: ") + (LPCTSTR)strconverter.from_bytes(res[2]).c_str());
			disWord = res[8];
			HiddenRemain("", res[7], msg, disWord);
			listPlayGround.AddString(CString("KEYWORD: ") + disWord.c_str());
			listPlayGround.SetCurSel(listPlayGround.GetCount() - 1);

			MessageBox(_T("Please wait for your turn..."));
		}

	}

	//set readonly when turn < 2

	txtKeyWord.SetReadOnly(TRUE);

	CString name;
	name = res[4].c_str();
	txtClientName.SetWindowText(name);
	CString score;
	score = res[5].c_str();
	txtScore.SetWindowTextW(score);

	UpdateData(FALSE);

	thread = AfxBeginThread(staticThreadHandle, this);
	handle = thread->m_hThread;

	return TRUE;
}

// CCLientPlayGround message handlers

void CCLientPlayGround::OnBnClickedSendanswer(){
	UpdateData(TRUE);

	txtGuessWord.GetWindowTextW(guessW);
	txtKeyWord.GetWindowTextW(keyW);
	if (keyW == _T("")) {
		keyW = string("#").c_str();
	}
	if (guessW == _T("")) {
		guessW = string("#").c_str();
	}

	if(guessW.GetLength() > 1) {
		MessageBox(_T("Please input only one character for guess word..."));
	}
	else {
		responseMsg = keyW + "," + guessW;
		// Terminate timer thread reset timer
		if (thread_timer != NULL) {
			TerminateThread(handle_timer, 0);
			CloseHandle(handle_timer);
			thread_timer = NULL;
			time = TIME;
		}
		time = TIME;
		clock_time.Format(L"%d", time);
		clock.SetWindowTextW(clock_time);

		send(nSocket, CStringA(responseMsg), 256, 0);

		txtGuessWord.SetWindowTextW(_T(""));
		txtKeyWord.SetWindowTextW(_T(""));
	}



	UpdateData(FALSE);
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


UINT __cdecl CCLientPlayGround::staticThreadHandleTimer(LPVOID pParam)
{
	CCLientPlayGround* timer = reinterpret_cast<CCLientPlayGround*>(pParam);
	UINT return_code = timer->threadHandleTimer();
	return return_code != 0 ? return_code : 0;
}

UINT CCLientPlayGround::threadHandleTimer()
{
	while (time >= 0) {
		time--;
		Sleep(1000);
		clock_time.Format(L"%d", time);
		clock.SetWindowTextW(clock_time);
		if (time == 0) {

			MessageBox(_T("Time Over !!!"));
			send(nSocket, CStringA(_T("#,#")), 256, 0);
			break;
		}
	}
	

	return 0;
}

UINT __cdecl CCLientPlayGround::staticThreadHandle(LPVOID pParam)
{
	CCLientPlayGround* play_ground = reinterpret_cast<CCLientPlayGround*>(pParam);
	UINT return_code = play_ground->threadHandle();
	return return_code != 0 ? return_code : 0;
	
}

UINT CCLientPlayGround::threadHandle()
{

	char receive_buffer[256];
	std::vector<std::string> res;
	std::memset(receive_buffer, 0, sizeof receive_buffer);
	

	while (recv(nSocket, receive_buffer, 256, 0) != SOCKET_ERROR)
	{
		
		if (checkTurnTwo >= 1) {
			txtKeyWord.SetReadOnly(FALSE);
			if (checkTurnTwo == 1) {
				MessageBox(_T("Turn two, you can give the full keyword to win or lose"));
			}
		}



		res = split(receive_buffer, ",");
		std::string gW = CT2A(guessW);
		std::string kW = CT2A(keyW);
		transform(gW.begin(), gW.end(), gW.begin(), ::toupper);
		transform(kW.begin(), kW.end(), kW.begin(), ::toupper);

		if (res.size() >= 8 && res[6].compare("Reset turn") == 0) {

			//reset clock

			if (res[3].compare("0") == 0) {
				if (thread_timer != NULL) {
					TerminateThread(handle_timer, 0);
					CloseHandle(handle_timer);
					thread_timer = NULL;
					time = TIME;
				}

				time = TIME;
				clock_time.Format(L"%d", time);
				clock.SetWindowTextW(clock_time);

				GetDlgItem(IDC_SENDANSWER)->EnableWindow(TRUE);
				time = TIME;
				//thread_timer = AfxBeginThread(staticThreadHandleTimer, this);
				//handle_timer = thread_timer->m_hThread;
				res[6] = "Your turn";
			
			}
			else {

				if (thread_timer != NULL) {
					TerminateThread(handle_timer, 0);
					CloseHandle(handle_timer);
					thread_timer = NULL;
					time = TIME;
				}
				GetDlgItem(IDC_SENDANSWER)->EnableWindow(FALSE);
				res[6] = "No turn";
				time = TIME;
				clock_time.Format(L"%d", time);
				clock.SetWindowTextW(clock_time);
			}

			//
			listPlayGround.AddString(CString("______________NEW GAME__________________________"));
			
			listPlayGround.AddString(CString("HINT: ") + (LPCTSTR)strconverter.from_bytes(res[2]).c_str());
			disWord = res[8];
			HiddenRemain("", res[7], msg, disWord);
			listPlayGround.AddString(CString("KEYWORD: ") + disWord.c_str());
			listPlayGround.SetCurSel(listPlayGround.GetCount() - 1);
			MessageBox(_T(">>> New game <<<"));
		}

		if (res.size() >= 10) {
			if (res[9].compare("") != 0 && res[9].find("Lost") != std::string::npos) {
				if (thread_timer != NULL) {
					TerminateThread(handle_timer, 0);
					CloseHandle(handle_timer);
					thread_timer = NULL;
					time = TIME;
				}
				time = TIME;
				clock_time.Format(L"%d", time);
				clock.SetWindowTextW(clock_time);
				MessageBox(_T("You lost"));
			}
			else if (res[9].compare("") != 0 && res[9].find("Congratulations") != std::string::npos) {
				if (thread_timer != NULL) {
					TerminateThread(handle_timer, 0);
					CloseHandle(handle_timer);
					thread_timer = NULL;
					time = TIME;
				}

				time = TIME;
				clock_time.Format(L"%d", time);
				clock.SetWindowTextW(clock_time);

				MessageBox(_T("Congratulations, you are the winner")); 
				disWord = res[7];
				for (int i = 10; i < 12; i++) {
					MessageBoxA(NULL, res[i].c_str(), "Summary ", MB_OK);
				}
			}
			else if (res[5].compare("") != 0 && res[5].compare("Correct guess") == 0) {
				MessageBox(_T("Correct guess"));
				disWord = res[8];
				HiddenRemain(gW, res[7], msg, disWord);
		
			}
			else if (res[5].compare("") != 0 && res[5].compare("Wrong guess") == 0) {
				MessageBox(_T("Wrong guess"));
			}
			else if (res[5].compare("") != 0 && res[5].compare("Not answer") == 0 && res[6].compare("Your turn") == 0) {
				MessageBox(_T("Previous player does not reply. Your turn !!!"));
			}

			CString name;
			name = res[3].c_str();
			txtClientName.SetWindowText(name);

			CString score = _T("");
			score = res[4].c_str();
			txtScore.SetWindowTextW(score);

			if (res[6].compare("Your turn") == 0) {

				// start timer thread;
				time = TIME;
				clock_time.Format(L"%d", time);
				clock.SetWindowTextW(clock_time);
				thread_timer = AfxBeginThread(staticThreadHandleTimer, this);
				handle_timer = thread_timer->m_hThread;

				GetDlgItem(IDC_SENDANSWER)->EnableWindow(TRUE);
			}
			else {

				if (thread_timer != NULL) {
					TerminateThread(handle_timer, 0);
					CloseHandle(handle_timer);
					thread_timer = NULL;
					time = TIME;
				}

				time = TIME;
				clock_time.Format(L"%d", time);
				clock.SetWindowTextW(clock_time);

				GetDlgItem(IDC_SENDANSWER)->EnableWindow(FALSE);
			}

		}
		if (res.size() < 10 && res.size() >= 7) {
			checkTurnTwo++;
			if (res[5].compare("") != 0 && res[5].compare("Correct guess") == 0) {
				MessageBox(_T("Correct guess"));
				disWord = res[8];
				HiddenRemain(gW, res[7], msg, disWord);

			}
			else if (res[5].compare("") != 0 && res[5].compare("Wrong guess") == 0) {
				MessageBox(_T("Wrong guess"));
			}
			else if (res[5].compare("") != 0 && res[5].compare("Not start") == 0) {
				checkTurnTwo--;
				MessageBox(_T("Please wait for game starting"));
			}	
			else if (res[5].compare("") != 0 && res[5].compare("Not answer") == 0 && res[6].compare("Your turn") == 0) {
				MessageBox(_T("Previous player does not reply. Your turn !!!"));
			}
			

			// hinh nhu correct thi moi can chay khuc duoi dien vo list
			CString name;
			name = res[3].c_str();
			txtClientName.SetWindowText(name);

			CString score = _T("");;
			score = res[4].c_str();
			txtScore.SetWindowTextW(score);

			if (res[6].compare("Your turn") == 0) {

				// start timer thread;

				time = TIME;
				clock_time.Format(L"%d", time);
				clock.SetWindowTextW(clock_time);
				thread_timer = AfxBeginThread(staticThreadHandleTimer, this);
				handle_timer = thread_timer->m_hThread;

				GetDlgItem(IDC_SENDANSWER)->EnableWindow(TRUE);
			}
			else {

				if (thread_timer != NULL) {
					TerminateThread(handle_timer, 0);
					CloseHandle(handle_timer);
					thread_timer = NULL;
					time = TIME;
				}

				time = TIME;
				clock_time.Format(L"%d", time);
				clock.SetWindowTextW(clock_time);

				GetDlgItem(IDC_SENDANSWER)->EnableWindow(FALSE);
			}

		}

		listPlayGround.AddString(CString("KEYWORD: ") + disWord.c_str());
		listPlayGround.SetCurSel(listPlayGround.GetCount() - 1);

		std::memset(receive_buffer, 0, sizeof receive_buffer);
		
	}

	return 0;
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




