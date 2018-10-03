/*
 * ncursesconsole.cpp
 *
 *  Created on: Oct 3, 2018
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2018 - Joshua Lettink
 */

#include <ncurses.h>

#include "console/ncursesconsole.h"
#include "logger.h"

NCursesConsole::NCursesConsole(uint16_t width, uint16_t height) :
  IConsole(width, height),
  last_pressed_char(0)
{

}

/* virtual */ bool NCursesConsole::init()
{
  initscr();
  raw();
  keypad(stdscr, true);
  noecho();

  start_color();

  init_pair(Color::RED,       COLOR_RED,     COLOR_BLACK);
  init_pair(Color::GREEN,     COLOR_GREEN,   COLOR_BLACK);
  init_pair(Color::YELLOW,    COLOR_YELLOW,  COLOR_BLACK);
  init_pair(Color::BLUE,      COLOR_BLUE,    COLOR_BLACK);
  init_pair(Color::CYAN,      COLOR_CYAN,    COLOR_BLACK);
  init_pair(Color::MAGENTA,   COLOR_MAGENTA, COLOR_BLACK);
  init_pair(Color::WHITE,     COLOR_WHITE,   COLOR_BLACK);
  init_pair(Color::GREY,      COLOR_WHITE,   COLOR_BLACK);
  init_pair(Color::DARK_GREY, COLOR_WHITE,   COLOR_BLACK);

  return true;
}

/* virtual */ void NCursesConsole::clscr()
{
  clear();
}

/* virtual */ void NCursesConsole::set_color(Color color)
{
  //terminal_color(color_map[color]);
  attron(A_BOLD);

  switch(color)
  {
    case Color::GREY:
      attroff(A_BOLD);
      break;
    case Color::DARK_GREY:
      attroff(A_BOLD);
      attron(A_DIM);
      break;
    default:
      break;
  }

  attron(COLOR_PAIR(static_cast<uint16_t>(color)));
}

/* virtual */ void NCursesConsole::set_char(uint16_t x, uint16_t y, uint16_t ch)
{
  mvaddch(y, x, ch);
}

/* virtual */ void NCursesConsole::put_string(uint16_t x, uint16_t y, const std::string& string)
{
  mvaddstr(y, x, string.c_str());
}

/* virtual */ void NCursesConsole::cleanup()
{
  endwin();
}

/* virtual */ void NCursesConsole::refresh()
{
  ::refresh();
}

/* virtual */  int NCursesConsole::getChar()
{
  /**
  * NCURSES NUMPAD LAYOUT
  *
  * |a1|up|a3|
  * |le|b2|ri|
  * |c1|do|c3|
  *
  * where le = left, ri = right, up = up and do = down keys
  */
  last_pressed_char = getch();

  switch(last_pressed_char)
  {
  case KEY_LEFT:
    last_pressed_char = 'h';
    break;
  case KEY_RIGHT:
    last_pressed_char = 'l';
    break;
  case KEY_DOWN:
    last_pressed_char = 'j';
    break;
  case KEY_UP:
    last_pressed_char = 'k';
    break;
  case KEY_A1:
    last_pressed_char = 'y';
    break;
  case KEY_A3:
    last_pressed_char = 'u';
    break;
  case KEY_C1:
    last_pressed_char = 'b';
    break;
  case KEY_C3:
    last_pressed_char = 'n';
    break;

  }

  return last_pressed_char;
}

/* virtual */  bool NCursesConsole::shouldClose()
{
  return last_pressed_char == 'q';
}

/* virtual */  bool NCursesConsole::getShift()
{
  return false;
}

/* virtual */  bool NCursesConsole::getControl()
{
  return false; 
}

/* virtual */  bool NCursesConsole::getAlt()
{
  return false; 
}


