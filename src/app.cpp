#include <exception>
#include <iostream>
#include <cstdint>

#include "ecs.h"
#include "components.h"
#include "systems/systems.h"
#include "logger.h"

#include "tiles.h"
#include "map.h"

#include "app.h"
#include "console/iconsole.h"

ECS ecs;
Map map;

Application::Application() :
	last_key_pressed(0)
{
}

Application::~Application()
{
	cleanup();
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

	map.render();
	ecs.update(1.0f);

	IConsole::Instance()->refresh();
}

void Application::mainloop()
{
	uint64_t entity = ecs.createEntity();
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Position(2, 2)));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Render( IConsole::Color::WHITE, '@')));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Input()));

	ecs.addSystem<RenderSystem>();
	ecs.addSystem<InputSystem>();
	
	entity = ecs.createEntity();
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Position(11, 11)));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Render( IConsole::Color::RED, '!')));
	
	Tiles::createDefinition(1, Tiles::Flags::BLOCKING,    IConsole::Color::WHITE, '#');
	Tiles::createDefinition(2, 0, IConsole::Color::WHITE, 0);
	Tiles::createDefinition(2, Tiles::Flags::TRANSPARENT, IConsole::Color::WHITE, '.');

	map.init();


    while (!IConsole::Instance()->shouldClose())
	{
		update();
		renderFrame();

		last_key_pressed = IConsole::Instance()->getChar();
	}
}
