#include<iostream>
#include<string>
#include <stdio.h>
#include<vector>
#include <winsock.h>
//#include "FileReader.h"
//#include "Keyword.h"
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()
#define PORT 9090
using namespace std;

vector<string> split(string s, string delimiter)
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

int main()
{
    //srand((int)time(0));
    //FileReader * file = new FileReader("database.txt", ios::in);
    //vector<Keyword*> keyword_list = file->getListKeyWord();
    //Keyword *selected_keyword = keyword_list[rand() % keyword_list.size()];
    //std::cout << selected_keyword->keyword << " + " << selected_keyword->description << std::endl;
    //file->~FileReader();


    /*Initiate the Socket environment*/
    WSADATA w;
    int res = 0;

    sockaddr_in srv;

    res = WSAStartup(MAKEWORD(2, 2), &w);
    if (res < 0)
    {
        printf("\nCannot Initialize socket lib");
        return -1;
    }
    //Open a socket - listener
    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nSocket < 0)
    {
        //errno is a system global variable which gets updated
        //with the last API call return value/result.
        printf("\nCannot Initialize listener socket:%d", errno);;
        return -1;
    }

    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");
    srv.sin_port = htons(PORT);
    memset(&(srv.sin_zero), 0, 8);
    
    u_long optval = 0;

    res = ioctlsocket(nSocket, FIONBIO, &optval);

    if (res != 0) {
        std::cout << "Non block failed !!!" << std::endl;
        WSACleanup();
    }
    else {
        std::cout << "Non block passed !!!" << std::endl;
    }

    res = connect(nSocket, (struct sockaddr*)&srv, sizeof(srv));
    if (res < 0)
    {
        printf("\nCannot connect to server:%d", errno);
        WSACleanup();
        return -1;
    }
    else {
        printf("Connect to server\n");
        char receive_buffer[256] = { 0 };
        string send_buffer;
        recv(nSocket, receive_buffer, 255, 0);
        //std::cout << "press any key to see the message from server ";
        //getchar();
        if (string(receive_buffer).compare("full") == 0) {
            cout << "Full queue!!!" << endl;
            getchar();
            cout << "Press any key to exit" << endl;
            WSACleanup();
            exit(EXIT_FAILURE);
        }       
        cout << endl << receive_buffer << endl;
        int index = 0;
        while (1) {
          
            if (recv(nSocket, receive_buffer, 256, 0) == -1) {
                break;
            }
            cout << endl <<receive_buffer << endl;
            if (string(receive_buffer).compare("full") == 0) {
                cout << "Full queue!!!" << endl;
                cout << "Press any key to exit" << endl;
                Sleep(3000);
                WSACleanup();
                exit(EXIT_FAILURE);
            }
            else if (string(receive_buffer).compare("Registration Completed Successfully") == 0) {
                break;
            }
            cout << "Input:" << endl;
            
            getline(cin, send_buffer);
            send(nSocket, send_buffer.c_str(), 256, 0);
            cout << "Sended.." << endl;
        }

        char receive_buffer1[256] = { 0, };        
        vector<string> res;
        send_buffer = "";

        while (1) {

            while (1) {

                if (recv(nSocket, receive_buffer1, 256, 0) == -1) {
                    break;
                }
                cout << endl << receive_buffer1 << endl;

                res = split(receive_buffer1, ",");
                index = 0;
                for (auto i : res) {
                    cout << "Word: " << index++ << " - " << i << endl;
                }
                if (res.size() > 0 && res[6].compare("Your turn") == 0) {
                    cout << "Input:" << endl;
                    getline(cin, send_buffer);
                    send_buffer.append(",").append("1");
                    send(nSocket, send_buffer.c_str(), 256, 0);
                    cout << "Sended.." << endl;
                }

                else {
                    if (res.size() >= 8 && res[7] != "" && res[7].find("Lost") != std::string::npos) {
                        cout << "Wait..." << endl;
                        break;
                    }
                    if (res.size() >= 8 && res[7] != "" && res[7].find("Congratulations") != std::string::npos) {
                        cout << "~~~ You win. End game ~~~" << endl;
                        break;
                    }
                }
            }
        }

    }

    ////Keep sending the messages 
    //char sBuff[1024] = { 0, };
    //while (1)
    //{
    //    Sleep(2000);
    //    printf("\nWhat message you want to send..?\n");
    //    fgets(sBuff, 1023, stdin);
    //    send(nSocket, sBuff, strlen(sBuff), 0);
    //}
    Sleep(300000);
    WSACleanup();
    
}