#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <spdlog/spdlog.h>
#include <vector>

#include <eventhandler.hpp>
#include <helpers.hpp>
#include <managers/modmanager.hpp>
#include <managers/obstaclemanager.hpp>
#include <managers/texturemanager.hpp>
#include <map/mapmanager.hpp>
#include <object.hpp>
class MainHandler : public EventHandler {
public:
	virtual void update(sf::RenderWindow* window);
	virtual void draw(sf::RenderWindow* window);
	virtual void init();
	virtual void clear();

private:
	std::vector<Object*> objects;
	MapManager map;
	ModManager mod_man = ModManager("mods");
};