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
	bool active = false;

	asIScriptContext* ctx;
	asIScriptModule* module;

	asIScriptFunction* as_on_update;
	asIScriptFunction* as_on_draw;
public:
	Mod(mod_config config, std::string filename, asIScriptEngine* engine);
	~Mod();

	bool is_active();

	bool update();
	bool draw();
};