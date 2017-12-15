#include "rendersystem.h"
#include "components.h"
#include "BearLibTerminal.h"
#include "ecs.h"

RenderSystem::RenderSystem() :
    ISystem(Component::Type::POSITION | Component::Type::RENDER
    		, ISystem::Type::RENDER)
{

}

RenderSystem::~RenderSystem()
{

}

/* virtual */ void RenderSystem::update(ECS* ecs, float delta) /* = 0 */
{
	for(uint64_t entity : interested_entities)
	{
		auto components = ecs->getComponents(entity);
		auto p_comp = ecs->getComponent<Component::Position>(entity, Component::Type::POSITION);
			//std::static_pointer_cast<Component::Position>(components[Component::Type::POSITION]);
			
		auto r_comp =
			std::static_pointer_cast<Component::Render>(components[Component::Type::RENDER]);

		terminal_color(r_comp->color);
		terminal_put(p_comp->x, p_comp->y, r_comp->glyph);
	}
}
