/*
 * tiles.cpp
 *
 *  Created on: Dec 10, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#include "tiles.h"

namespace Tiles
{
	std::map<std::string, uint16_t> tilenames = std::map<std::string, uint16_t>();
	std::map<uint16_t, TileDefinition> tiledefs = std::map<uint16_t, TileDefinition>();

	const TileDefinition& getDefinition(uint16_t defid)
	{
		return tiledefs[defid];
	}

	const TileDefinition& getDefinitionByName(const std::string& name)
	{
		return getDefinition(tilenames[name]);
	}

	uint16_t getIDByName(const std::string& name)
	{
		return tilenames[name];
	}

	void createDefinition(uint16_t defid, const std::string& name, uint16_t flags, IConsole::Color color, uint16_t glyph)
	{
		bool name_exists = false;

		if(tilenames.find(name) != tilenames.end())
		{
			name_exists = true;
		}

		std::stringstream sstream;
		sstream << "Adding tile definition: { id: " << defid << " flags: " <<  std::bitset<16>(flags)
				<< " color: " << std::hex << color << std::dec << " glyph: " << glyph;
		Logger::Instance()->logLine(Logger::Level::LDEBUG, sstream.str());

		if(tiledefs.find(defid) != tiledefs.end())
		{
			sstream.str("");
			sstream.clear();
			sstream << "tile with defid " << defid << " already exists! Overwriting with this one!";
			Logger::Instance()->logLine(Logger::Level::LWARNING, sstream.str());
		}
		else if (name_exists)
		{
			sstream.str("");
			sstream.clear();
			sstream << "tile with name " << name << " already exists! Overwriting with this one!";
			Logger::Instance()->logLine(Logger::Level::LWARNING, sstream.str());
		}


//		uint8_t a = 		  (color >> 24)   & 0xFF;
//		uint8_t r = (uint8_t) (((color >> 16) & 0xFF) * DARKEN_FACTOR);
//		uint8_t g = (uint8_t) (((color >> 8)  & 0xFF) * DARKEN_FACTOR);
//		uint8_t b = (uint8_t) ((color         & 0xFF) * DARKEN_FACTOR);

		//.color_t darker_color = (a << 24) | (r << 16) | (g << 8) | b;


		TileDefinition newDefinition { flags, color, IConsole::Color::DARK_GREY, glyph };
		tiledefs[defid] = newDefinition;
		tilenames[name] = defid;
	}
}


