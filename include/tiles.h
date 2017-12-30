/*
 * tiles.h
 *
 *  Created on: Dec 10, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#ifndef INCLUDE_TILES_H_
#define INCLUDE_TILES_H_

#include <cstdint>
#include <map>
#include <sstream>
#include <bitset>

#include "console/iconsole.h"
#include "logger.h"

#define DARKEN_FACTOR 0.25f

namespace Tiles
{
	enum Flags
	{
		NONE        = 0,
		TRANSPARENT = (1 << 0),
		BLOCKING    = (1 << 1)
	};

	struct TileDefinition
	{
		uint16_t flags;

		IConsole::Color  color;
		IConsole::Color  darker_color;

		uint16_t glyph;

		TileDefinition& operator=(const TileDefinition& def)
		{
			flags 		 = def.flags;
			color 		 = def.color;
			darker_color = def.darker_color;
			glyph 		 = def.glyph;

			return *this;
		}
	};

	extern std::map<uint16_t, TileDefinition> tiledefs;

	extern const TileDefinition& getDefinition(uint16_t defid);

	extern void createDefinition(uint16_t defid, uint16_t flags, IConsole::Color color, uint16_t glyph);
}



#endif /* INCLUDE_TILES_H_ */
