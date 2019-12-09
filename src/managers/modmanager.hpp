#pragma once
#include <vector>
#include <string>
#include <angelscript.h>
#include <mod/mod.hpp>

class ModManager {
private:
	std::string mods_directory;
	std::vector<Mod*> mods;

	asIScriptEngine* engine;
public:
	ModManager(std::string mods_directory);
	~ModManager();

	void init_mods();
};