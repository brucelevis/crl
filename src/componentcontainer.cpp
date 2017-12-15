#include "componentcontainer.h"

#include "components.h"
#include "logger.h"
#include <sstream>

/* static */  std::shared_ptr<ComponentContainer> ComponentContainer::component_container;

ComponentContainer::ComponentContainer()
{

}

ComponentContainer::~ComponentContainer()
{

}

/* static */ std::shared_ptr<ComponentContainer> ComponentContainer::Instance()
{
    if(component_container == nullptr) 
    {
        component_container = std::shared_ptr<ComponentContainer>(new ComponentContainer());
    }

    return component_container;
}

void ComponentContainer::registerComponent(uint64_t entity, Component::Type componentType)
{
    std::stringstream sstream;
    sstream << "adding componenttype " << static_cast<uint64_t>(componentType) << " for entity " << entity << "...";
    Logger::Instance()->logLine(Logger::Level::LDEBUG, sstream.str());

    uint64_t key = entity_key_map[entity];

    if(Component::hasComponent(key, componentType)) 
    {
        Logger::Instance()->logLine(Logger::Level::LDEBUG, "already registered!"); 
        return;
    }

    entity_key_map[entity] |= static_cast<uint64_t>(componentType);
    component_map[componentType][entity] = Component::createNew(componentType);
}

void ComponentContainer::removeComponent(uint64_t entity, Component::Type componentType)
{
    std::stringstream sstream;
    sstream << "removing componenttype " << static_cast<uint64_t>(componentType) << " for entity " << entity << "...";
    Logger::Instance()->logLine(Logger::Level::LDEBUG, sstream.str());

    uint64_t key = entity_key_map[entity];

    if(!Component::hasComponent(key, componentType)) 
    {
        Logger::Instance()->logLine(Logger::Level::LDEBUG, "not registered!"); 
        return;
    } 

    entity_key_map[entity] &= ~(static_cast<uint64_t>(componentType)); 
    component_map[componentType][entity].reset();
}

Component::TComponentPtr ComponentContainer::getComponent(uint64_t entity, Component::Type componentType)
{
    return nullptr;
}

uint64_t ComponentContainer::getKey(uint64_t entity)
{
    return entity_key_map[entity];
}
