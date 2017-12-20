/*
 * map.cpp
 *
 *  Created on: Dec 10, 2017
 *      Author: joshua
 */

#include "tiles.h"
#include "logger.h"
#include "map.h"

#include "console/iconsole.h"

Map::Map() :
	initialized(false)
{

}

void Map::init()
{
	Logger::Instance()->logLine("initializing map");

	for(int x = 0; x < width; ++ x)
	{
		for(int y = 0; y < height; ++y)
		{
			if(x == 0 || y  == 0 || x == width - 1 || y == height - 1)
			{
				tiles[x][y] = 1;
			}
			else
			{
				tiles[x][y] = 2;
			}

			visibility_map[x][y] = Visibility::VISIBLE;
		}
	}

	initialized = true;
}

void Map::update(float delta)
{
	if(!initialized) throw std::runtime_error("map not initialized!");

	calculateFOV(0, 0);
}

void Map::render()
{
	if(!initialized) throw std::runtime_error("map not initialized!");

	for(int x = 0; x < width; ++ x)
	{
		for(int y = 0; y < height; ++y)
		{
			const Tiles::TileDefinition& def = Tiles::getDefinition(tiles[x][y]);

			if(visibility_map[x][y] == Visibility::VISIBLE)
			{
				IConsole::Instance()->set_color(def.color);
				IConsole::Instance()->set_char(x, y, def.glyph);
			}
			else if(visibility_map[x][y] == Visibility::SEEN)
			{
				IConsole::Instance()->set_color(def.darker_color);
				IConsole::Instance()->set_char(x, y, def.glyph);
			}

		}
	}
}

void Map::calculateFOV(uint16_t x, uint16_t y)
{
	if(!initialized) throw std::runtime_error("map not initialized!");
}

bool Map::isVisible(uint16_t x, uint16_t y) const
{
	if(!initialized) throw std::runtime_error("map not initialized!");

	return visibility_map[x][y] == Visibility::VISIBLE;
}

bool Map::isSolid(uint16_t x, uint16_t y) const
{
	if(!initialized) throw std::runtime_error("map not initialized!");

	const Tiles::TileDefinition& def = Tiles::getDefinition(tiles[x][y]);

	return (def.flags & Tiles::Flags::BLOCKING) > 0;
}
