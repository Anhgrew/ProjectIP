#include "Server.h"
using namespace std;


void Server::initiateServer()
{
	// Initiate env
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
		std::cout << std::endl << "The WSA failed !!!";
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << std::endl << "The WSA init successfully" << std::endl;
	}
	
	// config socket address
	memset(&receiver, 0, sizeof(receiver));		//Initialize sockaddr
	receiver.sin_family = AF_INET;			//Server IP's type = IPv4
	receiver.sin_addr.s_addr = inet_addr("127.0.0.1");	//Can connect to any interface
	receiver.sin_port = htons(9090);			//The port number is 9090

	// config socket
	socket_receiver = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_receiver == INVALID_SOCKET)
	{
		printf("Can't create the server's receiver socket!\n");
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	int res;

	// About the blocking vs non blocking socket
	// optval = 0 mean blocking =1 mean non blocking

	u_long optval = 1;		

	res = ioctlsocket(socket_receiver, FIONBIO, &optval);

	if (res != 0) {
		WSACleanup();
		std::cout << "Non block failed !!!" << std::endl;
	}
	else {
		std::cout << "Non block passed !!!" << std::endl;
	}

	//Bind the socket to the port (port 9090)
	//printf("Bind()\n");

	int optVal = 0;
	int nLen = sizeof(optVal);

	res = setsockopt(socket_receiver, SOL_SOCKET, SO_REUSEADDR, (const char*)&nLen, nLen);

	if (res < 0)
	{
		std::cout << std::endl << "The setsockopt failed";
	}

	res = bind(socket_receiver, (const sockaddr*)&receiver, sizeof(receiver));

	if (res == SOCKET_ERROR)
	{
		printf("Can't bind the server's socket!\n");
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	//Listen for the connection from clients

	res = listen(socket_receiver, N);

	if (res < 0) {
		std::cout << std::endl << "The listening failed to local port !!!";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << std::endl << "Listening successfully to the local port" << std::endl;
	}

	struct timeval tv;

	tv.tv_sec = 5;
	tv.tv_usec = 0;

	int nMaxFd = socket_receiver + 1;

	this->keyword = keyword_list[rand() % keyword_list.size()];
	cout << "KEYWORD: " << this->keyword->keyword << endl;
	while (1) {
		if (start_new_game) {
			this->keyword = keyword_list[rand() % keyword_list.size()];
			cout << "KEYWORD: " << this->keyword->keyword << endl;
		}
		FD_ZERO(&fr);
		FD_ZERO(&fw);
		FD_ZERO(&fe);

		FD_SET(socket_receiver, &fr);

		FD_SET(socket_receiver, &fe);

		for (int i = 0; i < N; i++)
		{
			if (clients[i] != 0) {
				FD_SET(clients[i], &fr);
				FD_SET(clients[i], &fe);
			}
		}
		res = select(nMaxFd + 1, &fr, &fw, &fe, &tv);

		if (res > 0 && current_appearances <= N) {
			ProcessNewRequest();
		}
		else if (res == 0) {
			std::cout << " Nothing on port !!!" << std::endl;
		}
		else {						
			std::cout << "Errors or user limit exceed" << std::endl;
			getchar();
			std::cout << "Press any key to exit" << std::endl;
			WSACleanup();
			exit(EXIT_FAILURE);
		}
	}
	WSACleanup();
}

//int Server::ProcessNewMessage(int client_socket)
//{
//	std::cout << "Process the new message for client socket: " << " [" <<client_socket << "]" <<std::endl;
//	char buffer[256] = { 0, };
//	int relIn;
//
//	bool check = false;
//
//
//
//	relIn = recv(client_socket, buffer, 256, 0);
//	std::cout << buffer << std::endl;
//	if (relIn == -1) return -1;
//	std::string return_message = isExistingUser(buffer);
//	if (return_message.compare("") !=0 && return_message.compare("Registration Completed Successfully") == 0) {
//		User* user = new User(buffer, 0);
//		user->socket_id = client_socket;
//		users.push_back(user);
//		send(client_socket, "Success !", 10, 0);
//
//	}
//	else if (return_message.compare("") != 0 && isExistingUser(buffer).compare("Name is too long !") == 0) {
//		send(client_socket, "Please chose the shorter name !", 31, 0);
//	}
//	else {
//		send(client_socket, "Please chose the another name !", 32, 0);
//	}
//
//	if (recv(client_socket, buffer, 256, 0) == 0) {
//		current_appearances--;
//		// When number of socket == 0 reset server 
//		if (current_appearances == 0) full = false;
//		std::cout << "Failed to process new message something went wrong" << std::endl;
//		closesocket(client_socket);
//		for (int i = 0; i < N; i++) {
//			if (clients[i] == client_socket) {
//				clients[i] = 0;
//				break;
//			}
//		}
//	}
//	else {
//		std::cout << "Message recieved from client " << buffer << std::endl;
//		send(client_socket, "Proceed your request, server send back", 39, 0);
//	}
//
//}



void Server:: ProcessNewMessage(int client_socket) {
	
	cout << "Process the new message for client socket: " << client_socket << endl;
	char buffer[256] = { 0, };
	int relIn = recv(client_socket, buffer, 256, 0);
	if (relIn == -1) {
		
		current_appearances--;
		// When number of socket == 0 reset server 
		if (current_appearances == 0) {
			full = false;
			start = false;
		}

		cout << "Failed to process new message something went wrong" << endl;
		closesocket(client_socket);

		for (int i = 0; i < users.size(); i++) {
			
			if (users[i]->socket_id == client_socket) {	
				users.erase(users.begin()+i);
				break;
			}
		}
		for (int i = 0; i < N; i++) {
			if (clients[i] == client_socket) {
				clients[i] = 0;
				break;
			}
		}
	}
	else {
		if (users.size() < N) {
			std::cout << "Message recieved from client " << buffer << endl;
			std::string message = isExistingUser(buffer, client_socket);
			cout << ">>>>>>>>>>" << message << "<<<<<<<<<<<<<<" << users.size() <<  endl;
			send(client_socket, message.c_str(), 70, 0);
			cout << endl << "**********************************";
		}
		if (users.size() == N || start) {
			start = true;
			ProcessUsers(buffer, client_socket);
			//int id;
			//static int current_id;
			//int socket_id;
			//std::string name;
			//std::string status;     // success, miss, win, lose
			//std::string ans;        // answer 1 character
			//std::string final_ans;  // answer whole keyword
			//int mode;               // mode of answer (0 = character, 1 =  whole keyword)
			//int score;
			//bool turn = false;    // current turn
			//int rank;
			//for (int i = 0; i < users.size(); i++)
			//{
			//	if (users[i] != NULL && users[i]->socket_id != 0) {	

			//		int len = keyword->keyword.size();
			//		string message = string("Let 's start").append(",")
			//			.append(to_string(keyword->keyword.size()))
			//			.append(",")
			//			.append(keyword->description)
			//			.append(",")
			//			.append(to_string(users[i]->id))
			//			.append(",")
			//			.append(users[i]->name)
			//			.append(",")
			//			.append(to_string(users[i]->score));
			//		
			//		send(users[i]->socket_id, message.c_str(), message.size() , 0);


			//		//send(clients[i], hiden_len, strlen(hiden_len), 0);
			//		//send(clients[i], keyword->description.c_str(), strlen(keyword->description.c_str()), 0);
			//	}
			//}

			//cout << buffer << "AAA" << endl;

			

		}
		if (users.size() > N) {

		}


	}


}

void Server::setKeyWord(Keyword* keyword)
{
	this->keyword = keyword;
}


void Server::ProcessNewRequest() {
	cout << "aaaaa->>>>" << current_appearances << endl;

	if (FD_ISSET(socket_receiver, &fr)) {
		current_appearances++;
		if (!full && current_appearances == N) {
			full = true;
			int len = sizeof(struct sockaddr);
			int client_socket = accept(socket_receiver, NULL, &len); // return ID client process
			if (client_socket > 0) {
				for (int i = 0; i < N; i++)
				{
					if (clients[i] == 0) {
						clients[i] = client_socket;
						cout << " -------> " << client_socket << endl;
						break;
					}
					else {
						//send(clients[i], "++++++++\ Let 's start /++++++++\n", 32, 0);
					}
				}

			}

		}
		else if (!full && current_appearances < N) {
			int len = sizeof(struct sockaddr);
			int client_socket = accept(socket_receiver, NULL, &len); // return ID client process
			if (client_socket > 0) {
				for (int i = 0; i < N; i++)
				{
					if (clients[i] == 0) {
						clients[i] = client_socket;
						cout << " -------> " << client_socket << endl;
						break;
					}
				}

			}
		}
		else {
			// greater than N socket, accept, send mess and ignore
			current_appearances--;
			int len = sizeof(struct sockaddr);
			int client_socket = accept(socket_receiver, NULL, &len);
			/*send(client_socket, "full", 5, 0);*/
			closesocket(client_socket);
			cout << "Users limit exceed, So we rejected socket: " << client_socket << endl;
		}


	}
	else if (FD_ISSET(socket_receiver, &fe)) {
		cout << "Exception !" << endl;
	}
	else if (FD_ISSET(socket_receiver, &fw)) {
		cout << "Ready to write !" << endl;
	}
	
	for (int i = 0; i < N; i++)
	{
		if (FD_ISSET(clients[i], &fr)) {

			ProcessNewMessage(clients[i]);

		}
	}

			
	

}

vector<string> Server::split(string s, string delimiter)
{
	vector<std::string> res;
	int last = 0; int next = 0;
	while ((next = s.find(delimiter, last)) != string::npos) {
		res.push_back(s.substr(last, next - last));
		last = next + 1;
	}
	res.push_back(s.substr(last));
	return res;
}

void Server::setKeyWordList(vector<Keyword*> keywords)
{
	this->keyword_list = keywords;
}


void Server::ProcessUsers(char buffer[256], int client_socket)
{
	if (!start_receive_ans) {
		for (int i = 0; i < users.size(); i++)
		{
			if (users[i] != NULL && users[i]->socket_id != 0) {
				users[0]->turn = true;
				string message = string("Let 's start")
					.append(",")
					.append(to_string(keyword->keyword.size()))
					.append(",")
					.append(keyword->description)
					.append(",")
					.append(to_string(users[i]->id))
					.append(",")
					.append(users[i]->name)
					.append(",")
					.append(to_string(users[i]->score))
					.append(",")
					.append(users[i]->turn ? "Your turn": "No turn");
				send(users[i]->socket_id, message.c_str(), message.size(), 0);


				//send(clients[i], hiden_len, strlen(hiden_len), 0);
				//send(clients[i], keyword->description.c_str(), strlen(keyword->description.c_str()), 0);
			}
		}
	}
	else {
		vector<string> ans = split(buffer, ",");
		transform(ans[0].begin(), ans[0].end(), ans[0].begin(), ::toupper);
		transform(keyword->keyword.begin(), keyword->keyword.end(), keyword->keyword.begin(), ::toupper);
		string response_message = "";
		cout << "---"<< ans[0] << "-";
		cout << "---------" << keyword->keyword << "--------" << endl;
		for (auto user : users) {
			if (user->socket_id == client_socket) {
				user->ans = ans[0];
				user->mode = ans[1];
				if (ans[1].compare("0") == 0) {

					if (keyword->keyword.find(ans[0]) != std::string::npos) {
						response_message = "Correct guess";
					}
					else {
						response_message = "Wrong guess";
					}
				}
				else if (ans[1].compare("1") == 0) {
					if (keyword->keyword.compare(ans[0]) == 0) {
						response_message = "Correct keyword";
					}
					else {
						response_message = "Wrong keyword";
					}
				}
				user->status = response_message;
				break;
			}
		}
		

	
		for (int i = 0; i < users.size(); i++)
		{
			if (users[i] != NULL && users[i]->socket_id != 0 && users[i]->socket_id == client_socket) {
				
				if (users[i]->turn) {
					
					if (i <= users.size()-1 && (response_message.compare("Wrong keyword") == 0 || response_message.compare("Wrong guess") == 0)) {
						users[i]->turn = false;

						if (response_message.compare("Wrong keyword") == 0) users[i]->final_ans = true;

						for (int j = i + 1; j < users.size(); j++) {	
							if (users[j]->final_ans == false && users[j]->socket_id != 0) {
								
								users[j]->turn = true;
								break;
							}
						}
					}
					else if (i < users.size() - 1 && response_message.compare("Correct guess") == 0) {
						users[i]->score += 1;
					}
					else if (i < users.size() - 1 && response_message.compare("Correct keyword") == 0) {
						winner = users[i]->name;
						users[i]->score += 5;
						game_end = true;
					}

					string message = to_string(keyword->keyword.size())
						.append(",")
						.append(keyword->description)
						.append(",")
						.append(to_string(users[i]->id))
						.append(",")
						.append(users[i]->name)
						.append(",")
						.append(to_string(users[i]->score))
						.append(",")
						.append(response_message)
						.append(",")
						.append(users[i]->turn ? "Your turn" : "No turn");
					if (game_end) {
						message.append(",").append("Congratulations to the winner [ " + winner + " ]" + " with the correct keyword is: " + keyword->keyword);
						
					}
					if (users[i]->final_ans) {
						message.append(",").append("Lost the game with score: " + to_string(users[i]->score));
					}
					send(users[i]->socket_id, message.c_str(), message.size(), 0);
					if (game_end) {
						start_new_game = true;
					}
				}
				else {


					//int len = keyword->keyword.size();

					//string message = to_string(keyword->keyword.size())
					//	.append(",")
					//	.append(keyword->description)
					//	.append(",")
					//	.append(to_string(users[i]->id))
					//	.append(",")
					//	.append(users[i]->name)
					//	.append(",")
					//	.append(to_string(users[i]->score))
					//	.append(",")
					//	.append(response_message)
					//	.append(",");
					//if (game_end) {
					//	message.append("Congratulations to the winner [ " + users[i]->name + " ]" + " with the correct keyword is: " + keyword->keyword);
					//}

					//send(users[i]->socket_id, message.c_str(), message.size(), 0);
				}
				//send(clientsm[i], hiden_len, strlen(hiden_len), 0);
				//send(clients[i], keyword->description.c_str(), strlen(keyword->description.c_str()), 0);
			}
			else {
				cout << client_socket << "iiiiiiiiiiiiiiiiiiii" << users[i]->name << endl;
				string message = to_string(keyword->keyword.size())
					.append(",")
					.append(keyword->description)
					.append(",")
					.append(to_string(users[i]->id))
					.append(",")
					.append(users[i]->name)
					.append(",")
					.append(to_string(users[i]->score))
					.append(",")
					.append(response_message)
					.append(",")
					.append(users[i]->turn ? "Your turn" : "No turn")
					.append(",");
				if (game_end) {
					message.append("Congratulations to the winner [ " + winner + " ]" + " with the correct keyword is: " + keyword->keyword);
				}
				send(users[i]->socket_id, message.c_str(), message.size(), 0);
			}
		}
		
	}
	start_receive_ans = true;
}







string Server::isExistingUser(std::string name, int client_socket)
{
	if (name.size() > 10) {
		return "Name is longer than 10 character. Please input again !";
	}
	for (auto user : users) {
		if (user->name.compare(name) == 0) {
			return "Existed Name. Please input again !";
		}
	}

	User* new_user = new User();
	new_user->name = name;
	new_user->score = 0;
	new_user->socket_id = client_socket;

	users.push_back(new_user);
	return "Registration Completed Successfully";
}


