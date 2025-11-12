#pragma once
#include "fmt/color.h"
#include <iostream>
#include <fstream>
#include <source_location>
#include <fmt/core.h>
#include <sstream>
#include <vector>
#include <sys/ioctl.h>


using uint64 = uint64_t;
using uint32 = uint32_t;
using uint16 = uint16_t;
using uint8 = uint8_t;

using int64 = int64_t;
using int32 = int32_t;
using int16 = int16_t;
using int8 = int8_t;

namespace LM {

static constexpr int ERROR = -1;

class TERM {
  public:
// term utils
	static inline ssize_t columnCount() {
		struct winsize w;
		ioctl(0, TIOCGWINSZ, &w);
		return (ssize_t)w.ws_col;
	}


	static inline std::string getLineBreak(const std::string& s) {
		std::ostringstream oss;
		for (int64 i = 0; i < columnCount(); i++) {
			oss << s;
		}
		return oss.str();
	}
};

class string {
  public:
	static inline std::string toHex(const std::string& s) {
		std::ostringstream hex;
		for (unsigned char c : s) hex << ' ' << std::hex << (int)c;
		return hex.str();
	}

	static inline std::string toLiterals(const std::string& s) {
		std::ostringstream os;
		for (unsigned char c : s) {
			if (c == '\r') os << "\r";
			else if (c == '\n') os << "\n";
			else if (c == '\t') os << "\t";
			else
				os << ' ' << c;

		}
		return os.str();
	}

	/// Returns the substring within `s` that starts at `left`, whose last character is `s[right-1]`
	static inline std::string between(const std::string& s, size_t left, size_t right) {
		long int pos = left;
		long int count = right - left;
		return s.substr(pos, count);
	}

	// Trims whitespace
	static inline std::string trim(std::string& s, const char c) {
		return trim(s, &""[c]);
	}

	static inline std::string trim(std::string& s, const std::string targets = " \t\r\n") {
		for (int i = 0; i < s.size(); i++) {
			for (char target : targets) {
				if (s[i] == target)
					s.erase(i, 1);

			}
		}
		return s;
	}
	static inline std::vector<std::string> split(std::string& s, char delim) {
		std::vector<std::string> tokens{};
		size_t end = 0;
		while(end < s.size()) {
			auto start = end + 1;
			end = s.find(delim, start);
			if (end == std::string::npos) {
				end = s.size() - 1;
				auto tok = LM::string::between(s, start, end);
				tokens.push_back(tok);
				break;
			}
			tokens.push_back(LM::string::between(s, start, end));
		}
		return tokens;
	}
};

inline constexpr std::size_t NOT_FOUND = std::string::npos;


inline int get_line_number(const char* filename, const char* fn_name) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		fmt::print("Error opening file '{}'. \n", filename);
		perror("System error message");
		return LM::ERROR;
	}
	std::string line_buf;
	int line_n = 0;
	while (std::getline(file, line_buf)) {
		++line_n;
		if (line_buf.find(fn_name) != NOT_FOUND ) {
			return line_n;
		}
	}
	return LM::ERROR;
}


}
