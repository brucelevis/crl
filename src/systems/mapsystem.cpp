/*
 * mapsystem.cpp
 *
 *  Created on: Dec 20, 2017
 *      Author: joshua
 */


#include "systems/mapsystem.h"

#include "ecs.h"
#include "map.h"

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

	if(map_ptr)
	{
		map_ptr->update(ecs, delta);
		map_ptr->render();
	}
}
