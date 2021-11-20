#pragma once

#ifndef GamePlay_h_
#define GamePlay_h_

#include"Keyword.h"
#include <iostream>
#include <string>
#include <algorithm>
#include<vector>

class GamePlay
{
public:
    
    int keyword_list_size;
    std::fstream file;

    GamePlay(const char* fn, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) {
        file.open(fn, mode);
        if (!file.is_open())
        {
            std::cout << "WRONG FILE NAME OR FILE IS NOT EXISTING IN THE PROJECT!!!" << std::endl;
            exit(0);
        }

    }

    GamePlay(const std::string& fn, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) {
        file.open(fn, mode);
        if (!file.is_open())
        {
            std::cout << "WRONG FILE NAME OR FILE IS NOT EXISTING IN THE PROJECT!!!" << std::endl;
            exit(0);
        }
    }

    ~GamePlay() {
        file.close();
    }

    std::vector<Keyword*> getListKeyWord();

    void tokenize(std::string const& str, const char delim, std::vector<std::string>& out);

    void print();

};

#endif // !GamePlay_h_