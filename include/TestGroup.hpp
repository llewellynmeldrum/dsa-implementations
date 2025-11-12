#pragma once
#include <source_location>
#include <iostream>
#include <sstream>
#include <strstream>
#include <source_location>
#include <vector>
#include <string_view>
#include <type_traits>
#include <string.h>
#include <log.hpp>

#include "ansi_colors.hpp"
#include "LMUtils.hpp"

struct G {
	static size_t TOTAL_PASS_COUNT;
	static size_t TOTAL_FAIL_COUNT;
	static size_t SUCCESSFUL_GROUPS;
	static size_t FAILING_GROUPS;
};


template<typename T>
struct TestGroup {
	using srcloc = std::source_location;
	std::string suite_name;
	bool verbose = false;
	size_t pass_count = 0;
	size_t fail_count = 0;
	std::vector<std::string> passed_test_names;
	std::vector<std::string> failed_test_names;

	TestGroup(std::string name, bool verbose = false) : suite_name(name), verbose(verbose) {
		std::cerr << LM::TERM::getLineBreak("-");

		std::cerr << ansi::bold << ansi::inverse
		          << "TEST GROUP:" << ansi::reset << " "
		          << ansi::underline << name << ansi::reset;

		std::cerr << ansi::bold;
		if (verbose) std::cerr << " (VERBOSE)\n";
		else std::cerr << "\n";

		std::cerr << ansi::reset;
	};


	void log_success(std::string name, T expects, T result) {
		std::cerr        << ansi::green <<  "\nTEST '" << ansi::reset << name
		                 << ansi::green << "' PASSED:\n" << ansi::reset;
		std::cerr << ansi::bold << ansi::green << "\tEXPECTED --> " << ansi::reset << val_to_str(expects) << std::endl ;
		std::cerr << ansi::bold << ansi::green << "\t     GOT --> " << ansi::reset << val_to_str(result) << std::endl ;
	}

	void log_failure(std::string name, T expects, T result, srcloc err) {
		int rnu = err.line() - LM::get_line_number(err.file_name(), err.function_name());
		std::cerr << ansi::reset
		          << ansi::red <<  "\nTEST " << ansi::reset
		          << name
		          << ansi::red << " FAILED: "
		          << ansi::underline << err.file_name() << ':' << err.line() << std::endl
		          << ansi::reset << ansi::red << "IN FUNCTION: "
		          << ansi::underline  << err.function_name() << ":" << rnu << std::endl
		          << ansi::reset;

		std::cerr << ansi::bold << ansi::red << "\tEXPECTED --> " << ansi::reset << val_to_str(expects) << std::endl ;
		std::cerr << ansi::bold << ansi::red << "\t     GOT --> " << ansi::reset << val_to_str(result) << std::endl ;
		if constexpr (std::is_same_v<std::string, T>) {
			std::cerr << "\t             " << markDiff(expects, result) << std::endl << ansi::reset ;
		}


	}

	std::string markDiff(std::string a, std::string b) {
		std::cerr << ansi::red;
		auto& longer = (a.size() >= b.size() ? a : b);
		char marker = '^';
		std::string markers{};
		markers.resize(longer.size());
		for (int i = 0; i < markers.size(); i++) {
			if (i < a.size() && i < b.size()) {
				markers[i] = (a[i] == b[i] ? ' ' : marker);
			} else {
				markers[i] = marker;
			}
		}
		return markers;
	}

	std::string val_to_str(const T val) const {
		std::ostringstream oss;
		if constexpr(std::is_same_v<T, bool>) {
			oss << (val == true ? "TRUE" : "FALSE");
		} else {
			oss << val;
		}
		return oss.str();
	}


	~TestGroup() {
		std::cerr << summary_msg() << std::endl;
		std::cerr << ansi::reset;
		G::SUCCESSFUL_GROUPS 	+= (fail_count == 0);
		G::FAILING_GROUPS 	+= (fail_count > 0);
	}

	inline std::string summary_msg() {
		std::ostringstream oss;
		oss << ansi::bold;
		if (fail_count == 0) {
			oss << ansi::green << suite_name << " --> "
			    << "all " << pass_count << " test(s) succeeded:\n" << ansi::reset;
			oss << ansi::green;
			for (std::string name : passed_test_names) {
				oss << name << ", ";
			}
			oss << std::endl;
		} else {
			oss << ansi::red
			    << pass_count << "/" << pass_count + fail_count << " test(s) passed:\n" << ansi::reset;
			oss << ansi::green;
			for (std::string name : passed_test_names) {
				oss << name << ", ";
			}

			oss << ansi::red;
			for (std::string name : failed_test_names) {
				oss << name << ", ";
			}
			oss << std::endl;
		}
		return oss.str();
	}


#define run_test(name, expected, result) /* wrapper macro to grab source_location */\
	_RUN_TEST(name, expected, result, std::source_location::current())

	inline bool _RUN_TEST(std::string name, T expects, T result, std::source_location loc) {
		// getting strange results where strings arent equal but cstrings are.

		if (expects != result) {
			failed_test_names.push_back(name);
			log_failure(name, expects, result, loc);
			++fail_count;
			++G::TOTAL_FAIL_COUNT;
			return false;
		} else {
			if (verbose) log_success(name, expects, result);
			passed_test_names.push_back(name);
		}

		++pass_count;
		++G::TOTAL_PASS_COUNT;
		return true;
	}

};
