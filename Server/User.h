#pragma once

#ifndef User_h_
#define User_h_
#include <iostream>
#include <string>
#include <algorithm>
#include<vector>


class User
{
public:
    int id;
    static int current_id;
    int socket_id;
    std::string name;
    std::string status;     // success, miss, win, lose
    std::string ans;        // answer 1 character
    bool final_ans = false; // answer whole keyword
    std::string mode;               // mode of answer ('0' = character, '1' =  whole keyword)
    int score = 0;
    bool turn = false;    // current turn
    int rank;
  

    User(): id(current_id++) {}

    User(std::string name, int score) {
        this->name = name;
        this->score = score;
    }

    User(std::string name, std::string status, std::string ans, int final_ans, int mode, int score, bool turn, int rank) {
        this->name = name;
        this->status = status;
        this->ans = ans;
        this->final_ans = final_ans;
        this->mode = mode;
        this->score = score;
        this->turn = turn;
        this->rank = rank;
    };

    void printUser();

};


#endif // !User_h_