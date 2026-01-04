#pragma once

// meyers singleton - deleted ctors, copy ctor, and copy assignment operator
// returns a static instance,
#include <cstddef>

extern size_t N_PASSED_TESTS;
extern size_t N_FAILED_TESTS;
extern size_t N_PASSED_GROUPS;
extern size_t N_FAILED_GROUPS;
class CFG {
  public:
	bool testgroup_verbose_default = false;
	static CFG &get() {
		static CFG cfg;
		return cfg;
	};

  private:
	CFG() = default;
	CFG(const CFG &) = delete;
	CFG &operator=(const CFG &) = delete;
};

