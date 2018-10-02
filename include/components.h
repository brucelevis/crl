/*
 * inputsystem.h
 *
 *  Created on: Dec 15, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#ifndef INCLUDE_COMPONENTS_H_
#define INCLUDE_COMPONENTS_H_

#include <cstdint>
#include <memory>
#include <map>
#include <exception>
#include <string>
#include <vector>

#include "console/iconsole.h"

#include "deprecated.h"

namespace Component
{
  enum Type : uint64_t
  {
    NONE     = 0,
    POSITION   = ((uint64_t)1 << 0),  /*!< Contains location */
    MOVEMENT   = ((uint64_t)1 << 1),  /*!< Can move and has delta's */
    RENDER     = ((uint64_t)1 << 2),  /*!< Contains render information */
    INPUT    = ((uint64_t)1 << 3),  /*!< Gathers and reacts to input */
    AI       = ((uint64_t)1 << 4),  /*!< Has an AI */
    SKILL    = ((uint64_t)1 << 5),  /*!< Has skill modifiers */
    TARGET     = ((uint64_t)1 << 6),  /*!< Targets something */
    CAMERA     = ((uint64_t)1 << 7),  /*!< Has a viewport definition */
    DESTRUCTIBLE = ((uint64_t)1 << 8),  /*!< Can be destroyed */
    ATTACK       = ((uint64_t)1 << 9),  /*!< Can attack */
    PLAYER     = ((uint64_t)1 << 10), /*!< Is a player */
    PICKABLE     = ((uint64_t)1 << 11), /*!< Can be picked up */
    THROWABLE    = ((uint64_t)1 << 12), /*!< Can be thrown */
    EQUIPER      = ((uint64_t)1 << 13), /*!< Can equip */
    CONTAINER    = ((uint64_t)1 << 14), /*!< Contains other entities */
    CONSUMER     = ((uint64_t)1 << 15), /*!< Can consume */
    EFFECT       = ((uint64_t)1 << 16), /*!< Has an effect that will be applied to something */
    DOOR         = ((uint64_t)1 << 17), /*!< Opens and closes */
    WEAPON       = ((uint64_t)1 << 18), /*!< Used to determine weapon type*/
    STAIR        = ((uint64_t)1 << 19), /*!< Is a staircase */
    READ         = ((uint64_t)1 << 20), /*!< Can be read */
    FLY          = ((uint64_t)1 << 21), /*!< Entity flies */
    SOLID        = ((uint64_t)1 << 22), /*!< Cannot be walked through */
    PATH         = ((uint64_t)1 << 23), /*!< Contains waypoints for movement */
    TALK         = ((uint64_t)1 << 24), /*!< Can be talked to or talks */
    NAME         = ((uint64_t)1 << 25), /*!< Name of the entity */
    DESCRIPTION  = ((uint64_t)1 << 26), /*!< Description of the entity */
    RARITY       = ((uint64_t)1 << 27), /*!< Determines how rare an entity is */
    CONSUMABLE   = ((uint64_t)1 << 28), /*!< Determines if something can be consumed */
    EQUIPABLE    = ((uint64_t)1 << 29), /*!< Determine if something can be equipped */
    THROWER    = ((uint64_t)1 << 30), /*!< Can throw stuff */
    TIMER        = ((uint64_t)1 << 31), /*!< Does a specified action each x turns, multiple can be specied */
    CLOUD        = ((uint64_t)1 << 32), /*!< Used if entity is not solid, but blocked by walls & doors */
    MANA     = ((uint64_t)1 << 33), /*!< Used to keep track of mana for magic */
    DROPABLE   = ((uint64_t)1 << 34), /*!< Determines if something can be dropped */
    PICKER     = ((uint64_t)1 << 35), /*!< Determines if something can pick stuff up */
    DROPPER    = ((uint64_t)1 << 36), /*!< Determines if something can drop stuff */
    SPHERE     = ((uint64_t)1 << 37), /*!< Sphere an entity is associated with */
    DEITY    = ((uint64_t)1 << 38)  /*!< Determines if an entity is a deity */
  };

  struct Component {
    const Type type;

    Component(Type type) :
      type(type)
    {}
  };

  //! Position component, used to track positions
  struct Position : public Component
  {
    uint16_t x;
    uint16_t y;
    
    Position(uint16_t x  = 0, uint16_t y  = 0) :
      Component(Type::POSITION)
      , x (x)
      , y (y)
    {}
  };
  
  //! Movement component, used to determine if movement is possible and the speed
  struct Movement : public Component
  {
    int16_t speed; /*!speed of movement */

    Movement(int16_t speed) :
      Component(Type::MOVEMENT)
      , speed(speed)
    {}
  };

  //! Render component, used to render stuff
  struct Render : public Component
  {
    IConsole::Color color;
    uint16_t glyph;
    uint16_t layer;
    
    Render(IConsole::Color color = IConsole::Color::WHITE, uint16_t glyph = '-', uint16_t layer = 0) :
      Component(Type::RENDER)
      , color (color)
      , glyph (glyph)
      , layer (layer)
    {}
  };

  //! Input component, used to determine if input is possible
  struct Input : public Component
  {
    Input() :
      Component(Type::INPUT)
    {}
  };

  //! AI component, used to determine AI type
  struct AIComponent : public Component
  {
    //! AI Types
    enum AIType
    {
      NONE = 0,    /*!< None */
      DUMB,        /*!< Dumb AI, does not think about anything (just attack) */
      ANIMAL,      /*!< Animal AI, thinks some more, could decide to run for instance */
      INTELLIGENT, /*!< Intelligent AI, can speak/cast spells etc. */
      OMNISCIENT,  /*!< Omniscient AI, is perfect */
      PLAYER       /*!< Used for the player */
    };

    AIType ai_type; /*! Determines the type of ai */

    AIComponent(AIType ai_type) :
      Component(Type::AI)
      , ai_type(ai_type)
    {}
  };

  //! Skill component, used for determining possible actions/results etc.
  struct Skill : public Component
  {
    //! Skill types
    enum SkillType
    {
      None = 0, /*!< None */
      ATK,      /*!< Attack, used for accuracy/attack speed etc. */
      STR,      /*!< Strength, used to determine power */
      AGI,      /*!< Agility, used to determine speed etc. */
      INT       /*!< Intelligence, used for various intellect checks etc. Influences mana. */
    };

    std::map<SkillType, int16_t> skills; /*! Skills container, used to keep track of skill values */

    Skill(std::map<SkillType, int16_t> skills) :
      Component(Type::SKILL)
      , skills(skills)
    {}
  };

  //! Target component, used to target other entities
  struct Target : public Component
  {
    uint64_t target; /*! Target entity */

    Target(uint64_t target) :
      Component(Type::TARGET)
      , target(target)
    {}
  };

  //! Camera component, used for viewports
  struct Camera : public Component
  {
    uint16_t view_width;    /*! Viewport width */
    uint16_t view_height;   /*! Viewport height */
    uint16_t x_offset;      /*! X offset for rendering */
    uint16_t y_offset;      /*! Y offset for rendering */
    uint16_t viewport_x_loc;/*! X location of viewport on screen */
    uint16_t viewport_y_loc;/*! Y location of viewport on screen */

    Camera(uint16_t view_width, uint16_t view_height, uint16_t viewport_x_loc, uint16_t viewport_y_loc) :
      Component(Type::CAMERA)
      , view_width(view_width)
      , view_height(view_height)
      , x_offset(0)
      , y_offset(0)
      , viewport_x_loc(viewport_x_loc)
      , viewport_y_loc(viewport_y_loc)
    {
      //if(view_width % 2 > 0 || view_height % 2 > 0)
      //  throw std::runtime_error("Camera component viewport should be a power of two!");

    }
  };

  //! Destructible component, used to assign health to something
  struct Destructible : public Component
  {
    uint16_t max_hp; /*! Maximum hp */
    uint16_t cur_hp; /*! Current hp */

    Destructible(uint16_t max_hp) :
      Component(Type::DESTRUCTIBLE)
      , max_hp(max_hp)
      , cur_hp(max_hp)
    {}
  };

  //! Attack component, used to attack stuff
  struct Attack : public Component
  {
    int16_t damage;

    Attack(int16_t damage) :
      Component(Type::ATTACK)
      , damage(damage)
    {}
  };

  //! Component used to keep track of the player
  struct Player : public Component
  {
    Player() :
      Component(Type::PLAYER)
    {}
  };

  //! Can be picked up
  struct Pickable : Component
  {
    Pickable() :
      Component(Type::PICKABLE)
    {}
  };

  //! Can be thrown
  struct Throwable : Component
  {
    Throwable() :
      Component(Type::THROWABLE)
    {}
  };

  //! Can be equipped
  struct Equiper : Component
  {
    Equiper() :
      Component(Type::EQUIPER)
    {}
  };

  //! Can contain other entities
  struct Container : Component
  {
    std::vector<uint64_t> items;

    Container() :
      Component(Type::CONTAINER)
    {}
  };

  //! Can be consumed
  struct Consumer : Component
  {
    Consumer() :
      Component(Type::CONSUMER)
    {}
  };

  //! Applies an effect
  struct Effect : Component
  {
    Effect() :
      Component(Type::EFFECT)
    {}
  };

  //! Door
  struct Door : Component
  {
    bool is_open;

    Door(bool is_open) :
      Component(Type::DOOR)
      , is_open(is_open)
    {}
  };

  //! Determines weapon type
  struct Weapon : Component
  {
    Weapon() :
      Component(Type::WEAPON)
    {}
  };

  //! Stairs
  struct Stair : Component
  {
    Stair() :
      Component(Type::STAIR)
    {}
  };

  //! Can be read
  struct Read : Component
  {
    Read() :
      Component(Type::READ)
    {}
  };

  //! Can fly
  struct Fly : Component
  {
    Fly() :
      Component(Type::FLY)
    {}
  };

  //! Contains pathing information
  struct Path : Component
  {
    Path() :
      Component(Type::PATH)
    {}
  };

  //! Can talk or be talked to
  struct Talk : Component
  {
    Talk() :
      Component(Type::TALK)
    {}
  };

  //! Name
  struct Name : Component
  {
    std::string text;

    Name(const std::string& text) :
      Component(Type::NAME)
      , text(text)
    {}
  };

  //! Description
  struct Description : Component
  {
    std::string text;

    Description(const std::string& text) :
      Component(Type::DESCRIPTION)
      , text(text)
    {}
  };

  //! Determines rarity of an entity
  struct Rarity : Component
  {
    //! Rarity levels enum
    enum Level
    {
      COMMON = 0, /*!< Common entity */
      RARE,       /*!< Unusual, rare entity */
      LEGENDARY   /*!< One of a kind! */
    };

    Rarity() :
      Component(Type::RARITY)
    {}
  };

  struct Solid : Component
  {
    Solid() :
      Component(Type::SOLID)
    {}
  };

  struct Cloud : Component
  {
    Cloud() :
      Component(Type::CLOUD)
    {}
  };

  //! Mana component, used to keep track of mana for magic
  struct Mana : public Component
  {
    uint16_t max_mana; /*! Maximum hp */
    uint16_t cur_mana; /*! Current hp */

    Mana(uint16_t max_mana) :
      Component(Type::MANA)
      , max_mana(max_mana)
      , cur_mana(max_mana)
    {}
  };

  struct Dropable : Component
  {
    Dropable() :
      Component(Type::DROPABLE)
    {}
  };

  struct Picker : Component
  {
    Picker() :
      Component(Type::PICKER)
    {}
  };

  struct Dropper : Component
  {
    Dropper() :
      Component(Type::DROPPER)
    {}
  };

  struct Sphere : Component
  {
    enum SphereType
    {
      LIFE,
      DEATH,
      CREATION,
      DESTRUCTION,
      CHAOS,
      BALANCE,
      MAGIC,
      CRAFTING,
      SMITHING,
      WAR,
      PEACE,
      FERTILITY,
      DISEASE
    };

    SphereType type;

    Sphere(SphereType type) :
      Component(Type::SPHERE)
      , type(type)
    {}
  };

  struct Deity : Component
  {
    Deity() :
      Component(Type::DEITY)
    {}
  };

  typedef std::shared_ptr<Component> TComponentPtr; /*! Shared_ptr to Component type declaration */
  
  DEPRECATED extern bool hasComponent(uint16_t cbits, Type t);
  DEPRECATED extern TComponentPtr createNew(Type componentType);
}

#endif /* INCLUDE_COMPONENTS_H_ */

