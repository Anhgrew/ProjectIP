#include "FileReader.h"

std::vector<Keyword*> FileReader::getListKeyWord()
{
    std::string line, des;
    getline(file, line);
    std::vector<std::string> first_line;
    tokenize(line, '-', first_line);
    keyword_list_size = stoi(first_line[0]);
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


bool FileReader::checkExistingUser(std::string name)
{
    std::string id, user_name, score_string;
    while (!file.eof()) {
        getline(file, id);
        getline(file, user_name);
        getline(file, score_string);
        transform(user_name.begin(), user_name.end(), user_name.begin(), ::tolower);
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (name.compare(user_name) == 0) {
            return true;
        }
    }
    file.close();
    return false;
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



