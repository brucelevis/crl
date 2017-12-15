#include <exception>
#include <iostream>
#include <cstdint>

#include "BearLibTerminal.h"

#include "ecs.h"
#include "components.h"
#include "systems/systems.h"
#include "logger.h"

#include "tiles.h"
#include "map.h"

#include "app.h"

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

	if(!terminal_open())
	{
		throw std::runtime_error("error initializing terminal");
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
		throw std::runtime_error("error setting terminal settings");
	}
}

void Application::cleanup()
{
	Logger::Instance()->logLine("performing cleanup");
	terminal_close();
}

void Application::update()
{
	ecs.sendSystemMessage(ISystem::Type::INPUT
			, SystemMessage::TMessagePtr(
					new SystemMessage::InputMessage(
						last_key_pressed
					  , terminal_check (TK_SHIFT)
					  , terminal_check (TK_CONTROL)
					  , terminal_check (TK_ALT)
					)
			)
	);
}

void Application::renderFrame()
{
	//Clear whole terminal for now
	terminal_clear();

	map.render();
	ecs.update(1.0f);

	terminal_refresh();
}

void Application::mainloop()
{
	

	
	uint64_t entity = ecs.createEntity();
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Position(2, 2)));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Render( 0xFFFFFFFF, '@')));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Input()));

	ecs.addSystem<RenderSystem>();
	ecs.addSystem<InputSystem>();
	
	entity = ecs.createEntity();
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Position(11, 11)));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Render( 0xFF00FFFF, '!')));
	
	Tiles::createDefinition(1, Tiles::Flags::BLOCKING,    0xFFFFFFFF, '#');
	Tiles::createDefinition(2, 0, 0, 0);
	Tiles::createDefinition(2, Tiles::Flags::TRANSPARENT, 0xFF777777, '.');

	map.init();


    while (last_key_pressed != TK_CLOSE)
	{
		update();
		renderFrame();

		terminal_refresh();

		last_key_pressed = terminal_read();
	}
}
