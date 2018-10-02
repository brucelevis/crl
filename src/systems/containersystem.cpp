/*
 * containersystem.cpp
 *
 *  Created on: Dec 30, 2017
 *      Author: Joshua Lettink
 */


#include "systems/containersystem.h"

#include "console/iconsole.h"
#include "components.h"
#include "ecs.h"

ContainerSystem::ContainerSystem() :
    ISystem(Component::Type::CONTAINER
        , ISystem::Type::CONTAINER)
{

}

ContainerSystem::~ContainerSystem()
{

}

void ContainerSystem::store(SystemMessage::TMessagePtr message, ECS* ecs)
{
  auto mmessage = std::static_pointer_cast<SystemMessage::StoreInContainerMessage>(message);
  if(!mmessage) return;

  auto c_component = ecs->getComponent<Component::Container>(mmessage->container, Component::Type::CONTAINER);

  if(!c_component || !ecs->entityExists(mmessage->entity)) return;

  // We make sure the entity is removed for use in other places
  // No position means no rendering or pickups by any other system
  ecs->removeComponent(mmessage->entity, Component::Type::POSITION);

  c_component->items.push_back(mmessage->entity);
}

void ContainerSystem::pickup(SystemMessage::TMessagePtr message, ECS* ecs)
{
  auto mmessage = std::static_pointer_cast<SystemMessage::PickUpMessage>(message);
  if(!mmessage) return;

  auto c_component = ecs->getComponent<Component::Container>(mmessage->picker, Component::Type::CONTAINER);

  // When picking actually make sure it is pickable
  if( !c_component ||
    !ecs->hasComponent(mmessage->picker, Component::Type::PICKER) ||
    !ecs->entityExists(mmessage->pickable) ||
    !ecs->hasComponent(mmessage->pickable, Component::Type::PICKABLE) ||
    !ecs->hasComponent(mmessage->pickable, Component::Type::POSITION))
    return;

  auto p_comp   = ecs->getComponent<Component::Position> (mmessage->picker,   Component::Type::POSITION);
  auto p_p_comp = ecs->getComponent<Component::Position> (mmessage->pickable, Component::Type::POSITION);

  if( !p_comp   ||
    !p_p_comp ||
    (p_comp->x != p_p_comp->x || p_comp->y != p_p_comp->y)) //make sure x,y of picker == x,y of pickable
    return;

  // We make sure the entity is removed for use in other places
  // No position means no rendering or pickups by any other system
  ecs->removeComponent(mmessage->pickable, Component::Type::POSITION);

  c_component->items.push_back(mmessage->pickable);
}

void ContainerSystem::remove(SystemMessage::TMessagePtr message, ECS* ecs)
{
  auto mmessage = std::static_pointer_cast<SystemMessage::RemoveFromContainerMessage>(message);
  if(!mmessage) return;

  auto c_component = ecs->getComponent<Component::Container>(mmessage->container, Component::Type::CONTAINER);

  if(!c_component) return;

  auto iter = std::find(c_component->items.begin(), c_component->items.end(), mmessage->entity);
  if(iter != c_component->items.end())
  {
    c_component->items.erase(iter);
  }
}

void ContainerSystem::drop(SystemMessage::TMessagePtr message, ECS* ecs)
{
  auto mmessage = std::static_pointer_cast<SystemMessage::DropMessage>(message);
  if(!mmessage) return;

  auto c_component = ecs->getComponent<Component::Container>(mmessage->dropper, Component::Type::CONTAINER);
  auto p_comp    = ecs->getComponent<Component::Position> (mmessage->dropper, Component::Type::POSITION);

  // make sure we can actually drop it
  if( !c_component ||
    !p_comp ||
    !ecs->hasComponent(mmessage->dropper, Component::Type::DROPPER) ||
    !ecs->entityExists(mmessage->dropable) ||
    !ecs->hasComponent(mmessage->dropable, Component::Type::DROPABLE))
    return;

  // remove it from the container and drop it
  auto iter = std::find(c_component->items.begin(), c_component->items.end(), mmessage->dropable);
  if(iter != c_component->items.end())
  {
    c_component->items.erase(iter);

    // Create a position component for the dropped entity
    ecs->registerComponent(
        mmessage->dropable,
        Component::TComponentPtr(new Component::Position(p_comp->x, p_comp->y))
    );
  }
}

void ContainerSystem::handleMessage(SystemMessage::TMessagePtr message, ECS* ecs)
{
  auto map = ecs->getMap();
  if(!map) return;

  switch(message->type)
  {
  case SystemMessage::STORETOCONTAINER:
    store(message, ecs);
    break;
  case SystemMessage::REMOVEFROMCONTAINER:
    remove(message, ecs);
    break;
  case SystemMessage::PICKUP:
    pickup(message, ecs);
    break;
  case SystemMessage::DROP:
    drop(message, ecs);
    break;
  default:
    break;
  }
}

/* virtual */ void ContainerSystem::update(ECS* ecs, float delta) /* = 0 */
{
  auto messages = ecs->getMessages(type);

  for(auto message : messages)
  {
    handleMessage(message, ecs);
  }
}

