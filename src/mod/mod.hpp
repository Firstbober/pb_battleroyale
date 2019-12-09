#pragma once
#include <angelscript.h>
#include <string>

typedef struct {
	std::string name;
	int version_major;
	int version_minor;
	std::string description;
	std::string author;
} mod_config;

class Mod {
private:
	mod_config config;
	bool is_active;

	asIScriptContext* ctx;
	asIScriptModule* module;
public:
	Mod(mod_config config, std::string filename, asIScriptEngine* engine);
	~Mod();
};