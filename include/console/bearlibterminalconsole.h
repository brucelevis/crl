/*
 * bearlibterminalconsole.h
 *
 *  Created on: Dec 18, 2017
 *      Author: Joshua Lettink
 */

#ifndef INCLUDE_CONSOLE_BEARLIBTERMINALCONSOLE_H_
#define INCLUDE_CONSOLE_BEARLIBTERMINALCONSOLE_H_

#include <BearLibTerminal.h>
#include <cstdint>
#include <map>
#include "console/iconsole.h"

class BearLibTerminalConsole : public IConsole
{
public:
	//! Ctor
	/*!
	 * \param width Width of the console in chars
	 * \param height Height of the console in chars
	 */
	BearLibTerminalConsole(uint16_t width, uint16_t height);

	virtual bool init();
	virtual void clscr();

	virtual void set_color(Color color);
	virtual void set_char(uint16_t x, uint16_t y, uint16_t ch);

	virtual void cleanup();
	virtual void refresh();
	virtual bool shouldClose();

	virtual int getChar();
	virtual bool getShift();
	virtual bool getControl();
	virtual bool getAlt();

private:
	int last_pressed_key;

	std::map<Color, color_t> color_map;
};


#endif /* INCLUDE_CONSOLE_BEARLIBTERMINALCONSOLE_H_ */
