/*
 * map.cpp
 *
 *  Created on: Dec 10, 2017
 *      Author: Joshua Lettink
 */

#include <vector>

#include "tiles.h"
#include "logger.h"
#include "map.h"
#include "ecs.h"

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
			else if(x % 4 == 0 && y % 4 == 0)
			{
				tiles[x][y] = 1;
			}
			else
			{
				tiles[x][y] = 2;
			}

			visibility_map[x][y] = Visibility::INVISIBLE;
		}
	}

	initialized = true;
}

void Map::update(ECS* ecs, float delta)
{
	if(!initialized) throw std::runtime_error("map not initialized!");

	uint64_t player_id = ecs->getPlayerId();
	if(player_id != 0)
	{
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
    for (uint i = row; i <= radius; i++)
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
            if (    (sax < 0 && (uint)std::abs(sax) > x)
                 || (say < 0 && (uint)std::abs(say) > y)
			   )
            {
                continue;
            }

            uint ax = x + sax;
            uint ay = y + say;

            // Ensure we stay in bounds
            if (ax >= width || ay >= height)
            {
                continue;
            }

            uint radius2 = radius * radius;

            // Determine if current tile is within FOV
            if ((uint)(dx * dx + dy * dy) < radius2)
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

    uint radius = 10;

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
