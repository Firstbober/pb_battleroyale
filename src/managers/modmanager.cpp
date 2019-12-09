#include "modmanager.hpp"
#include <filesystem>
#include <ini.h>
#include <scriptstdstring/scriptstdstring.h>
#include <spdlog/spdlog.h>
#include <string.h>

void as_message_handler(const asSMessageInfo* msg, void* param) {
	char* buf;
	sprintf(buf, "%s (%d, %d): %s", msg->section, msg->row, msg->col, msg->message);

	if(msg->type == asMSGTYPE_ERROR) {
		SPDLOG_ERROR(buf);
	} else if(msg->type == asMSGTYPE_WARNING) {
		SPDLOG_WARN(buf);
	} else if(msg->type == asMSGTYPE_INFORMATION) {
		SPDLOG_INFO(buf);
	}
}

/* To delete */
void as_print(std::string& msg) {
	printf("PBBR mod print: %s\n", msg.c_str());
}

ModManager::ModManager(std::string mods_directory) {
	this->mods_directory = mods_directory;

	this->engine = asCreateScriptEngine();
	int r = this->engine->SetMessageCallback(asFUNCTION(as_message_handler), 0, asCALL_CDECL);
	assert(r >= 0);
	RegisterStdString(engine);

	r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(as_print), asCALL_CDECL);
	assert(r >= 0);

	SPDLOG_INFO("Initialized ModManager");
}

ModManager::~ModManager() {
	for(Mod* i : this->mods) {
		delete i;
	}

	this->engine->ShutDownAndRelease();

	SPDLOG_INFO("Cleaned ModManager");
}

static int handler(void* user, const char* section, const char* name, const char* value) {
	mod_config* mconfig = (mod_config*)user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

	if(MATCH("mod", "name")) {
		mconfig->name = value;
	} else if(MATCH("mod", "description")) {
		mconfig->description = value;
	} else if(MATCH("mod", "author")) {
		mconfig->author = value;
	} else if(MATCH("version", "major")) {
		mconfig->version_major = atoi(value);
	} else if(MATCH("version", "minor")) {
		mconfig->version_minor = atoi(value);
	} else {
		return 0;
	}

	return 1;
}

std::optional<Mod*> init_mod(std::filesystem::directory_entry dir, asIScriptEngine* engine) {
	std::string mod_config_path;
	std::string main_script_path;

	for(auto&& i : std::filesystem::directory_iterator(dir)) {
		if(!i.is_directory()) {
			if(i.path().filename() == "mod.ini") {
				mod_config_path = i.path().c_str();
			}
			if(i.path().filename() == "main.as") {
				main_script_path = i.path();
			}
		}
	}

	if(mod_config_path == "") {
		SPDLOG_ERROR("mod.ini doesn't exist in {}", dir.path().parent_path().c_str());
		return {};
	}

	if(main_script_path == "") {
		SPDLOG_ERROR("main.as doesn't exist in {}", dir.path().parent_path().c_str());
		return {};
	}

	mod_config mod_config;

	if(ini_parse(mod_config_path.c_str(), handler, &mod_config) < 0) {
		SPDLOG_ERROR("Can't load {}", mod_config_path);
		return {};
	} else {
		SPDLOG_INFO(
			"Started loading \"{}\" mod ver. {}.{} by {}",
			mod_config.name,
			mod_config.version_major,
			mod_config.version_minor,
			mod_config.author);

		return new Mod(mod_config, main_script_path, engine);
	}
}

void ModManager::init_mods() {
	for(auto&& i : std::filesystem::directory_iterator(this->mods_directory)) {
		std::optional<Mod*> mod = init_mod(i, this->engine);
		if(mod) {
			this->mods.push_back(*mod);
		} else {
			SPDLOG_ERROR("Can't load mod from {}", i.path().c_str());
			break;
		}
	}
}