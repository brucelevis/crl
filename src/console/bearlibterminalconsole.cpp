/*
 * bearlibterminalconsole.cpp
 *
 *  Created on: Dec 18, 2017
 *      Author: joshua
 */

#include <BearLibTerminal.h>

#include "console/bearlibterminalconsole.h"
#include "logger.h"

BearLibTerminalConsole::BearLibTerminalConsole(uint16_t width, uint16_t height) :
	IConsole(width, height)
  , last_pressed_key(0)
{

}

/* virtual */ bool BearLibTerminalConsole::init()
{
	if(!terminal_open())
	{
		Logger::Instance()->logLine(Logger::Level::LCRITICAL, "error initializing terminal");
		return false;
	}

	#ifndef NDEBUG
		const char* config =
			"window: title='CRL - DEBUG', size=80x25;\
			input: alt-functions=false;\
			log.file='terminal.log';\
			log.level=debug;";
	#else
		const char* config =
			"window: title='CRL', size=80x25;\
			input: alt-functions=false;\
			log.file='terminal.log';";
	#endif

	if(!terminal_set(config))
	{
		Logger::Instance()->logLine(Logger::Level::LCRITICAL, "error setting terminal settings");
		return false;
	}

	return true;
}

/* virtual */ void BearLibTerminalConsole::clscr()
{
	terminal_clear();
}

/* virtual */ void BearLibTerminalConsole::set_color(Color color)
{

}

/* virtual */ void BearLibTerminalConsole::set_char(uint16_t x, uint16_t y, uint16_t ch)
{
	terminal_put(x, y, ch);
}

/* virtual */ void BearLibTerminalConsole::cleanup()
{
	terminal_close();
}

/* virtual */ void BearLibTerminalConsole::refresh()
{
	terminal_refresh();
}

/* virtual */  int BearLibTerminalConsole::getChar()
{
	last_pressed_key = terminal_read();
	return last_pressed_key;
}

/* virtual */  bool BearLibTerminalConsole::shouldClose()
{
	return last_pressed_key == TK_CLOSE;
}

/* virtual */  bool BearLibTerminalConsole::getShift()
{
	return terminal_state(TK_SHIFT);
}

/* virtual */  bool BearLibTerminalConsole::getControl()
{
	return terminal_state(TK_CONTROL);
}

/* virtual */  bool BearLibTerminalConsole::getAlt()
{
	return terminal_state(TK_ALT);
}


