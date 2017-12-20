/*
 * movementsystem.cpp
 *
 *  Created on: Dec 20, 2017
 *      Author: joshua
 */


#include "systems/movementsystem.h"

#include "console/iconsole.h"
#include "components.h"
#include "ecs.h"

MovementSystem::MovementSystem() :
    ISystem(Component::Type::POSITION | Component::Type::MOVEMENT
    		, ISystem::Type::MOVEMENT)
{

}

MovementSystem::~MovementSystem()
{

}

void MovementSystem::handleMessage(SystemMessage::TMessagePtr message, ECS* ecs)
{
	auto map = ecs->getMap();
	if(!map) return;

	auto mmessage = std::static_pointer_cast<SystemMessage::MovementMessage>(message);
	if(!mmessage) return;

	auto p_comp = ecs->getComponent<Component::Position>(mmessage->entity, Component::Type::POSITION);
	if(!p_comp) return;


	uint16_t new_x = p_comp->x + mmessage->delta_x;
	uint16_t new_y = p_comp->y + mmessage->delta_y;

	// if the new x > original x but the movement was -1 it means we overflowed...
	if(new_x > p_comp->x && mmessage->delta_x < 0) return;
	// ...same for y
	if(new_y > p_comp->y && mmessage->delta_y < 0) return;

	// we cannot move into solid spaces
	if(map->isSolid(new_x, new_y)) return;

	// we passed all checks so we can move!
	p_comp->x = new_x;
	p_comp->y = new_y;
}

/* virtual */ void MovementSystem::update(ECS* ecs, float delta) /* = 0 */
{
	auto messages = ecs->getMessages(type);

	for(auto message : messages)
	{
		handleMessage(message, ecs);
	}
}
