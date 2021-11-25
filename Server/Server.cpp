#include "Server.h"



User* Server::findNextUser(int index)
{
	int i = index;
	User* next_user = nullptr;
	while (1) {
		if (i == users.size() - 1) {
			i = 0;
			if (users[i] != nullptr && users[i]->final_ans == false && users[i]->turn == false) {
				next_user = users[i];
				break;
			}
		}
		if (i < users.size() - 1 && users[i+1] != nullptr && users[i+1]->final_ans == false && users[i+1]->turn == false) {
			++i;
			next_user = users[i];
			break;
		}
	}
	return next_user;
}

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
	receiver.sin_addr.s_addr = INADDR_ANY;	//Can connect to any interface
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
	std::cout << "KEYWORD: " << this->keyword->keyword << std::endl;
	this->disword = std::string(this->keyword->keyword.length(), '*');
	while (1) {
		if (start_new_game || (five_turn_check == 5 && has_ans == false)) {
			this->keyword = keyword_list[rand() % keyword_list.size()];
			std::cout << "KEYWORD: " << this->keyword->keyword << std::endl;
			this->disword = std::string(this->keyword->keyword.length(), '*');
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

void Server::ProcessNewMessage(int client_socket) {

	std::cout << "Process the new message for client socket: " << client_socket << std::endl;

	char buffer[256] = { 0, };
	int relIn = recv(client_socket, buffer, 256, 0);
	std::cout << "User send to: " << std::string(buffer) << std::endl;
	if (relIn == -1) {

		current_appearances--;
		// When number of socket == 0 reset server 
		if (current_appearances == 0) {
			full = false;
			start = false;
		}

		std::cout << "Failed to process new message something went wrong" << std::endl;
		closesocket(client_socket);

		for (int i = 0; i < users.size(); i++) {

			if (users[i]->socket_id == client_socket) {
				users.erase(users.begin() + i);
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

		std::vector<std::string> register_name = split(std::string(buffer), ",");

		char* receive_mess_client = new char[register_name[0].length() + 1];
		strcpy(receive_mess_client, register_name[0].c_str());
		std::string typing = "";
		if (register_name.size() > 1) {
			typing = register_name[1];
		}

		if (users.size() < N && typing.compare("register") == 0) {

			std::cout << "Message recieved from client " << receive_mess_client << std::endl;
			std::string message = isExistingUser(receive_mess_client, client_socket);
			std::cout << "AAAA" << std::endl;

			send(client_socket, message.c_str(), 70, 0);
			for (int i = 0; i < users.size(); i++) {

				if (users[i]->socket_id == client_socket) {
					if (message.compare("Registration Completed Successfully") == 0) {
						std::string ms = std::string("Let 's start")
							.append(",")
							.append(std::to_string(keyword->keyword.size()))
							.append(",")
							.append(keyword->description)
							.append(",")
							.append(std::to_string(users[i]->id))
							.append(",")
							.append(users[i]->name)
							.append(",")
							.append(std::to_string(users[i]->score))
							.append(",")
							.append(users[i]->turn ? "Your turn" : "No turn")
							.append(",")
							.append(keyword->keyword)
							.append(",")
							.append(disword);
						send(users[i]->socket_id, ms.c_str(), ms.size(), 0);
						std::cout << "Return message: " << ms << std::endl;
						break;
					}

				}
			}


			std::cout << std::endl << "<________________________________________________________>";
		}
		else if (users.size() < N && typing.compare("register") != 0) {
			std::string failmess;
			for (auto user : users) {
				if (user->socket_id == client_socket) {
					failmess = std::to_string(keyword->keyword.size())
						.append(",")
						.append(keyword->description)
						.append(",")
						.append(std::to_string(user->id))
						.append(",")
						.append(user->name)
						.append(",")
						.append(std::to_string(user->score))
						.append(",")
						.append("Not start")
						.append(",")
						.append("Your turn")
						.append(",")
						.append(keyword->keyword)
						.append(",")
						.append(disword);
					send(client_socket, failmess.c_str(), failmess.size(), 0);
					break;
				}
			}
		}
		else if (users.size() == N || start) {
			start = true;
			ProcessUsers(buffer, client_socket);
		}
	}

}

void Server::setKeyWord(Keyword* keyword)
{
	this->keyword = keyword;
}


void Server::ProcessNewRequest() {

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
						std::cout << " -------> " << client_socket << std::endl;
						send(client_socket, "Got connected !!!\n", 18, 0);

						break;
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
						std::cout << " -------> " << client_socket << std::endl;
						send(client_socket, "Got connected !!!\n", 18, 0);
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
			closesocket(client_socket);
			std::cout << "Users limit exceed, So we rejected socket: " << client_socket << std::endl;
		}

	}
	else if (FD_ISSET(socket_receiver, &fe)) {
		std::cout << "Exception !" << std::endl;
	}
	else if (FD_ISSET(socket_receiver, &fw)) {
		std::cout << "Ready to write !" << std::endl;
	}

	for (int i = 0; i < N; i++)
	{
		if (FD_ISSET(clients[i], &fr)) {

			ProcessNewMessage(clients[i]);

		}
	}

}

std::vector< std::string> Server::split(std::string s, std::string delimiter)
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

void Server::setKeyWordList(std::vector<Keyword*> keywords)
{
	this->keyword_list = keywords;
}


void Server::ProcessUsers(char buffer[256], int client_socket)
{

	std::vector< std::string> ans = split(buffer, ",");
	five_turn_check++;
	for (auto i : ans) {
		std::cout << i << " - " << std::endl;
	}
	std::string word_guess;
	if (ans.size() > 1) {
		word_guess = ans[1];
	}
	std::string word_key = ans[0];
	transform(word_key.begin(), word_key.end(), word_key.begin(), ::toupper);
	transform(keyword->keyword.begin(), keyword->keyword.end(), keyword->keyword.begin(), ::toupper);
	transform(word_guess.begin(), word_guess.end(), word_guess.begin(), ::toupper);

	std::string response_message = "";
	std::cout << "Keyword: " << word_key << std::endl;
	std::cout << "Guess word: " << word_guess << "--------" << std::endl;

	for (auto user : users) {
		if (user->socket_id == client_socket) {
			user->ans = word_key;
			user->guess = word_guess;

			if (word_key.compare("#") == 0) {
				if (keyword->keyword.find(word_guess) != std::string::npos) {
					response_message = "Correct guess";
				}
				else {
					response_message = "Wrong guess";
				}
			}
			else {
				if (keyword->keyword.compare(word_key) == 0) {
					response_message = "Correct keyword";
					has_ans = true;
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
				if (response_message.compare("Wrong keyword") == 0 || response_message.compare("Wrong guess") == 0) {
					users[i]->turn = false;

					User* next_user = findNextUser(i);

					next_user->turn = true;

					std::cout << "Next User: " << next_user->id << next_user->name << std::endl;

					if (response_message.compare("Wrong keyword") == 0) {
						users[i]->final_ans = true;
						users[i]->status = "Wrong keyword";
					}

					if (response_message.compare("Wrong guess") == 0) {
						users[i]->status = "Wrong guess";
					}

					for (int j = 0; j < users.size(); j++) {
						if (users[j]->socket_id != client_socket) {
							std::string message = std::to_string(keyword->keyword.size())
								.append(",")
								.append(keyword->description)
								.append(",")
								.append(std::to_string(users[j]->id))
								.append(",")
								.append(users[j]->name)
								.append(",")
								.append(std::to_string(users[j]->score))
								.append(",")
								.append(response_message)
								.append(",")
								.append(users[j]->turn ? "Your turn" : "No turn");

							if (game_end) {
								message.append(",").append("Congratulations to the winner [ " + winner + " ]" + " with the correct keyword is: " + keyword->keyword);
							}
							std::cout << "Send to user: " << users[j]->name << "- Socket: " << users[j]->socket_id << "- Mess: " << message << std::endl;
							send(users[j]->socket_id, message.c_str(), message.size(), 0);
						}
					}
		

				}
				else if (i <= users.size() - 1 && response_message.compare("Correct guess") == 0) {
					users[i]->score += 1;
					users[i]->status = "Correct guess";
				}
				else if (i <= users.size() - 1 && response_message.compare("Correct keyword") == 0) {
					winner = users[i]->name;
					users[i]->score += 5;
					users[i]->status = "Correct keyword";
					game_end = true;
				}

					std::string message = std::to_string(keyword->keyword.size())
						.append(",")
						.append(keyword->description)
						.append(",")
						.append(std::to_string(users[i]->id))
						.append(",")
						.append(users[i]->name)
						.append(",")
						.append(std::to_string(users[i]->score))
						.append(",")
						.append(response_message)
						.append(",")
						.append(users[i]->turn ? "Your turn" : "No turn")
						.append(",")
						.append(keyword->keyword)
						.append(",")
						.append(disword);
						
					if (game_end) {
						message.append(",").append("Congratulations to the winner [ " + winner + " ]" + " with the correct keyword is: " + keyword->keyword);
						
					}
					if (users[i]->final_ans) {
						message.append(",").append("Lost the game with score: " + std::to_string(users[i]->score));

				}
				std::cout << "Send to user: " << users[i]->name << "- Socket: " << users[i]->socket_id << "- Mess: " << message << std::endl;
				send(users[i]->socket_id, message.c_str(), message.size(), 0);
				if (game_end) {
					start_new_game = true;
				}
			}
			
		}
	
	}

}







std::string Server::isExistingUser(std::string name, int client_socket)
{
	if (name.size() > 10) {
		return "Name is longer than 10 character. Please input again !";
	}
	for (auto user : users) {
		if (user->name == name) {
			return "Existed Name. Please input again !";

		}
	}

	User* new_user = new User();
	new_user->name = name;
	new_user->score = 0;
	new_user->socket_id = client_socket;
	if (users.size() == 0) {
		new_user->turn = true;
	}
	users.push_back(new_user);

	return "Registration Completed Successfully";
}

