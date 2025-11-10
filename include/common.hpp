#pragma once
#include <iostream>
#include <fstream>
using std::cout;
using std::ostringstream;
using std::string;
using std::vector;
namespace LM {

inline int get_line_number(const char* filename, const char* fn_name) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "error opening file '" << filename << "'. \n";
		return -1;
	}
	std::string line;
	int line_num = 0;
	while (std::getline(file, line)) {
		++line_num;
		if (line.find(fn_name) != std::string::npos) {
			return line_num;
		}
	}
	return -1;
}
}
