/*
 * inputsystem.h
 *
 *  Created on: Dec 30, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */


#include "components.h"

namespace Component
{
	bool hasComponent(uint16_t cbits, Type t)
	{
		return (cbits & static_cast<uint16_t>(t)) > 0;
	}

	TComponentPtr createNew(Type componentType)
	{
		switch(componentType)
		{
		case Type::POSITION:
			return std::make_shared<Position>();
		case Type::RENDER:
			return std::make_shared<Render>();
		case Type::NONE:
		default:
			return nullptr;
		}
	}
}
