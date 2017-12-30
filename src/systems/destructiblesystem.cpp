/*
 * destructiblesystem.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
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

void DestructibleSystem::handleMessage(SystemMessage::TMessagePtr message, ECS* ecs)
{
	auto mmessage = std::static_pointer_cast<SystemMessage::DamageMessage>(message);
	if(!mmessage) return;

	auto d_comp = ecs->getComponent<Component::Destructible> (mmessage->target, Component::Type::DESTRUCTIBLE);
	if(!d_comp) return;

	int16_t damage = mmessage->damage;

	// make sure we cant underflow
	if(damage > 0 && d_comp->cur_hp < damage) d_comp->cur_hp = 0;
	else d_comp->cur_hp -= damage;

	// we do not want to go above max health
	if(d_comp->cur_hp > d_comp->max_hp) d_comp->cur_hp = d_comp->max_hp;

	// hp = 0, destroy the target
	//TODO: Just change components or something (?)
	//Note: !!! Above could cause problems with existing targets !!!
	if(d_comp->cur_hp == 0) ecs->destroyEntity(mmessage->target);
}

/* virtual */ void DestructibleSystem::update(ECS* ecs, float delta) /* = 0 */
{
	auto messages = ecs->getMessages(type);

	for(auto message : messages)
	{
		handleMessage(message, ecs);
	}
}


