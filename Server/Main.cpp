#include "Server.h"
#include "FileReader.h"
#include "Keyword.h"
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()

using namespace std;

int main()
{			
	//// Create keyword for game turn
	srand((int)time(0));
	FileReader* file = new FileReader("database.txt", ios::in);
	vector<Keyword*> keyword_list =  file->getListKeyWord();
	file->~FileReader();  

	Keyword* selected_keyword = keyword_list[rand() % keyword_list.size()];
	std::cout << selected_keyword->keyword << " + " << selected_keyword->description << std::endl;	




	int PORT = 9090;
	int N = 2;

	SOCKET socket_receiver = INVALID_SOCKET;
	int current_appearances = 0;

	Server* server = new Server(N, PORT, socket_receiver, current_appearances);	
	server->setKeyWordList(keyword_list);
	// server->setKeyWord(selected_keyword);
	server->initiateServer();
	
	server->~Server();
	return 0;
}
