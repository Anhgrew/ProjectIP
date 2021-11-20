#pragma once

#ifndef Server_h_
#define Server_h_
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>  
#include <string>
#include <winsock.h>
#include <algorithm>
#include<vector>
#include"FileReader.h"
#include"User.h"

class Server
{
public:
	struct sockaddr_in receiver;
	WSADATA ws;
	int N;
	SOCKET socket_receiver = INVALID_SOCKET;
	std::vector<int>clients;
	std::vector<User*> users;
	int current_appearances = 0;
	std::vector<Keyword*> keyword_list;
	std::vector<User*> queue;
	Keyword* keyword;
	Keyword* hint;
	int PORT;
	fd_set fr, fw, fe;
	bool full = false;
	bool start = false;	
	bool start_receive_ans = false;
	bool game_end = false;
	bool start_new_game = false;
	std::string winner;

	Server(int n, int port, SOCKET socket = INVALID_SOCKET,  int current_appears = 0) {
		N = n;
		PORT = port;
		clients.resize(n);
		current_appearances = current_appears;
		socket_receiver = socket;  
	}

	~Server() {
		closesocket(socket_receiver);
		WSACleanup();
	}



	void initiateServer();

	void setKeyWordList(std::vector<Keyword*> keywords);

	void setKeyWord(Keyword* keyword);

	std::vector<std::string> split(std::string s, std::string delimiter);

	std::string isExistingUser(std::string name, int client_socket);

	void ProcessNewMessage(int client_socket);

	void ProcessNewRequest();

	void ProcessUsers(char buffer[256], int client_socket);

};

#endif // !Server_h_