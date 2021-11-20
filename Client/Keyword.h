#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include<vector>


#ifndef Keyword_h_
#define Keyword_h_

class Keyword
{
public: 
	std::string keyword;
	std::string description;
	Keyword(std::string keyword, std::string description) {
		this->keyword = keyword;
		this->description = description;
	}
};

#endif // !Keyword_h_