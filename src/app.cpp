#include <exception>
#include <iostream>
#include <cstdint>

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

	ecs.init();

	uint64_t entity = ecs.createEntity();
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Position(2, 2)));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Render( IConsole::Color::WHITE, '@')));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Input()));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Movement(100)));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::AIComponent(Component::AIComponent::PLAYER)));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Attack(10)));

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
	
	ecs.setPlayerId(entity);

	// create camera that follows the player
	entity = ecs.createEntity();
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Position(0, 0)));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Camera( 80, 24 )));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Target( ecs.getPlayerId() )));
	ecs.setActiveCamera(entity);

	entity = ecs.createEntity();
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Position(11, 11)));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Render( IConsole::Color::RED, '!')));
	
	entity = ecs.createEntity();
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Position(9, 9)));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Render( IConsole::Color::BLUE, '1')));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::AIComponent(Component::AIComponent::DUMB)));
	ecs.registerComponent(entity, Component::TComponentPtr(new Component::Destructible(30)));

	Tiles::createDefinition(1, Tiles::Flags::BLOCKING,    IConsole::Color::WHITE, '#');
	Tiles::createDefinition(2, 0, IConsole::Color::WHITE, 0);
	Tiles::createDefinition(2, Tiles::Flags::TRANSPARENT, IConsole::Color::GREY, '.');

    while (!IConsole::Instance()->shouldClose())
	{
		update();
		renderFrame();

		last_key_pressed = IConsole::Instance()->getChar();
	}
}
