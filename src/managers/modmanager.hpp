#pragma once
#include <vector>
#include <string>
#include <mod/mod.hpp>

class ModManager {
private:
	std::string mods_directory;
	std::vector<Mod> mods;
public:
	ModManager(std::string mods_directory);
	~ModManager();

	void init_mods();
};