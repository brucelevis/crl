/*
 * aisystem.cpp
 *
 *  Created on: Dec 20, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#include <random>
#include "systems/aisystem.h"

#include "components.h"
#include "ecs.h"
#include "randomgenerator.h"

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

  if(t_comp && ecs->hasComponent(entity, Component::ATTACK) && ecs->entityExists(t_comp->target))
  {
    auto p_comp   = ecs->getComponent<Component::Position>(entity,         Component::Type::POSITION);
    auto t_p_comp = ecs->getComponent<Component::Position>(t_comp->target, Component::Type::POSITION);

    //TODO: Random move instead of just cancelling
    if(!t_p_comp || !p_comp) return;

    int dx = t_p_comp->x - p_comp->x;
    int dy = t_p_comp->y - p_comp->y;

    // if both delta's are between -1 and 1 we can hit that shit!
    // also make sure we can actually hit it!
    if(dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1 &&
       ecs->hasComponent(t_comp->target, Component::DESTRUCTIBLE))
    {
      ecs->sendSystemMessage(ISystem::Type::ATTACK,
                  SystemMessage::TMessagePtr(new SystemMessage::AttackMessage(entity, t_comp->target)));
    }
    else
    {
      //ensure delta's are between -1 and 1
      if(dx > 1) dx = 1;
      else if(dx < -1) dx = -1;

      if(dy > 1) dy = 1;
      else if(dy < -1) dy = -1;

      ecs->sendSystemMessage(ISystem::Type::MOVEMENT,
          SystemMessage::TMessagePtr(new SystemMessage::MovementMessage(entity, dx, dy)));
    }
  }
  else
  {
    int dx = RandomGenerator::Instance()->randDelta();
    int dy = RandomGenerator::Instance()->randDelta();

      // move randomly
    ecs->sendSystemMessage(ISystem::Type::MOVEMENT,
        SystemMessage::TMessagePtr(new SystemMessage::MovementMessage(entity, dx, dy)));
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


