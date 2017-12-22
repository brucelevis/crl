/*
 * attacksystem.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: joshua
 */

#include "systems/attacksystem.h"

#include "components.h"
#include "ecs.h"

AttackSystem::AttackSystem() :
    ISystem(Component::Type::ATTACK
    		, ISystem::Type::ATTACK)
{

}

AttackSystem::~AttackSystem()
{

}

void AttackSystem::handleMessage(SystemMessage::TMessagePtr message, ECS* ecs)
{
	auto mmessage = std::static_pointer_cast<SystemMessage::MovementMessage>(message);
	if(!mmessage) return;

	auto p_comp = ecs->getComponent<Component::Position>(mmessage->entity, Component::Type::POSITION);
	if(!p_comp) return;
}

/*
 * 	auto map = ecs->getMap();
	if(!map) return;


	for(uint64_t entity : interested_entities)
	{
		auto a_comp = ecs->getComponent<Component::Attack>(entity, Component::Type::ATTACK);

		if(!a_comp) continue;
	}
 */

/* virtual */ void AttackSystem::update(ECS* ecs, float delta) /* = 0 */
{
	auto messages = ecs->getMessages(type);

	for(auto message : messages)
	{
		handleMessage(message, ecs);
	}
}



