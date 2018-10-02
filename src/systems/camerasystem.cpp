/*
 * camerasystem.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#include "systems/camerasystem.h"

#include "components.h"
#include "ecs.h"

CameraSystem::CameraSystem() :
    ISystem(Component::Type::POSITION | Component::Type::CAMERA | Component::Type::TARGET
        , ISystem::Type::CAMERA)
{

}

CameraSystem::~CameraSystem()
{

}

/* virtual */ void CameraSystem::update(ECS* ecs, float delta) /* = 0 */
{
  auto map = ecs->getMap();
  if(!map) return;

  for(uint64_t entity : interested_entities)
  {
    auto p_comp = ecs->getComponent<Component::Position>(entity, Component::Type::POSITION);
    auto c_comp = ecs->getComponent<Component::Camera>  (entity, Component::Type::CAMERA);
    auto t_comp = ecs->getComponent<Component::Target>  (entity, Component::Type::TARGET);

    if(!p_comp || !c_comp || !t_comp || !ecs->entityExists(t_comp->target)) continue;

    auto t_p_comp = ecs->getComponent<Component::Position>(t_comp->target, Component::Type::POSITION);

    if(!t_p_comp) continue;

    p_comp->x = t_p_comp->x;
    p_comp->y = t_p_comp->y;

    uint16_t half_view_w = c_comp->view_width  / 2;
    uint16_t half_view_h = c_comp->view_height / 2;

    // if view width > x it means we go out of bounds (x < 0) when rendering
    if(p_comp->x < half_view_w) p_comp->x = half_view_w;

    // if x > map width - view width it means we go out of bounds to the right
    // ensure we do not
    else if(p_comp->x >= map->getWidth() - half_view_w)
        p_comp->x = map->getWidth() - half_view_w - 1;

    // if view height > y it means we go out of bounds (y < 0) when rendering
    if(p_comp->y < half_view_h) p_comp->y = half_view_h;

    // if y > map height - view height it means we go out of bounds to the bottom
    // ensure we do not
    else if(p_comp->y >= map->getHeight() - half_view_h)
        p_comp->y = map->getHeight() - half_view_h - 1;

    // determine offsets
    if(p_comp->x > map->getWidth() - half_view_w)
      c_comp->x_offset = p_comp->x - half_view_w;
    else if(p_comp->x > half_view_w)
      c_comp->x_offset = p_comp->x - half_view_w;
    else
      c_comp->x_offset = 0;

    if(p_comp->y > map->getHeight() - half_view_h)
      c_comp->y_offset = p_comp->y - half_view_h;
    else if(p_comp->y > half_view_h)
      c_comp->y_offset = p_comp->y - half_view_h;
    else
      c_comp->y_offset = 0;
  }
}


