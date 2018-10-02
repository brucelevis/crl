/*
 * attacksystem.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#include "systems/attacksystem.h"

#include "components.h"
#include "ecs.h"

#include "logger.h"

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
  auto mmessage = std::static_pointer_cast<SystemMessage::AttackMessage>(message);
  if(!mmessage) return;

  auto a_a_comp = ecs->getComponent<Component::Attack>      (mmessage->attacker, Component::Type::ATTACK);
  auto t_d_comp = ecs->getComponent<Component::Destructible>(mmessage->target,   Component::Type::DESTRUCTIBLE);
  if(!a_a_comp || ! t_d_comp) return;

  //Todo: More complex attack shizzle
  int16_t damage = a_a_comp->damage;

  if(ecs->hasComponent(mmessage->target, Component::PLAYER)) Logger::Instance()->logLine("Thou arth attacked");

  ecs->sendSystemMessage(ISystem::Type::DESTRUCTIBLE,
            SystemMessage::TMessagePtr(new SystemMessage::DamageMessage(mmessage->target, damage)));
}

/* virtual */ void AttackSystem::update(ECS* ecs, float delta) /* = 0 */
{
  auto messages = ecs->getMessages(type);

  for(auto message : messages)
  {
    handleMessage(message, ecs);
  }
}



