#include "systems/rendersystem.h"

#include "console/iconsole.h"
#include "components.h"
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
	auto map = ecs->getMap();
	if(!map) return;

	uint64_t active_camera = ecs->getActiveCamera();
	if(!ecs->entityExists(active_camera)) return;

	//get camera stuff
	auto c_p_comp = ecs->getComponent<Component::Position>(active_camera, Component::Type::POSITION);
	auto c_c_comp = ecs->getComponent<Component::Camera>  (active_camera, Component::Type::CAMERA);

	if(!c_p_comp || !c_c_comp) return;

	std::map<uint16_t, std::vector<uint64_t>> render_entities;

	for(uint64_t entity : interested_entities)
	{
		auto p_comp = ecs->getComponent<Component::Position>(entity, Component::Type::POSITION);
		auto r_comp = ecs->getComponent<Component::Render>(entity, Component::Type::RENDER);

		if(!p_comp || !r_comp) continue;

		if(p_comp && map->isVisible(p_comp->x, p_comp->y))
		{
			// No need to render if we are out of bounds
			if(p_comp->x < c_p_comp->x - c_c_comp->view_width  ||
			   p_comp->x > c_p_comp->x + c_c_comp->view_width  ||
			   p_comp->y < c_p_comp->y - c_c_comp->view_height ||
			   p_comp->y > c_p_comp->y + c_c_comp->view_height)
			{
				continue;
			}

			render_entities[r_comp->layer].push_back(entity);
		}

	}

	for(auto entities : render_entities)
	{
		for(auto r_entity : entities.second)
		{
			auto p_comp = ecs->getComponent<Component::Position>(r_entity, Component::Type::POSITION);
			auto r_comp = ecs->getComponent<Component::Render>  (r_entity, Component::Type::RENDER);


			IConsole::Instance()->set_color(r_comp->color);
			IConsole::Instance()->set_char(
					p_comp->x - c_c_comp->x_offset,
					p_comp->y - c_c_comp->y_offset,
					r_comp->glyph);
		}
	}
}
