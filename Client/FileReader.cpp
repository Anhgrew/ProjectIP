#include "FileReader.h"

std::vector<Keyword*> FileReader::getListKeyWord()
{
    std::string line, des;
    getline(file, line);
    std::vector<std::string> first_line;
    tokenize(line, '-', first_line);
    //keyword_list_size = stoi(first_line[0]);
    std::vector<Keyword*> list_keyword;

    while (!file.eof()) {
        getline(file, line);
        transform(line.begin(), line.end(), line.begin(), ::toupper);
        getline(file, des);
        // transform(des.begin(), des.end(), des.begin(), ::toupper);
        list_keyword.push_back(new Keyword(line, des));
    }
    return list_keyword;
}

void FileReader::tokenize(std::string const& str, const char delim, std::vector<std::string>& out)
{
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

void FileReader::print()
{
    std::string line;
    while (!file.eof()) {
        getline(file, line);
        std::cout << line << std::endl;
    } 
}

                          
                             
                                                                