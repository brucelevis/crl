/*
 * uisystem.h
 *
 *  Created on: Dec 30, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#ifndef INCLUDE_SYSTEMS_UISYSTEM_H_
#define INCLUDE_SYSTEMS_UISYSTEM_H_

#include "systems/isystem.h"
#include "console/iconsole.h"

class UISystem : public ISystem
{
public:
  UISystem();
    ~UISystem();

    virtual void update(ECS* ecs, float delta) /* = 0 */;

private:
    //! Draws a progress bar
    /*!
     * \param x X location of the progress bar
     * \param y Y location of the progress bar
     * \param size Size of the progress bar
     * \param min Min value of the progress bar
     * \param max Max value of the progress bar
     * \param value Current value of the progress bar
     * \param low_value Value where value is considered low
     * \param color_background Color of the background chars
     * \param color_foreground Color of the foreground chars
     * \param color_low Color of the foreground chars when value <= low_value
     * \oaram char_foreground Foreground character to use
     * \param char_background Background character to use
     * \param horizontal If true bar will be drawn horizontal, otherwise vertical
     */
    void drawProgressBar(
        uint16_t x,
      uint16_t y,
      uint16_t size,
      uint16_t min,
      uint16_t max,
      uint16_t value,
      uint16_t low_value = 0,
      IConsole::Color color_background = IConsole::Color::WHITE,
      IConsole::Color color_foreground = IConsole::Color::GREEN,
      IConsole::Color color_low        = IConsole::Color::RED,
      uint16_t char_foreground = '=',
      uint16_t char_background = '-',
      bool horizontal = true);
};

#endif /* INCLUDE_SYSTEMS_UISYSTEM_H_ */
