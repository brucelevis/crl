/*
 * aisystem.cpp
 *
 *  Created on: Dec 20, 2017
 *      Author: joshua
 */

#include <random>
#include "systems/aisystem.h"

#include "components.h"
#include "ecs.h"

AISystem::AISystem() :
    ISystem(Component::Type::AI
    		, ISystem::Type::AI)
{

}

AISystem::~AISystem()
{

}

/* virtual */ void AISystem::update(ECS* ecs, float delta) /* = 0 */
{
	auto map = ecs->getMap();
	if(!map) return;

	for(uint64_t entity : interested_entities)
	{
		auto ai_comp = ecs->getComponent<Component::AIComponent>(entity, Component::Type::AI);

		if(!ai_comp) continue;

		switch(ai_comp->ai_type)
		{
		case Component::AIComponent::DUMB:
			runDumbAI(ecs, entity, delta);
			break;
		case Component::AIComponent::ANIMAL:
			runAnimalAI(ecs, entity, delta);
			break;
		case Component::AIComponent::INTELLIGENT:
			runIntelligentAI(ecs, entity, delta);
			break;
		case Component::AIComponent::OMNISCIENT:
			runOmniscientAI(ecs, entity, delta);
			break;
		case Component::AIComponent::PLAYER:
			runPlayerAI(ecs, entity, delta);
			break;
		case Component::AIComponent::NONE:
			break;
		}

	}
}

void AISystem::runDumbAI(ECS* ecs, uint64_t entity, float delta)
{
	auto t_comp = ecs->getComponent<Component::Target>(entity, Component::Type::TARGET);

	if(t_comp && ecs->entityExists(t_comp->target))
	{

	}
	else
	{
	    // Seed with a real random value, if available
	    std::random_device r;

	    // Choose a random mean between 1 and 6
	    std::default_random_engine e1(r());
	    std::uniform_int_distribution<int> uniform_dist(-1, 1);

	    // move randomly
		ecs->sendSystemMessage(ISystem::Type::MOVEMENT,
				SystemMessage::TMessagePtr(new SystemMessage::MovementMessage(entity, uniform_dist(e1), uniform_dist(e1))));
	}
}

void AISystem::runAnimalAI(ECS* ecs, uint64_t entity, float delta)
{

}

void AISystem::runIntelligentAI(ECS* ecs, uint64_t entity, float delta)
{

}

void AISystem::runOmniscientAI(ECS* ecs, uint64_t entity, float delta)
{

}

void AISystem::runPlayerAI(ECS* ecs, uint64_t entity, float delta)
{

}


