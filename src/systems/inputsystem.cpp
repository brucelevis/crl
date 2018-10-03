/*
 * inputsystem.cpp
 *
 *  Created on: Dec 10, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#include "systems/inputsystem.h"
#include "components.h"
#include "ecs.h"

#include <memory>

InputSystem::InputSystem() :
    ISystem(Component::Type::INPUT
        , ISystem::Type::INPUT)
{

}

InputSystem::~InputSystem()
{

}

void InputSystem::handleMessage(SystemMessage::TMessagePtr message, uint64_t entity, ECS* ecs)
{
  if(message->type == SystemMessage::MessageType::INPUT)
  {
    auto imessage = std::static_pointer_cast<SystemMessage::InputMessage>(message);
    if(!imessage) return;


    int dx = 0;
    int dy = 0;

    switch(imessage->key)
    {
    case 'y':
      dx = -1; dy = -1;
      break;
    case 'k':
      dx = 0;dy = -1;
      break;
    case 'u':
      dx = 1; dy = -1;
      break;
    case 'h':
      dx = -1; dy = 0;
      break;
    case '.':
      //do nothing
      break;
    case 'l':
      dx = 1; dy = 0;
      break;
    case 'b':
      dx = -1; dy = 1;
      break;
    case 'j':
      dx = 0;dy = 1;
      break;
    case 'n':
      dx = 1; dy = 1;
      break;
    case 'g':
      {
        auto p_comp = ecs->getComponent<Component::Position>(entity, Component::POSITION);

        if(!p_comp) break;

        auto c_ents = ecs->getEntitiesWithComponent(Component::POSITION);

        std::vector<uint64_t> pick_targets;

        // Gather all posible targets
        for(uint64_t possible_target : c_ents)
        {
          if(possible_target == entity) continue;

          auto p_p_comp = ecs->getComponent<Component::Position>(possible_target, Component::POSITION);

          if(p_p_comp && p_p_comp->x == p_comp->x && p_p_comp->y == p_comp->y)
          {
            pick_targets.push_back(possible_target);
          }
        }

        //TODO: CHOOSE IF MORE
        if(!pick_targets.empty())
        {
          ecs->sendSystemMessage(ISystem::Type::CONTAINER,
                      SystemMessage::TMessagePtr(new SystemMessage::PickUpMessage(entity, pick_targets.at(0))));
        }
      }
      break;
    }

    if(dx != 0 || dy != 0)
    {
      bool attacked = false;
      auto p_comp = ecs->getComponent<Component::Position>(entity, Component::POSITION);

      if(p_comp && ecs->hasComponent(entity, Component::ATTACK))
      {
        auto c_ents = ecs->getEntitiesWithComponent(Component::POSITION);

        for(uint64_t possible_target : c_ents)
        {
          auto p_p_comp = ecs->getComponent<Component::Position>(possible_target, Component::POSITION);

          if(p_p_comp && p_p_comp->x == p_comp->x + dx &&
                       p_p_comp->y == p_comp->y + dy)
          {
            if(ecs->hasComponent(possible_target, Component::DESTRUCTIBLE))
            {
              // send the "I'd like to obliberate that thing" message
              ecs->sendSystemMessage(ISystem::Type::ATTACK,
                  SystemMessage::TMessagePtr(new SystemMessage::AttackMessage(entity, possible_target)));

              attacked = true;
              break;
            }
          }
        }
      }

      if(!attacked)
      {
        // send the "I like to move it, move it" message
        ecs->sendSystemMessage(ISystem::Type::MOVEMENT,
            SystemMessage::TMessagePtr(new SystemMessage::MovementMessage(entity, dx, dy)));
      }
    }
  }
}

/* virtual */ void InputSystem::update(ECS* ecs, float delta) /* = 0 */
{
  for(uint64_t entity : interested_entities)
  {
    auto i_comp = ecs->getComponent<Component::Input>(entity, Component::Type::INPUT);

    if(!i_comp) return;

    auto messages = ecs->getMessages(type);

    for(auto message : messages)
    {
      handleMessage(message, entity, ecs);
    }
  }
}



