/*
 * destructiblesystem.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: joshua
 */

#include "systems/destructiblesystem.h"

#include "components.h"
#include "ecs.h"

DestructibleSystem::DestructibleSystem() :
    ISystem(Component::Type::DESTRUCTIBLE
    		, ISystem::Type::DESTRUCTIBLE)
{

}

DestructibleSystem::~DestructibleSystem()
{

}

/* virtual */ void DestructibleSystem::update(ECS* ecs, float delta) /* = 0 */
{
	auto map = ecs->getMap();
	if(!map) return;


	for(uint64_t entity : interested_entities)
	{

	}
}


