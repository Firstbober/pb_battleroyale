#include "reg_stdlib.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <map/location.hpp>
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

/* Obstacle */

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

sf::IntRect& as_obj_Obstacle_func_get_rect(Obstacle* ob) {
	Obstacle o = *ob;
	const sf::IntRect& i = o.get_rect();
	return (sf::IntRect&)i;
}

/* sf::IntRect */

void as_obj_sfIntRect_constructor(sf::IntRect* memory) {
	new(memory) sf::Rect<int>();
}

void as_obj_sfIntRect_constructor_copy(sf::IntRect* memory, sf::IntRect& rect) {
	new(memory) sf::IntRect(rect);
}

void as_obj_sfIntRect_constructor_coords(
	sf::IntRect* memory,
	int rect_left,
	int rect_top,
	int rect_width,
	int rect_height) {

	new(memory) sf::Rect<int>(rect_left, rect_top, rect_width, rect_height);
}

void as_obj_sfIntRect_destructor(sf::IntRect* memory) {
	((sf::IntRect*)memory)->~Rect<int>();
}

sf::IntRect& as_obj_sfIntRect_op_assign(sf::IntRect* rect, const sf::IntRect& to_set) {
	rect->height = to_set.height;
	rect->width = to_set.width;
	rect->top = to_set.top;
	rect->left = to_set.left;
	return *rect;
}

/* sf::Texture */

void as_obj_sfTexture_constructor(sf::Texture* memory) {
	new(memory) sf::Texture();
}

void as_obj_sfTexture_destructor(sf::Texture* memory) {
	((sf::Texture*)memory)->~Texture();
}

sf::Texture& as_obj_sfTexture_op_assign(sf::Texture* tx, const sf::Texture& to_set) {
	sf::Texture t = to_set;
	tx = &t;

	return *tx;
}

/* Location */

class LocWrap {
private:
	/* data */
public:
	Location* loc;
	LocWrap(/* args */);
	LocWrap(sf::Texture& background, sf::Texture& tileset);
	LocWrap(sf::Texture& background, sf::Texture& tileset, const char* fname);
	~LocWrap();
};

LocWrap::LocWrap(/* args */) {
	loc = new Location();
}

LocWrap::LocWrap(sf::Texture& background, sf::Texture& tileset) {
	loc = new Location(background, tileset);
}

LocWrap::LocWrap(sf::Texture& background, sf::Texture& tileset, const char* fname) {
	loc = new Location(background, tileset, fname);
}

LocWrap::~LocWrap() {
	delete loc;
}

void as_obj_Location_constructor(void* memory) {
	new(memory) LocWrap();
}

void as_obj_Location_constructor_bgts(LocWrap* memory, sf::Texture& background, sf::Texture& tileset) {
	new(memory) LocWrap(background, tileset);
}

void as_obj_Location_constructor_bgtsfn(LocWrap* memory, sf::Texture& background, sf::Texture& tileset, std::string& str) {
	new(memory) LocWrap(background, tileset, str.c_str());
}

void as_obj_Location_destructor(void* memory) {
	((LocWrap*)memory)->~LocWrap();
}

LocWrap& as_obj_Location_op_assign(LocWrap* loc, const LocWrap& to_set) {
	LocWrap l = to_set;
	loc = &l;

	return *loc;
}

void as_obj_Location_func_set_obstacle(LocWrap* loc) {
	loc->loc->set_obstacle(6, 6, new Obstacle(2));
	//Location* loc = (Location*)gen->GetObject();
	//SPDLOG_INFO(" ORIGN X: {}", loc->getOrigin().x);
	//loc->load("map.bin");
	//loc->set_obstacle(x, y, &ob);

	//Obstacle* o = &ob;
	//SPDLOG_WARN(" X: {}", x);
	//SPDLOG_WARN(" Y: {}", y);

	//loc->loc->set_obstacle(x, y, new Obstacle(2));
	//SPDLOG_WARN(" ID: {}", ob.get_id());
	//((Location*)l1)->set_obstacle(6, 6, new Obstacle(2));

	//loc->set_obstacle(x, y, new Obstacle(3));
	/*
	try
	{
		Location* l = reinterpret_cast<Location*>(loc);
		l->set_obstacle(6, 6, new Obstacle(6));
	}
	catch(const std::exception& e)
	{
		SPDLOG_ERROR("{}", e.what());
	}
	*/
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

	r = engine->RegisterObjectType("Location", sizeof(LocWrap), asOBJ_VALUE | asGetTypeTraits<LocWrap>());
	assert(r >= 0);
	r = engine->RegisterObjectBehaviour("Location", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(as_obj_Location_constructor), asCALL_CDECL_OBJLAST);
	assert(r >= 0);
	/*
	r = engine->RegisterObjectBehaviour(
		"Location",
		asBEHAVE_CONSTRUCT,
		"void f(sf::Texture& in, sf::Texture& in)",
		asFUNCTION(as_obj_Location_constructor_bgts),
		asCALL_CDECL_OBJFIRST);
	assert(r >= 0);
	r = engine->RegisterObjectBehaviour(
		"Location",
		asBEHAVE_CONSTRUCT,
		"void f(sf::Texture& in, sf::Texture& in, const string filename)",
		asFUNCTION(as_obj_Location_constructor_bgtsfn),
		asCALL_CDECL_OBJFIRST);
	assert(r >= 0);
	*/
	r = engine->RegisterObjectBehaviour("Location", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(as_obj_Location_destructor), asCALL_CDECL_OBJLAST);
	assert(r >= 0);

	r = engine->RegisterObjectMethod("Location", "Location &opAssign(const Location& in)", asFUNCTION(as_obj_Location_op_assign), asCALL_CDECL_OBJLAST);
	assert(r >= 0);

	r = engine->RegisterObjectMethod("Location", "void aqa_dbstacle()", asFUNCTION(as_obj_Location_func_set_obstacle), asCALL_CDECL_OBJLAST);
	assert(r >= 0);

	/* Register basic data types */

	// SFML
	r = engine->SetDefaultNamespace("sf");
	assert(r >= 0);

	// sf::IntRect
	r = engine->RegisterObjectType("IntRect", sizeof(sf::IntRect), asOBJ_VALUE | asGetTypeTraits<sf::IntRect>());
	assert(r >= 0);
	r = engine->RegisterObjectBehaviour("IntRect", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(as_obj_sfIntRect_constructor), asCALL_CDECL_OBJLAST);
	assert(r >= 0);
	r = engine->RegisterObjectBehaviour("IntRect", asBEHAVE_CONSTRUCT, "void f(IntRect& in)", asFUNCTION(as_obj_sfIntRect_constructor_copy), asCALL_CDECL_OBJFIRST);
	assert(r >= 0);
	r = engine->RegisterObjectBehaviour("IntRect", asBEHAVE_CONSTRUCT, "void f(int rect_left, int rect_top, int rect_width, int rect_height)", asFUNCTION(as_obj_sfIntRect_constructor_coords), asCALL_CDECL_OBJFIRST);
	assert(r >= 0);
	r = engine->RegisterObjectBehaviour("IntRect", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(as_obj_sfIntRect_destructor), asCALL_CDECL_OBJLAST);
	assert(r >= 0);

	r = engine->RegisterObjectMethod("IntRect", "IntRect &opAssign(const IntRect& in)", asFUNCTION(as_obj_sfIntRect_op_assign), asCALL_CDECL_OBJLAST);
	assert(r >= 0);

	// sf::Texture
	r = engine->RegisterObjectType("Texture", sizeof(sf::Texture), asOBJ_VALUE | asGetTypeTraits<sf::Texture>());
	assert(r >= 0);
	r = engine->RegisterObjectBehaviour("Texture", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(as_obj_sfTexture_constructor), asCALL_CDECL_OBJLAST);
	assert(r >= 0);
	r = engine->RegisterObjectBehaviour("Texture", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(as_obj_sfTexture_destructor), asCALL_CDECL_OBJLAST);
	assert(r >= 0);

	r = engine->RegisterObjectMethod("Texture", "Texture &opAssign(const Texture& in)", asFUNCTION(as_obj_sfTexture_op_assign), asCALL_CDECL_OBJLAST);
	assert(r >= 0);

	r = engine->SetDefaultNamespace("");
	assert(r >= 0);
	// SFML

	// Obstacle
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

	r = engine->RegisterObjectMethod("Obstacle", "void setRect(sf::IntRect& in)", asMETHOD(Obstacle, set_rect), asCALL_THISCALL);
	assert(r >= 0);
	r = engine->RegisterObjectMethod("Obstacle", "sf::IntRect& getRect()", asFUNCTION(as_obj_Obstacle_func_get_rect), asCALL_CDECL_OBJLAST);
	assert(r >= 0);

	//Location
	
}