#include "mod.hpp"
#include <spdlog/spdlog.h>


Mod::Mod(mod_config config, std::string filename) {
	this->config = config;

	SPDLOG_INFO("Internally initialized \"{}\" mod", config.name.c_str());
}

Mod::~Mod() {
}
