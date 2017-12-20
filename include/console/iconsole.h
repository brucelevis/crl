/*
 * console.h
 *
 *  Created on: Dec 16, 2017
 *      Author: joshua
 */

#ifndef INCLUDE_ICONSOLE_H_
#define INCLUDE_ICONSOLE_H_

#include <cstdint>
#include <memory>

class IConsole
{
public:
	//! Console types that can be created
	enum class ConsoleType
	{
		CURSES 			= 0, /*!< Curses based console */
		BEARLIBTERMINAL = 1, /*!< BearLibTerminal based console */
		TILESBEARLIB	= 2  /*!< BearLibTerminal based tiles console */
	};

	//! Console colors
	enum Color
	{
		RED 		= 1,
		GREEN 		= 2,
		YELLOW 		= 3,
		BLUE 		= 4,
		CYAN 		= 5,
		MAGENTA 	= 6,
		WHITE 		= 7,
		GREY		= 8,
		DARK_GREY 	= 9
	};


	virtual ~IConsole();

	//! Initializes the console
	/*!
	 * \return true if init was a success, otherwise false
	 */
	virtual bool init() = 0;

	//! Clears the screen
	virtual void clscr() = 0;

	//! Sets the current drawing color
	/*!
	 * \param color The color to use
	 */
	virtual void set_color(Color color) = 0;

	//! Sets a char at position x and y
	/*!
	 * \param x The x position of the char
	 * \param y The y position of the char
	 * \param ch The char to put on screen
	 */
	virtual void set_char(uint16_t x, uint16_t y, uint16_t ch) = 0;

	//! Cleans up the console
	virtual void cleanup() = 0;

	//! Refresh terminal (Draw all buffers)
	virtual void refresh() = 0;

	//! Returns any key press
	/*!
	 * \return The last character pressed
	 */
	virtual int getChar() = 0;

	//! Determines if shift was pressed
	/*!
	 * \return True if shift was pressed with the last char
	 */
	virtual bool getShift() = 0;

	//! Determines if control was pressed
	/*!
	 * \return True if control was pressed with the last char
	 */
	virtual bool getControl() = 0;

	//! Determines if alt was pressed
	/*!
	 * \return True if alt was pressed with the last char
	 */
	virtual bool getAlt() = 0;

	//! Determine if the console should close
	/*!
	 * \return True if the console should be closed
	 */
	virtual bool shouldClose() = 0;

	//! Allocates a console
	/*!
	 * \param type The type of console to create
	 * \param width The width of the console in chars
	 * \param height The height of the console in chars
	 * \sa ConsoleType
	 */
	static void allocateConsole(ConsoleType type, uint16_t width, uint16_t height);

	//! Returns an instance of the allocated console
	/*!
	 * \return A shared pointer to the console instance
	 */
	static std::shared_ptr<IConsole>& Instance();

protected:
	//! Width of the console in chars
	uint16_t width;

	//! Height of the console in chars
	uint16_t height;

	//! Determines if a console was created
	static bool allocated;

	//! Instance of the console
	static std::shared_ptr<IConsole> console;


	//! Ctor
	/*!
	 * \param width Width of the console in chars
	 * \param height Height of the console in chars
	 */
	IConsole(uint16_t width, uint16_t height);
};

#endif /* INCLUDE_ICONSOLE_H_ */
