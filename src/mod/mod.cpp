#include "mod.hpp"
#include <scriptbuilder/scriptbuilder.h>
#include <spdlog/spdlog.h>

constexpr void __no_func_err(asIScriptFunction* func, const char* mod_name) {
	if(func == 0) {
		SPDLOG_ERROR("({}) The script must have the function '{}'. Please add it and try again.", mod_name, func->GetName());
		return;
	}
}

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

	asIScriptFunction* init = this->module->GetFunctionByDecl("void onInit()");
	if(init == 0) {
		SPDLOG_ERROR("({}) The script must have the function 'void onInit()'. Please add it and try again.", this->config.name.c_str());
		return;
	}

	this->as_on_update = this->module->GetFunctionByDecl("void onUpdate()");
	__no_func_err(this->as_on_update, this->config.name.c_str());
	this->as_on_draw = this->module->GetFunctionByDecl("void onDraw()");
	__no_func_err(this->as_on_update, this->config.name.c_str());

	this->ctx = engine->CreateContext();
	this->ctx->Prepare(init);

	SPDLOG_INFO("Internally initialized \"{}\" mod", this->config.name.c_str());

	r = this->ctx->Execute();
	if(r != asEXECUTION_FINISHED) {
		if(r == asEXECUTION_EXCEPTION) {
			SPDLOG_ERROR("({}) An exception '{}' occurred. Please correct the code and try again.", this->config.name.c_str(), this->ctx->GetExceptionString());
			return;
		}
		return;
	}

	this->active = true;
}

Mod::~Mod() {
	this->ctx->Release();
	SPDLOG_INFO("Cleaned \"{}\" mod", this->config.name.c_str());
}


bool Mod::is_active() {
	return this->active;
}

inline bool __func_run(asIScriptContext* ctx, asIScriptFunction* func, const char* mod_name) {
	ctx->Prepare(func);
	int r = ctx->Execute();

	if(r != asEXECUTION_FINISHED) {
		if(r == asEXECUTION_EXCEPTION) {
			SPDLOG_ERROR("({}) An exception '{}' occurred. Please correct the code and try again.", mod_name, ctx->GetExceptionString());
			return false;
		}
		return false;
	}

	return true;
}

bool Mod::update() {
	return __func_run(this->ctx, this->as_on_update, this->config.name.c_str());
}

bool Mod::draw() {
	return __func_run(this->ctx, this->as_on_draw, this->config.name.c_str());
}