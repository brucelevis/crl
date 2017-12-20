/*
 * inputsystem.h
 *
 *  Created on: Dec 15, 2017
 *      Author: joshua
 */

#ifndef INCLUDE_COMPONENTS_H_
#define INCLUDE_COMPONENTS_H_

#include <cstdint>
#include <memory>
#include <map>
#include "console/iconsole.h"

namespace Component
{
	enum Type
	{
		NONE	 = 0,
		POSITION = (1 << 0),
		MOVEMENT = (1 << 1),
		RENDER	 = (1 << 2),
		INPUT	 = (1 << 3),
		AI		 = (1 << 4),
		SKILL	 = (1 << 5),
		TARGET	 = (1 << 6)
	};

	struct Component {
		const Type type;

		Component(Type type) :
			type(type)
		{}
	};

	//! Position component, used to track positions
	struct Position : public Component
	{
		uint16_t x;
		uint16_t y;
		
		Position(uint16_t x  = 0, uint16_t y  = 0) :
			Component(Type::POSITION)
		  , x (x)
		  , y (y)
		{}
	};
	
	//! Movement component, used to determine if movement is possible and the speed
	struct Movement : public Component
	{
		int16_t speed; /*!speed of movement */

		Movement(int16_t speed) :
			Component(Type::MOVEMENT)
		  , speed(speed)
		{}
	};

	//! Render component, used to render stuff
	struct Render : public Component
	{
		IConsole::Color color;
		uint16_t glyph;
		
		Render(IConsole::Color color = IConsole::Color::WHITE, uint16_t glyph = '-') :
			Component(Type::RENDER)
		  , color (color)
		  , glyph (glyph)
		{}
	};

	//! Input component, used to determine if input is possible
	struct Input : public Component
	{
		Input() :
			Component(Type::INPUT)
		{}
	};

	//! AI component, used to determine AI type
	struct AIComponent : public Component
	{
		//! AI Types
		enum AIType
		{
			NONE = 0,    /*!< None */
			DUMB,        /*!< Dumb AI, does not think about anything (just attack) */
			ANIMAL,      /*!< Animal AI, thinks some more, could decide to run for instance */
			INTELLIGENT, /*!< Intelligent AI, can speak/cast spells etc. */
			OMNISCIENT,  /*!< Omniscient AI, is perfect */
			PLAYER       /*!< Used for the player */
		};

		AIType ai_type; /*! Determines the type of ai */

		AIComponent(AIType ai_type) :
			Component(Type::AI)
		  , ai_type(ai_type)
		{}
	};

	//! Skill component, used for determining possible actions/results etc.
	struct Skill : public Component
	{
		//! Skill types
		enum SkillType
		{
			None = 0, /*!< None */
			ATK,      /*!< Attack, used for accuracy/attack speed etc. */
			STR,      /*!< Strength, used to determine power */
			AGI,      /*!< Agility, used to determine speed etc. */
			INT       /*!< Intelligence, used for various intellect checks etc. Influences mana. */
		};

		std::map<SkillType, int16_t> skills; /*! Skills container, used to keep track of skill values */

		Skill(std::map<SkillType, int16_t> skills) :
			Component(Type::SKILL)
		  , skills(skills)
		{}
	};

	//! Target component, used to target other entities
	struct Target : public Component
	{
		uint64_t target; /*! Target entity */

		Target(uint64_t target) :
			Component(Type::TARGET)
		  , target(target)
		{}
	};

	typedef std::shared_ptr<Component> TComponentPtr;
	
	extern bool hasComponent(uint16_t cbits, Type t);
	extern TComponentPtr createNew(Type componentType);
}

#endif /* INCLUDE_COMPONENTS_H_ */

