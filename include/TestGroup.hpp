#pragma once
#include <cassert>
#include <source_location>
#include <iostream>
#include <sstream>
#include <strstream>
#include <source_location>
#include <vector>
#include <string_view>
#include <type_traits>
#include <string.h>

#include "config.hpp"
#include "ansi_colors.hpp"
#include "LMUtils.hpp"


struct TestGroupConfig {
	const bool noHeader = false;
	const bool verbose = true;
};


template<typename T>
class TestGroup {
  public:
	using srcloc = std::source_location;
	std::string suite_name;
	TestGroupConfig cfg;
	size_t pass_count = 0;
	size_t fail_count = 0;
	std::vector<std::string> passed_test_names;
	std::vector<std::string> failed_test_names;

	TestGroup(std::string name, TestGroupConfig cfg = {}) : suite_name(name), cfg(cfg) {
		if (!cfg.noHeader) {
			/*if (cfg.verbose) */std::cerr << LM::TERM::getLineBreak("-");

			std::cerr << ansi::bold << ansi::inverse
			          << "TEST GROUP:" << ansi::reset << " "
			          << ansi::underline << name << ansi::reset;

			std::cerr << ansi::bold;
			if (cfg.verbose) std::cerr << " (VERBOSE)\n";
			else std::cerr << "\n";
		}

		std::cerr << ansi::reset;
	};
	~TestGroup() {
		std::cerr << summary_msg() << std::endl;
		std::cerr << ansi::reset;
		N_PASSED_GROUPS 	+= (fail_count == 0);
		N_FAILED_GROUPS 	+= (fail_count > 0);
	}

	inline std::string summary_msg() {
		std::ostringstream oss;
		oss << ansi::bold;
		if (fail_count == 0) {
			oss << ansi::fg_green << suite_name << " --> "
			    << "all " << pass_count << " test(s) succeeded:\n" << ansi::reset;
			oss << ansi::fg_green;
			for (std::string name : passed_test_names) {
				oss << name << ", ";
			}
			oss << std::endl;
		} else {
			oss << ansi::fg_red
			    << pass_count << "/" << pass_count + fail_count << " test(s) passed:\n" << ansi::reset;
			oss << ansi::fg_green;
			for (std::string name : passed_test_names) {
				oss << name << ", ";
			}

			oss << ansi::fg_red;
			for (std::string name : failed_test_names) {
				oss << name << ", ";
			}
			oss << std::endl;
		}
		return oss.str();
	}
#define run(name, expected, result) /* wrapper macro to grab source_location::current() */\
	_RUN_TEST(name, expected, result, std::source_location::current())

	inline bool _RUN_TEST(std::string name, T expects, T result, std::source_location loc) {
		// getting strange results where strings arent equal but cstrings are.

		if (expects != result) {
			failed_test_names.push_back(name);
			log_failure(name, expects, result, loc);
			++fail_count;
			++N_FAILED_TESTS;
			return false;
		} else {
			if (cfg.verbose) log_success(name, expects, result);
			passed_test_names.push_back(name);
		}

		++pass_count;
		++N_PASSED_TESTS;
		return true;
	}


  private:

	void log_success(std::string name, T expects, T result) {
		std::string bold_green = ansi::bold;
		bold_green += ansi::fg_green;
		LM::cerr(
		    ansi::fg_green, "\nTEST '",
		    ansi::reset, name,
		    ansi::fg_green, "' PASSED:", ansi::reset
		);

		LM::cerr(
		    bold_green, "    EXPECTED --> ", ansi::reset, val_to_str(expects), "\n",
		    bold_green, "         GOT --> ", ansi::reset, val_to_str(result), "\n"
		);
	}

	void log_failure(std::string name, T expects, T result, srcloc err) {
		int rnu = err.line() - LM::get_line_number(err.file_name(), err.function_name());
		std::string bold_red = ansi::bold;
		bold_red += ansi::fg_red;

		LM::cerr(
		    ansi::fg_red,  "\nTEST ", ansi::reset, name, ansi::fg_red, " FAILED: ",
		    ansi::underline, err.file_name(), ':', err.line(), "\n",
		    ansi::reset
		);

		LM::cerr(
		    ansi::fg_red, "IN FUNCTION: ", ansi::underline, err.function_name(), ":", rnu, "\n",
		    ansi::reset
		);

		LM::cerr(
		    bold_red, "\tEXPECTED --> ", ansi::reset, val_to_str(expects), "\n"
		);

		LM::cerr(
		    bold_red,  "\t     GOT --> ", ansi::reset, val_to_str(result), "\n"
		);
		if constexpr (std::is_same_v<std::string, T>) {
			std::cerr << "\t              " << differenceLine(expects, result) << std::endl << ansi::reset ;
		}
	}

	std::string differenceLine(std::string expected, std::string result) {

		constexpr const char POISON_CH = 0x7F;
		constexpr const char MARK = '^';
		constexpr const char *WIDE_MARK = "/\\";
		if (expected.size() != result.size()) {
			/*
			 if two strings are of different size, then necessarily, all characters
			 beyond the limit of the shorter string will not match.
			 Thus, we append the shorter string with DEL bytes to pad its size, which will never
			 match a real string.
			*/
			auto& longer_str = (expected.size() >= result.size() ? expected : result);
			auto& shorter_str = (expected.size() <= result.size() ? expected : result);
			size_t diff = longer_str.size() - shorter_str.size();
			shorter_str.append(diff, POISON_CH);
		}

		assert(expected.size() == result.size());
		std::ostringstream marker_line;
		marker_line << ansi::fg_red;
		for (int i = 0; i < expected.size(); i++) {
			if (expected[i] == result[i]) marker_line << ' ';
			else {
				if (expected[i] == '\r' || expected[i] == '\n' || expected[i] == '\t') {
					// handle special cases which are drawn as 2 chars. See LM::string::toLiterals
					marker_line << WIDE_MARK;
				} else {
					marker_line << MARK;
				}
			}
		}
		return marker_line.str();
	}

	std::string val_to_str(const T val) const {
		// handle some special cases
		std::string prefix;
		std::string suffix;
		std::ostringstream oss;
		if constexpr(std::is_same_v<T, bool>) {
			oss << (val == true ? "TRUE" : "FALSE");
		} else if constexpr (std::is_same_v<T, std::string>) {
			prefix = "'";
			suffix = "'";

			std::ostringstream val_str;
			val_str << val;
			oss << LM::string::toLiterals(val_str.str());
		} else {
			oss << val;
		}

		suffix += ansi::reset;
		return prefix +  oss.str() + suffix;
	}






};
