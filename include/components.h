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
		RENDER	 = (1 << 1),
		INPUT	 = (1 << 2)
	};

	struct Component {
		const Type type;

		Component(Type type) :
			type(type)
		{}
	};

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

	struct Input : public Component
	{
		Input() :
			Component(Type::INPUT)
		{}
	};

	typedef std::shared_ptr<Component> TComponentPtr;
	
	extern bool hasComponent(uint16_t cbits, Type t);
	extern TComponentPtr createNew(Type componentType);
}

#endif /* INCLUDE_COMPONENTS_H_ */

