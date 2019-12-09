#include "mod.hpp"
#include <scriptbuilder/scriptbuilder.h>
#include <spdlog/spdlog.h>

Mod::Mod(mod_config config, std::string filename, asIScriptEngine* engine) {
	this->config = config;

	CScriptBuilder sb;
	int r = sb.StartNewModule(engine, this->config.name.c_str());
	if(r < 0) {
		SPDLOG_ERROR("({}) Unrecoverable error while starting a new module.", this->config.name.c_str());
		return;
	}

	r = sb.AddSectionFromFile(filename.c_str());
	if(r < 0) {
		SPDLOG_ERROR("({}) Please correct the errors in the script and try again.", this->config.name.c_str());
		return;
	}

	r = sb.BuildModule();
	if(r < 0) {
		SPDLOG_ERROR("({}) Please correct the errors in the script and try again.", this->config.name.c_str());
		return;
	}

	this->module = engine->GetModule(this->config.name.c_str());

	/* Delete after making new init function */

	asIScriptFunction* init = this->module->GetFunctionByDecl("void main()");
	if(init == 0) {
		SPDLOG_ERROR("({}) The script must have the function 'void main()'. Please add it and try again.", this->config.name.c_str());
		return;
	}

	/**/

	this->ctx = engine->CreateContext();
	this->ctx->Prepare(init);

	SPDLOG_INFO("Internally initialized \"{}\" mod", this->config.name.c_str());

	r = this->ctx->Execute();
	if(r != asEXECUTION_FINISHED) {
		if(r == asEXECUTION_EXCEPTION) {
			SPDLOG_ERROR("({}) An exception '{}' occurred. Please correct the code and try again.", this->config.name.c_str(), ctx->GetExceptionString());
		}
	}
}

Mod::~Mod() {
	this->ctx->Release();
	SPDLOG_INFO("Cleaned \"{}\" mod", this->config.name.c_str());
}
