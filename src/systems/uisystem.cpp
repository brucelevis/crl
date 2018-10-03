/*
 * uisystem.cpp
 *
 *  Created on: Dec 30, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#include <sstream>

#include "systems/uisystem.h"

#include "components.h"
#include "ecs.h"

UISystem::UISystem() :
    ISystem(Component::Type::NONE
        , ISystem::Type::UI)
{

}

UISystem::~UISystem()
{

}

/* virtual */ void UISystem::update(ECS* ecs, float delta) /* = 0 */
{
  auto players = ecs->getEntitiesWithComponent(Component::Type::PLAYER);
  if(players.empty()) return;

  auto console = IConsole::Instance();

  uint64_t player_id = players.at(0);

  auto d_comp = ecs->getComponent<Component::Destructible> (player_id, Component::Type::DESTRUCTIBLE);
  auto m_comp = ecs->getComponent<Component::Mana>         (player_id, Component::Type::MANA);
  auto s_comp = ecs->getComponent<Component::Skill>        (player_id, Component::Type::SKILL);
  auto c_comp = ecs->getComponent<Component::Container>    (player_id, Component::Type::CONTAINER);

  if(d_comp)
  {
    console->set_color(IConsole::Color::WHITE);

    std::stringstream sstream;

    sstream << "HP: " << d_comp->cur_hp << "/" << d_comp->max_hp;

    console->put_string(42, 4, sstream.str());
    drawProgressBar(
        42, 5, 10, 0,
        d_comp->max_hp,
        d_comp->cur_hp,
        d_comp->max_hp * 0.2);
  }
  if(m_comp)
  {
    console->set_color(IConsole::Color::WHITE);

    std::stringstream sstream;

    sstream << "MP: " << m_comp->cur_mana << "/" << m_comp->max_mana;

    console->put_string(42, 6, sstream.str());
    drawProgressBar(
        42, 7, 10, 0,
        m_comp->max_mana,
        m_comp->cur_mana,
        m_comp->max_mana * 0.2,
        IConsole::Color::WHITE,
        IConsole::Color::BLUE);
  }
  if(s_comp)
  {
    console->set_color(IConsole::Color::WHITE);

    std::stringstream sstream;

    sstream << "ATK: " << s_comp->skills[Component::Skill::ATK];
    console->put_string(42, 9, sstream.str());
    sstream.str("");

    sstream << "STR: " << s_comp->skills[Component::Skill::STR];
    console->put_string(42, 10, sstream.str());
    sstream.str("");

    sstream << "AGI: " << s_comp->skills[Component::Skill::AGI];
    console->put_string(42, 11, sstream.str());
    sstream.str("");

    sstream << "INT: " << s_comp->skills[Component::Skill::INT];
    console->put_string(42, 12, sstream.str());
  }
  if(c_comp)
  {
    console->set_color(IConsole::Color::WHITE);

    std::stringstream sstream;

    sstream << "[[Inventory]]";

    for(uint64_t entity : c_comp->items)
    {
      auto e_n_comp = ecs->getComponent<Component::Name>(entity, Component::Type::NAME);

      if(!e_n_comp) continue;

      sstream << "\u2022 " << e_n_comp->text;
    }

    console->put_string(42, 15, sstream.str());
  }
}

void UISystem::drawProgressBar(
    uint16_t x,
    uint16_t y,
    uint16_t size,
    uint16_t min,
    uint16_t max,
    uint16_t value,
    uint16_t low_value /* = 0 */,
    IConsole::Color color_background /* = IConsole::Color::WHITE */,
    IConsole::Color color_foreground /* = IConsole::Color::GREEN */,
    IConsole::Color color_low        /* = IConsole::Color::RED */,
    uint16_t char_foreground /* = '=' */,
    uint16_t char_background /* = '-' */,
    bool horizontal /* = true */)
{
  auto console = IConsole::Instance();

  if(value <= low_value) console->set_color(color_low);
  else console->set_color(color_foreground);


  uint16_t filled_till = 0;

  if(value < min)
  {
    filled_till = 0;
  }
  else if(value == max || max == min)
  {
    filled_till = size;
  }
  else
  {
    filled_till = (value / (double)(max - min)) * size;
  }

  bool background_color_set = false;
  for(int i = 0; i < size; ++i)
  {
    if(!background_color_set && i > filled_till)
    {
      console->set_color(color_background);
      background_color_set = true;
    }

    if(background_color_set) console->set_char(x, y, char_background);
    else console->set_char(x, y, char_foreground);

    if(horizontal) ++x;
    else ++y;
  }
}


