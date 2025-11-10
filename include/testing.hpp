#pragma once
#include <source_location>
#include "ansi_colors.hpp"
#include "common.hpp"
#include <iostream>
#include <sstream>
#include <strstream>
#include <source_location>
#include <string_view>
#include <type_traits>
struct G {
	static size_t TOTAL_FAIL_COUNT;
	static size_t TOTAL_PASS_COUNT;
};


template<typename T>
struct TestGroup {
	using srcloc = std::source_location;
	string suite_name;
	bool verbose = false;
	size_t pass_count = 0;
	size_t fail_count = 0;

	TestGroup(std::string name, bool verbose = false) : suite_name(name), verbose(verbose) {
		std::cerr << ansi::bold
		          << "TESTING --> '" << ansi::reset
		          << ansi::underline << name << ansi::reset;

		std::cerr << ansi::bold;
		if (verbose) std::cerr << "'\nIN VERBOSE MODE... \n";
		else std::cerr << "'...\n";

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

	}

	string val_to_str(const T& val) const {
		ostringstream oss;
		if constexpr(std::is_same_v<T, bool>) {
			oss << (val ? "TRUE" : "FALSE");
		} else {

			oss << val;
		}
		return oss.str();
	}


	~TestGroup() {
		std::cerr << summary_msg() << std::endl;
	}

	inline std::string summary_msg() {
		std::ostringstream oss;
		oss << ansi::bold;
		if (fail_count == 0) {
			oss << ansi::green << suite_name << " --> "
			    << "all " << pass_count << " test(s) succeeded.\n" << ansi::reset;
		} else {
			oss << ansi::red
			    << pass_count << "/" << pass_count + fail_count << " test(s) passed.\n" << ansi::reset;
		}
		return oss.str();
	}


#define run_test(name, expected, result) /* wrapper macro to grab source_location */\
	_RUN_TEST(name, expected, result, std::source_location::current())

	inline bool _RUN_TEST(std::string name, T expects, T result, std::source_location loc) {
		if (result != expects) {
			log_failure(name, expects, result, loc);
			++fail_count;
			++G::TOTAL_FAIL_COUNT;
			return false;
		}
		if (verbose) log_success(name, expects, result);

		++pass_count;
		++G::TOTAL_PASS_COUNT;
		return true;
	}

};
