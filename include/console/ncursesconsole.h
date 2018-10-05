/*
 * ncursesconsole.h
 *
 * Created on: Oct 3, 2018
 *     Author: Joshua Lettink
 *
 * Copyright (C) 2018 - Joshua Lettink
 */

#ifdef USE_NCURSES
#ifndef INCLUDE_NCURSESCONSOLE_H_
#define INCLUDE_NCURSESCONSOLE_H_

#include "iconsole.h"

class NCursesConsole : public IConsole
{
public:
  NCursesConsole(uint16_t width, uint16_t height);

  /* virtual */ bool init(); /* =0 */

  /* virtual */ void clscr(); /* =0 */

  /* virtual */ void set_color(IConsole::Color color); /* =0 */
  
  /* virtual */ void set_char(uint16_t x, uint16_t y, uint16_t ch); /* =0 */

  /* virtual */ void put_string(uint16_t x, uint16_t y, const std::string& string); /* =0 */

  /* virtual */ void cleanup(); /* =0 */
  
  /* virtual */ void refresh(); /* =0 */

  /* virtual */ int getChar(); /* =0 */

  /* virtual */ bool getShift(); /* =0 */

  /* virtual */ bool getControl(); /* =0 */

  /* virtual */ bool getAlt(); /* =0 */
 
  /* virtual */ bool shouldClose(); /* =0 */
private:
  int last_pressed_char;
};

#endif /* INCLUDE_NCURSESCONSOLE_H_ */
#endif /* USE_NCURSES */

