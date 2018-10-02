/*
 * uisystem.cpp
 *
 *  Created on: Dec 30, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */


#include <exception>
#include <iostream>
#include <cstdint>
#include <chrono>

#include "ecs.h"
#include "components.h"
#include "systems/systems.h"
#include "logger.h"

#include "tiles.h"

#include "app.h"
#include "console/iconsole.h"

#include "randomgenerator.h"

ECS ecs;

Application::Application() :
  last_key_pressed(0)
{
}

Application::~Application()
{

}

int Application::run()
{
  initialize();
  mainloop();
  
  return 0;
}

void Application::initialize()
{
  Logger::Instance()->logLine("initializing");

  IConsole::allocateConsole(IConsole::ConsoleType::BEARLIBTERMINAL, 80, 24);
}

void Application::cleanup()
{
  Logger::Instance()->logLine("performing cleanup");
  ecs.cleanUp();
  IConsole::Instance()->cleanup();
}

void Application::update()
{
  ecs.sendSystemMessage(ISystem::Type::INPUT
      , SystemMessage::TMessagePtr(
          new SystemMessage::InputMessage(
            last_key_pressed
            , IConsole::Instance()->getShift()
            , IConsole::Instance()->getControl()
            , IConsole::Instance()->getAlt()
          )
      )
  );
}

void Application::renderFrame()
{
  //Clear whole terminal for now
  IConsole::Instance()->clscr();

  ecs.update(1.0f);

  IConsole::Instance()->refresh();
}

void Application::mainloop()
{
  #ifndef NDEBUG
    RandomGenerator::create(0xFF11FF22AA33AA44);
    auto rng = RandomGenerator::Instance();
  #endif

  Tiles::createDefinition(1, "wall_basic",  Tiles::Flags::BLOCKING,    IConsole::Color::WHITE, '#');
  Tiles::createDefinition(2, "null",        0,                         IConsole::Color::WHITE,  0 );
  Tiles::createDefinition(2, "floor_basic", Tiles::Flags::TRANSPARENT, IConsole::Color::GREY,  '.');

  ecs.init();

  uint64_t entity = ecs.createEntity();
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Position(15, 15)));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Render( IConsole::Color::WHITE, '@', 1)));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Input()));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Solid()));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Movement(100)));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::AIComponent(Component::AIComponent::PLAYER)));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Attack(10)));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Player()));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Destructible(200)));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Mana(100)));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Dropper()));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Picker()));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Container()));

  std::map<Component::Skill::SkillType, int16_t> skills;
  skills[Component::Skill::SkillType::ATK] = 10;
  skills[Component::Skill::SkillType::STR] = 10;
  skills[Component::Skill::SkillType::AGI] = 10;
  skills[Component::Skill::SkillType::INT] = 10;

  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Skill(skills)));

  ecs.addSystem<RenderSystem>();
  ecs.addSystem<InputSystem>();
  ecs.addSystem<AISystem>();
  ecs.addSystem<MovementSystem>();
  ecs.addSystem<MapSystem>();
  ecs.addSystem<CameraSystem>();
  ecs.addSystem<AttackSystem>();
  ecs.addSystem<DestructibleSystem>();
  ecs.addSystem<ContainerSystem>();
  ecs.addSystem<UISystem>();
  
  //ecs.setPlayerId(entity);

  uint64_t player_entity = entity;

  // create camera that follows the player
  entity = ecs.createEntity();
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Position(0, 0)));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Camera( 40, 20, 0, 0 )));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Target( player_entity )));
  //ecs.setActiveCamera(entity);

  entity = ecs.createEntity();
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Position(11, 11)));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Render( IConsole::Color::RED, '!')));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Pickable()));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Dropable()));
  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Name( "Testing thingy" )));
  
  for(int i = 0; i < 1; ++i)
  {
    entity = ecs.createEntity();
    ecs.registerComponent(entity, Component::TComponentPtr(new Component::Position(3 + i, 3 + i)));
    ecs.registerComponent(entity, Component::TComponentPtr(new Component::Render( IConsole::Color::BLUE, '1', 1)));
    ecs.registerComponent(entity, Component::TComponentPtr(new Component::AIComponent(Component::AIComponent::DUMB)));
    ecs.registerComponent(entity, Component::TComponentPtr(new Component::Destructible(30)));
    ecs.registerComponent(entity, Component::TComponentPtr(new Component::Attack(10)));
    ecs.registerComponent(entity, Component::TComponentPtr(new Component::Solid()));
    ecs.registerComponent(entity, Component::TComponentPtr(new Component::Target( player_entity )));
  }

//  uint64_t ai_entity = entity;
//  entity = ecs.createEntity();
//  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Position(0, 0)));
//  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Camera( 9, 9, 72, 0 )));
//  ecs.registerComponent(entity, Component::TComponentPtr(new Component::Target( ai_entity )));


  std::stringstream sstream;
  sstream << "using high_resolution_clock "  << std::endl;
  sstream << "period: "     << std::chrono::high_resolution_clock::period::num << std::endl;
  sstream << "resolution: " << 1.0 / (double)std::chrono::high_resolution_clock::period::den << "s" << std::endl;
  sstream << "steady: "     << std::boolalpha << std::chrono::high_resolution_clock::is_steady;
  Logger::Instance()->logLine(sstream.str());

    while (!IConsole::Instance()->shouldClose())
  {
      auto start = std::chrono::high_resolution_clock::now();

    update();
    renderFrame();

    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;

    std::cout << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;

    last_key_pressed = IConsole::Instance()->getChar();
  }
}
