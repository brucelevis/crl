#include <algorithm>
#include <functional>
#include <sstream>

#include "logger.h"
#include "ecs.h"
#include "map.h"

#include "generators/mapgenerator.h"

/* static */ uint64_t ECS::current_max_id = 0;

ECS::ECS() :
	player_id(0)
  , active_camera(0)
{

}

ECS::~ECS()
{
}

void ECS::init()
{
	MapGeneratorConfig config;

	config.min_width  = 35;
	config.max_width  = 40;
	config.min_height = 20;
	config.max_height = 24;
	config.max_bsp_recursion = 16;
	config.type       = MapGeneratorConfig::DUNGEON;

	MapGenerator mgen(config);

	map_ptr = mgen.generate();
}

void ECS::cleanUp()
{
	Logger::Instance()->logLine("ECS cleanup");

	if(map_ptr) map_ptr->cleanUp();
}

/*const std::vector<uint64_t>& EntityContainer::getEntities() const
{
	return entities;
}*/

void ECS::destroyEntity(uint64_t id)
{
	std::stringstream sstream;
    sstream << "destroying entity " << id;
    Logger::Instance()->logLine(Logger::Level::LDEBUG, sstream.str());

    if(!entityExists(id)) return;

	auto components = getComponents(id);

	for(auto comp : components)
	{
		if(comp.second) removeComponent(id, comp.first);
	}
	
	auto pr = std::equal_range(std::begin(entities), std::end(entities), id);
    entities.erase(pr.first, pr.second);
	
	if(pr.first != pr.second)
	{
		reusable_ids.push_back(id);
	}
	
	auto ekm_iter = entity_key_map.find(id);
	if(ekm_iter != entity_key_map.end())
	{
		entity_key_map.erase(ekm_iter);
	}
}

uint64_t ECS::createEntity()
{
	std::stringstream sstream;
    sstream << "creating entity";
    Logger::Instance()->logLine(Logger::Level::LDEBUG, sstream.str());
	
	uint64_t new_id = getId();
	
	entities.push_back(new_id);
	
	std::sort(entities.begin(), entities.end());
	
	sstream.str("");
	sstream.clear();
    sstream << "created entity with id: " << new_id;
    Logger::Instance()->logLine(Logger::Level::LDEBUG, sstream.str());
	
	return new_id;
}

void ECS::sendSystemMessage(ISystem::Type type, SystemMessage::TMessagePtr message)
{
	system_messages[type].push_back(message);
}

const std::vector<SystemMessage::TMessagePtr>& ECS::getMessages(ISystem::Type type)
{
	return system_messages[type];
}

uint64_t ECS::getId() 
{
	uint64_t new_id = 0;
	
	if(reusable_ids.size() > 0)
	{
		new_id = reusable_ids.back();
		reusable_ids.pop_back();
	}
	else
	{
		new_id = ++current_max_id;
	}		
	
	return new_id;
}

void ECS::registerComponent(uint64_t entity, Component::TComponentPtr component)
{
	if(!component || !entityExists(entity)) return;

    std::stringstream sstream;
    sstream << "adding componenttype " << static_cast<uint64_t>(component->type) << " for entity " << entity << "...";
    Logger::Instance()->logLine(Logger::Level::LDEBUG, sstream.str());

    if(hasComponent(entity, component->type))
    {
        Logger::Instance()->logLine(Logger::Level::LDEBUG, "already registered!"); 
        return;
    }

    entity_key_map[entity] |= static_cast<uint64_t>(component->type);
    entity_component_map[entity][component->type] = component;
    component_entity_map[component->type].push_back(entity);
	
	checkSystemInterests(entity);
}

void ECS::removeComponent(uint64_t entity, Component::Type componentType)
{
	if(!entityExists(entity)) return;

    std::stringstream sstream;
    sstream << "removing componenttype " << static_cast<uint64_t>(componentType) << " for entity " << entity << "...";
    Logger::Instance()->logLine(Logger::Level::LDEBUG, sstream.str());

    if(!hasComponent(entity, componentType))
    {
        Logger::Instance()->logLine(Logger::Level::LDEBUG, "not registered!"); 
        return;
    } 

    entity_key_map[entity] &= ~(static_cast<uint64_t>(componentType));
    entity_component_map[entity][componentType].reset();

    //TODO: can this be O(1)?
    auto entity_index =
    		std::find(component_entity_map[componentType].begin(),
    				  component_entity_map[componentType].end(), entity);

    if(entity_index != component_entity_map[componentType].end())
    {
    	component_entity_map[componentType].erase(entity_index);
    }
	
	checkSystemInterests(entity);
}

uint64_t ECS::getEntityKey(uint64_t entity) const
{
	auto iter = entity_key_map.find(entity);
	if(iter != entity_key_map.end())
	{
		return iter->second;
	}
	return 0;
}

void ECS::setPlayerId(uint64_t id)
{
	if(std::find(entities.begin(), entities.end(), id) != entities.end())
	{
		player_id = id;
	}
}

const uint64_t ECS::getPlayerId() const
{
	return player_id;
}

void ECS::setActiveCamera(uint64_t id)
{
	if(std::find(entities.begin(), entities.end(), id) != entities.end())
	{
		active_camera = id;
	}
}


const uint64_t ECS::getActiveCamera() const
{
	return active_camera;
}

std::shared_ptr<Map> ECS::getMap() const
{
	return map_ptr;
}

std::map<Component::Type, Component::TComponentPtr>& ECS::getComponents(uint64_t entity)
{
	return entity_component_map[entity];
}

bool ECS::entityExists(uint64_t entity) const
{
	return std::find(entities.begin(), entities.end(), entity) != entities.end();
}

void ECS::update(float delta)
{
	for(auto system : systems)
	{
		if(system)
		{
			system->update(this, delta);
		}
	}

	auto iter = system_messages.begin();
	for(system_messages.begin(); iter != system_messages.end(); ++iter)
	{
		iter->second.clear();
	}
}

void ECS::checkSystemInterests(uint64_t entity)
{
	// get entity key map
	uint64_t ekm = getEntityKey(entity);
	// if zero ignore
	if(ekm == 0) return;
	
	for(auto system : systems)
	{
		if(!system) continue;
		
		//check if entity key map is interesting to the system
		if((system->get_interest_mask() & ekm) > 0)
		{
			system->addInterest(entity);
		}
		else
		{
			system->removeInterest(entity);
		}
	}
}

const std::vector<uint64_t>& ECS::getEntitiesWithComponent(Component::Type componentType)
{
	return component_entity_map[componentType];
}

bool ECS::hasComponent(uint64_t entity, Component::Type type)
{
	return (entity_key_map[entity] & static_cast<uint64_t>(type)) > 0;
}

