/*
 * mapsystem.cpp
 *
 *  Created on: Dec 20, 2017
 *      Author: Joshua Lettink
 */


#include "systems/mapsystem.h"

#include "ecs.h"
#include "map.h"
#include "tiles.h"

MapSystem::MapSystem() :
    ISystem(Component::Type::NONE, ISystem::Type::MAP)
{

}

MapSystem::~MapSystem()
{

}

/* virtual */ void MapSystem::update(ECS* ecs, float delta) /* = 0 */
{
	auto map_ptr = ecs->getMap();
	if(!map_ptr) return;

	uint64_t active_camera = ecs->getActiveCamera();
	if(!ecs->entityExists(active_camera)) return;

	//get camera stuff
	auto c_p_comp = ecs->getComponent<Component::Position>(active_camera, Component::Type::POSITION);
	auto c_c_comp = ecs->getComponent<Component::Camera>  (active_camera, Component::Type::CAMERA);

	if(!c_p_comp || !c_c_comp) return;

	map_ptr->update(ecs, delta);
	//map_ptr->render();

	uint16_t** tiles                 = map_ptr->getTiles();
	Map::Visibility** visibility_map = map_ptr->getVisibilityMap();

	for(int x = c_c_comp->x_offset; x < c_c_comp->x_offset + c_c_comp->view_width; ++ x)
	{
		for(int y = c_c_comp->y_offset; y < c_c_comp->y_offset + c_c_comp->view_height; ++y)
		{
			if(x >= map_ptr->getWidth() || y >= map_ptr->getHeight()) continue;

			const Tiles::TileDefinition& def = Tiles::getDefinition(tiles[x][y]);

			if(visibility_map[x][y] == Map::Visibility::VISIBLE)
			{
				IConsole::Instance()->set_color(def.color);
				IConsole::Instance()->set_char(x - c_c_comp->x_offset, y - c_c_comp->y_offset, def.glyph);
			}
			else if(visibility_map[x][y] == Map::Visibility::SEEN)
			{
				IConsole::Instance()->set_color(def.darker_color);
				IConsole::Instance()->set_char(x - c_c_comp->x_offset, y - c_c_comp->y_offset, def.glyph);
			}

		}
	}
}
