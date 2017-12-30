/*
 * console.cpp
 *
 *  Created on: Dec 18, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#include <exception>

#include "console/iconsole.h"

#include "console/bearlibterminalconsole.h"

/* static */ std::shared_ptr<IConsole> IConsole::console;
/* static */ bool IConsole::allocated = false;


IConsole::IConsole(uint16_t width, uint16_t height) :
	width(width), height(height)
{}

/* virtual */ IConsole::~IConsole()
{

}

/* static */  void IConsole::allocateConsole(ConsoleType type, uint16_t width, uint16_t height)
{
	if(allocated) throw std::runtime_error("Cannot allocate multiple consoles!");

	switch(type)
	{
	case ConsoleType::CURSES:
		throw std::runtime_error("CURSES console not implemented");
	case ConsoleType::BEARLIBTERMINAL:
		console = std::shared_ptr<IConsole>(new BearLibTerminalConsole(width, height));
		break;
	case ConsoleType::TILESBEARLIB:
		throw std::runtime_error("TILESBEARLIB console not implemented");
	}

	if(!console->init())
		throw std::runtime_error("Could not allocate console!");

	allocated = true;
}

/* static */ std::shared_ptr<IConsole>& IConsole::Instance()
{
	if(!allocated) throw std::runtime_error("No console allocated!");

	return console;
}
