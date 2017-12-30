/*
 * containersystem.cpp
 *
 *  Created on: Dec 30, 2017
 *      Author: joshua
 */


#include "systems/containersystem.h"

#include "console/iconsole.h"
#include "components.h"
#include "ecs.h"

ContainerSystem::ContainerSystem() :
    ISystem(Component::Type::CONTAINER
    		, ISystem::Type::CONTAINER)
{

}

ContainerSystem::~ContainerSystem()
{

}

/* virtual */ void ContainerSystem::update(ECS* ecs, float delta) /* = 0 */
{
	auto map = ecs->getMap();
	if(!map) return;
}

