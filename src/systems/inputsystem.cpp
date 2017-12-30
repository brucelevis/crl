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
#include "BearLibTerminal.h"
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
		case TK_KP_7:
			dx = -1; dy = -1;
			break;
		case TK_UP:
		case TK_KP_8:
			dx = 0;dy = -1;
			break;
		case TK_KP_9:
			dx = 1; dy = -1;
			break;
		case TK_LEFT:
		case TK_KP_4:
			dx = -1; dy = 0;
			break;
		case TK_KP_5:
			//do nothing
			break;
		case TK_RIGHT:
		case TK_KP_6:
			dx = 1; dy = 0;
			break;
		case TK_KP_1:
			dx = -1; dy = 1;
			break;
		case TK_DOWN:
		case TK_KP_2:
			dx = 0;dy = 1;
			break;
		case TK_KP_3:
			dx = 1; dy = 1;
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



