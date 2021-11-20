#pragma once


#ifndef FileReader_h_
#define FileReader_h_

#include"Keyword.h"
#include"User.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include<vector>


class FileReader
{
public:
    int keyword_list_size;
    std::fstream file;

    FileReader(const char* fn, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) {
        file.open(fn, mode);
        if (!file.is_open())
        {
            std::cout << "WRONG FILE NAME OR FILE IS NOT EXISTING IN THE PROJECT!!!" << std::endl;
            exit(0);
        }

    }

    FileReader(const std::string& fn, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) {
        file.open(fn, mode);
        if (!file.is_open())
        {
            std::cout << "WRONG FILE NAME OR FILE IS NOT EXISTING IN THE PROJECT!!!" << std::endl;
            exit(0);
        }
    }

    ~FileReader() {
        file.close();
    }

    std::vector<Keyword*> getListKeyWord();

    bool checkExistingUser(std::string name);

    void tokenize(std::string const& str, const char delim, std::vector<std::string>& out);

    void print();

};

#endif // !FileReader_h_