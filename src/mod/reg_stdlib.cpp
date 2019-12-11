#include "reg_stdlib.hpp"
#include <map/obstacle.hpp>
#include <spdlog/spdlog.h>

/* SPDLOG execution for scripts */

void as_log_info(std::string& msg) {
	SPDLOG_INFO(msg.c_str());
}

void as_log_warn(std::string& msg) {
	SPDLOG_WARN(msg.c_str());
}

void as_log_error(std::string& msg) {
	SPDLOG_ERROR(msg.c_str());
}

/* Obstacle  */

void as_obj_Obstacle_constructor(Obstacle* memory) {
	new(memory) Obstacle();
}

void as_obj_Obstacle_constructor_copy(Obstacle* memory, Obstacle& obstacle) {
	new(memory) Obstacle(obstacle);
}

void as_obj_Obstacle_constructor_full(Obstacle* memory, uint16_t id, bool collision, bool shadow) {
	new(memory) Obstacle(id, collision, shadow);
}

void as_obj_Obstacle_destructor(Obstacle* memory) {
	((Obstacle*)memory)->~Obstacle();
}


void RegisterSTDLIB(asIScriptEngine* engine) {
	int r;
	/* Register logger functions */
	r = engine->RegisterGlobalFunction("void log_info(const string msg)", asFUNCTION(as_log_info), asCALL_CDECL);
	assert(r >= 0);
	r = engine->RegisterGlobalFunction("void log_warn(const string msg)", asFUNCTION(as_log_warn), asCALL_CDECL);
	assert(r >= 0);
	r = engine->RegisterGlobalFunction("void log_error(const string msg)", asFUNCTION(as_log_error), asCALL_CDECL);
	assert(r >= 0);

	/* Register basic data types */

	/* Obstacle */
	r = engine->RegisterObjectType("Obstacle", sizeof(Obstacle), asOBJ_VALUE | asGetTypeTraits<Obstacle>());
	assert(r >= 0);
	r = engine->RegisterObjectBehaviour("Obstacle", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(as_obj_Obstacle_constructor), asCALL_CDECL_OBJLAST);
	assert(r >= 0);
	r = engine->RegisterObjectBehaviour("Obstacle", asBEHAVE_CONSTRUCT, "void f(Obstacle& in)", asFUNCTION(as_obj_Obstacle_constructor_copy), asCALL_CDECL_OBJFIRST);
	assert(r >= 0);
	r = engine->RegisterObjectBehaviour("Obstacle", asBEHAVE_CONSTRUCT, "void f(uint id, bool collision = false, bool shadow = false)", asFUNCTION(as_obj_Obstacle_constructor_full), asCALL_CDECL_OBJFIRST);
	assert(r >= 0);
	r = engine->RegisterObjectBehaviour("Obstacle", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(as_obj_Obstacle_destructor), asCALL_CDECL_OBJLAST);
	assert(r >= 0);

	r = engine->RegisterObjectMethod("Obstacle", "Obstacle &opAssign(const Obstacle& in)", asMETHOD(Obstacle, operator=), asCALL_THISCALL);
	assert(r >= 0);

	r = engine->RegisterObjectMethod("Obstacle", "uint getID()", asMETHOD(Obstacle, get_id), asCALL_THISCALL);
	assert(r >= 0);
	r = engine->RegisterObjectMethod("Obstacle", "bool getCollisionState()", asMETHOD(Obstacle, get_collision_state), asCALL_THISCALL);
	assert(r >= 0);
	r = engine->RegisterObjectMethod("Obstacle", "bool getShadowState()", asMETHOD(Obstacle, get_shadow_state), asCALL_THISCALL);
	assert(r >= 0);
}