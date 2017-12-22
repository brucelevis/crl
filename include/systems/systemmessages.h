/*
 * systemmessages.h
 *
 *  Created on: Dec 10, 2017
 *      Author: Joshua Lettink
 */

#ifndef INCLUDE_SYSTEMMESSAGES_H_
#define INCLUDE_SYSTEMMESSAGES_H_

#include <memory>

namespace SystemMessage
{
	enum MessageType
	{
		NONE  = 0,
		INPUT,
		MOVEMENT,
		ATTACK,
		DAMAGE
	};

	struct Message
	{
		MessageType type;

		Message(MessageType type) :
			type(type)
		{}
	};

	struct InputMessage : public Message
	{
		int  key;
		bool shift;
		bool control;
		bool alt;

		InputMessage(int key, bool shift, bool control, bool alt) :
			Message(MessageType::INPUT)
		  , key     (key)
		  , shift   (shift)
		  , control (control)
		  , alt     (alt)
		{}
	};

	struct MovementMessage : public Message
	{
		uint64_t entity;

		int delta_x;
		int delta_y;

		MovementMessage(uint64_t entity, int delta_x, int delta_y) :
			Message(MessageType::MOVEMENT)
		  ,	entity(entity)
		  , delta_x(delta_x)
		  , delta_y(delta_y)
		{}
	};

	struct AttackMessage : public Message
	{
		uint64_t attacker;
		uint64_t target;

		AttackMessage(uint64_t attacker, uint64_t target) :
			Message(MessageType::ATTACK)
		  ,	attacker(attacker)
		  , target(target)
		{}
	};

	struct DamageMessage : public Message
	{
		uint64_t target;
		int16_t damage;

		DamageMessage(uint64_t target, int16_t damage) :
			Message(MessageType::DAMAGE)
		  ,	target(target)
		  , damage(damage)
		{}
	};

	typedef std::shared_ptr<Message> TMessagePtr;
}



#endif /* INCLUDE_SYSTEMMESSAGES_H_ */
