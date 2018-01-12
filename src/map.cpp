/*
 * map.cpp
 *
 *  Created on: Dec 10, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#include <vector>

#include "tiles.h"
#include "logger.h"
#include "map.h"
#include "ecs.h"

#include "console/iconsole.h"

Map::Map(uint16_t width, uint16_t height) :
    width(width)
  , height(height)
  , initialized(false)
  , cleaned(false)
  , tiles(nullptr)
  , visibility_map(nullptr)
{

}

Map::~Map()
{
	if(!cleaned)
		cleanUp();
}

uint16_t Map::getWidth() const
{
	return width;
}

uint16_t Map::getHeight() const
{
	return height;
}

void Map::init()
{
	Logger::Instance()->logLine("initializing map");

	tiles          = new uint16_t*  [width];
	visibility_map = new Visibility*[width];

	for(int i = 0; i < width; ++i)
	{
		tiles[i]          = new uint16_t[height];
		visibility_map[i] = new Visibility[height];
	}

	initialized = true;
}

void Map::cleanUp()
{
	Logger::Instance()->logLine("map cleanup");

	if(tiles) {
		for(int i = 0; i < width; ++i)
			delete[] tiles[i];

		delete tiles;
	}

	if(visibility_map) {
		for(int i = 0; i < width; ++i)
			delete[] visibility_map[i];

		delete visibility_map;
	}

	cleaned  = true;
}

void Map::update(ECS* ecs, float delta)
{
	if(!initialized) throw std::runtime_error("map not initialized!");

	auto p_comps = ecs->getEntitiesWithComponent(Component::PLAYER);
	if(!p_comps.empty())
	{
		uint64_t player_id = p_comps.at(0);

		auto p_comp = ecs->getComponent<Component::Position>(player_id, Component::Type::POSITION);

		// if no position is available we can do nothing
		if(!p_comp) return;

		calculateFOV(p_comp->x, p_comp->y);
	}
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

uint16_t** Map::getTiles() const
{
	if(!initialized) throw std::runtime_error("map not initialized!");

	return tiles;
}

Map::Visibility** Map::getVisibilityMap() const
{
	if(!initialized) throw std::runtime_error("map not initialized!");

	return visibility_map;
}

const int8_t multipliers[4][8] = {
    {1, 0, 0, -1, -1, 0, 0, 1},
    {0, 1, -1, 0, 0, -1, 1, 0},
    {0, 1, 1, 0, 0, -1, -1, 0},
    {1, 0, 0, 1, -1, 0, 0, -1}
};

//Source: http://www.roguebasin.com/index.php?title=C%2B%2B_shadowcasting_implementation
//Source: http://www.roguebasin.com/index.php?title=FOV_using_recursive_shadowcasting
void Map::castLight(
		uint16_t x, uint16_t y,
		uint8_t radius, uint16_t row,
        float start_slope, float end_slope,
		int16_t xx, int16_t xy,
		int16_t yx, int16_t yy)
{
    if (start_slope < end_slope)
    {
        return;
    }

    float next_start_slope = start_slope;

    // Go over each row in the octant
    for (unsigned int i = row; i <= radius; i++)
    {
        bool blocked = false;

        for (int dx = -i, dy = -i; dx <= 0; dx++)
        {
        	const float slope_factor = 0.65f;
        	// Calculate slopes
            float l_slope = (dx - slope_factor) / (dy + slope_factor);
            float r_slope = (dx + slope_factor) / (dy - slope_factor);

            if (start_slope < r_slope)
            {
                continue;
            }
            else if (end_slope > l_slope)
            {
                break;
            }

            int sax = dx * xx + dy * xy;
            int say = dx * yx + dy * yy;

            // Ensure we stay in bounds
            if (    (sax < 0 && (unsigned int)std::abs(sax) > x)
                 || (say < 0 && (unsigned int)std::abs(say) > y)
			   )
            {
                continue;
            }

            unsigned int ax = x + sax;
            unsigned int ay = y + say;

            // Ensure we stay in bounds
            if (ax >= width || ay >= height)
            {
                continue;
            }

            unsigned int radius2 = radius * radius;

            // Determine if current tile is within FOV
            if ((unsigned int)(dx * dx + dy * dy) < radius2)
            {
                visibility_map[ax][ay] = Visibility::VISIBLE;
            }

            if (blocked)
            {
                if (isOpaque(ax, ay)) //Go to next slope...
                {
                    next_start_slope = r_slope;
                    continue;
                }
                else // ...or continue
                {
                    blocked = false;
                    start_slope = next_start_slope;
                }
            }
            else if (isOpaque(ax, ay))
            {
                blocked = true;
                next_start_slope = r_slope;
                castLight(x, y, radius, i + 1, start_slope, l_slope, xx, xy, yx, yy);
            }
        }

        // We are completely blocked, stop here
        if (blocked)
        {
            break;
        }
    }
}



void Map::calculateFOV(uint16_t x, uint16_t y)
{
	if(!initialized) throw std::runtime_error("map not initialized!");

    unsigned int radius = 10;

    // Make all previous visible tiles seen
    for(uint16_t yy = 0; yy < height; ++yy)
    {
        for(uint16_t xx = 0; xx < width; ++xx)
        {
			if( visibility_map[xx][yy] == Visibility::VISIBLE
			 || visibility_map[xx][yy] == Visibility::SEEN)
			{
				visibility_map[xx][yy] = Visibility::SEEN;
			}
			else
			{
				visibility_map[xx][yy] = Visibility::INVISIBLE;
			}
        }
    }

    // Make origin tile always visible
    if(x > 0 && y > 0 && x < width && y < height)
    	visibility_map[x][y] = Visibility::VISIBLE;

    // Resolve for each octant
    for (uint8_t i = 0; i < 8; i++)
    {
		castLight( x, y, radius, 1, 1.0, 0.0, multipliers[0][i], multipliers[1][i], multipliers[2][i], multipliers[3][i]);
	}
}

bool Map::isSolid(uint16_t x, uint16_t y) const
{
	if(!initialized) throw std::runtime_error("map not initialized!");

	const Tiles::TileDefinition& def = Tiles::getDefinition(tiles[x][y]);

	return (def.flags & Tiles::Flags::BLOCKING) > 0;
}

bool Map::isOpaque(uint16_t x, uint16_t y) const
{
	if(!initialized) throw std::runtime_error("map not initialized!");

	const Tiles::TileDefinition& def = Tiles::getDefinition(tiles[x][y]);

	return (def.flags & Tiles::Flags::TRANSPARENT) == 0;
}

bool Map::isVisible(uint16_t x, uint16_t y) const
{
	if(!initialized) throw std::runtime_error("map not initialized!");

	if(x < 0 || y < 0 || x >= width || y >= height) return false;

	return visibility_map[x][y] == Visibility::VISIBLE;
}
