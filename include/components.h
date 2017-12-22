/*
 * inputsystem.h
 *
 *  Created on: Dec 15, 2017
 *      Author: Joshua Lettink
 */

#ifndef INCLUDE_COMPONENTS_H_
#define INCLUDE_COMPONENTS_H_

#include <cstdint>
#include <memory>
#include <map>
#include <exception>
#include "console/iconsole.h"

#include "deprecated.h"

namespace Component
{
	enum Type
	{
		NONE	 	 = 0,
		POSITION 	 = (1 << 0),  /*!< Contains location */
		MOVEMENT 	 = (1 << 1),  /*!< Can move and has delta's */
		RENDER	 	 = (1 << 2),  /*!< Contains render information */
		INPUT	 	 = (1 << 3),  /*!< Gathers and reacts to input */
		AI		 	 = (1 << 4),  /*!< Has an AI */
		SKILL	 	 = (1 << 5),  /*!< Has skill modifiers */
		TARGET	 	 = (1 << 6),  /*!< Targets something */
		CAMERA   	 = (1 << 7),  /*!< Has a viewport definition */
		DESTRUCTIBLE = (1 << 8),  /*!< Can be destroyed */
		ATTACK       = (1 << 9),  /*!< Can attack */
		PLAYER		 = (1 << 10), /*!< Is a player */
		PICKABLE     = (1 << 11), /*!< Can be picked up */
		THROW        = (1 << 12), /*!< Can be thrown */
		EQUIP        = (1 << 13), /*!< Is equipable */
		CONTAINER    = (1 << 14), /*!< Contains other entities */
		CONSUME      = (1 << 15), /*!< Can be consumed */
		EFFECT       = (1 << 16), /*!< Has an effect */
		DOOR         = (1 << 17), /*!< Opens and closes */
		NOTUSED      = (1 << 18), /*!< NOT USED */
		STAIR        = (1 << 19), /*!< Is a staircase */
		READ         = (1 << 20), /*!< Can be read */
		FLY          = (1 << 21), /*!< Entity flies */
		SOLID        = (1 << 22), /*!< Cannot be walked through */
		PATH         = (1 << 23), /*!< Contains waypoints for movement */
		TALK         = (1 << 24), /*!< Can be talked to or talks */
		NAME         = (1 << 25), /*!< Name of the entity */
		DESCRIPTION  = (1 << 26), /*!< Description of the entity */
		RARITY       = (1 << 27)  /*!< Determines how rare an entity is */
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

	//! Camera component, used for viewports
	struct Camera : public Component
	{
		uint16_t view_width;  /*! Viewport width */
		uint16_t view_height; /*! Viewport height */
		uint16_t x_offset;    /*! X offset for rendering */
		uint16_t y_offset;    /*! Y offset for rendering */

		Camera(uint16_t view_width, uint16_t view_height) :
			Component(Type::CAMERA)
		  , view_width(view_width)
		  , view_height(view_height)
		  , x_offset(0)
		  , y_offset(0)
		{
			if(view_width % 2 > 0 || view_height % 2 > 0)
				throw std::runtime_error("Camera component viewport should be a power of two!");

		}
	};

	//! Destructible component, used to assign health to something
	struct Destructible : public Component
	{
		uint16_t max_hp; /*! Maximum hp */
		uint16_t cur_hp; /*! Current hp */

		Destructible(uint16_t max_hp) :
			Component(Type::DESTRUCTIBLE)
		  , max_hp(max_hp)
		  , cur_hp(max_hp)
		{}
	};

	//! Attack component, used to attack stuff
	struct Attack : public Component
	{
		int16_t damage;

		Attack(int16_t damage) :
			Component(Type::ATTACK)
		  , damage(damage)
		{}
	};

	//! Component used to keep track of the player
	struct Player : public Component
	{
		Player() :
			Component(Type::PLAYER)
		{}
	};

	//! Can be picked up
	struct Pickable : Component
	{
		Pickable() :
			Component(Type::PICKABLE)
		{}
	};

	//! Can be thrown
	struct Throw : Component
	{
		Throw() :
			Component(Type::THROW)
		{}
	};

	//! Can be equipped
	struct Equip : Component
	{
		Equip() :
			Component(Type::EQUIP)
		{}
	};

	//! Can contain other entities
	struct Container : Component
	{
		Container() :
			Component(Type::CONTAINER)
		{}
	};

	//! Can be consumed
	struct Consume : Component
	{
		Consume() :
			Component(Type::CONSUME)
		{}
	};

	//! Applies an effect
	struct Effect : Component
	{
		Effect() :
			Component(Type::EFFECT)
		{}
	};

	//! Door
	struct Door : Component
	{
		Door() :
			Component(Type::DOOR)
		{}
	};

	//! Stairs
	struct Stair : Component
	{
		Stair() :
			Component(Type::STAIR)
		{}
	};

	//! Can be read
	struct Read : Component
	{
		Read() :
			Component(Type::READ)
		{}
	};

	//! Can fly
	struct Fly : Component
	{
		Fly() :
			Component(Type::FLY)
		{}
	};

	//! Contains pathing information
	struct Path : Component
	{
		Path() :
			Component(Type::PATH)
		{}
	};

	//! Can talk or be talked to
	struct Talk : Component
	{
		Talk() :
			Component(Type::TALK)
		{}
	};

	//! Name
	struct Name : Component
	{
		Name() :
			Component(Type::NAME)
		{}
	};

	//! Description
	struct Description : Component
	{
		Description() :
			Component(Type::DESCRIPTION)
		{}
	};

	//! Determines rarity of an entity
	struct Rarity : Component
	{
		Rarity() :
			Component(Type::RARITY)
		{}
	};

	typedef std::shared_ptr<Component> TComponentPtr; /*! Shared_ptr to Component type declaration */
	
	DEPRECATED extern bool hasComponent(uint16_t cbits, Type t);
	DEPRECATED extern TComponentPtr createNew(Type componentType);
}

#endif /* INCLUDE_COMPONENTS_H_ */

