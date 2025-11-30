#pragma once

class CFG {
  public:
	bool testgroup_verbose_default = false;
	static CFG &instance() {
		static CFG cfg;
		return cfg;
	};

  private:
	CFG() = default;
	CFG(const CFG &) = delete;
	CFG &operator=(const CFG &) = delete;
};

