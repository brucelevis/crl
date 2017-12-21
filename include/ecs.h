/*
 * inputsystem.h
 *
 *  Created on: Dec 15, 2017
 *      Author: Joshua Lettink
 */

#ifndef INCLUDE_ECS_H_
#define INCLUDE_ECS_H_

#include <vector>
#include <map>
#include <cstdint>

#include "components.h"
#include "systems/isystem.h"
#include "systems/systemmessages.h"
#include "map.h"

/*!
* Contains and creates entities + their component keys
*/
class ECS
{
public:
	ECS();
	~ECS();
	
	//! Cleanup method
	void cleanUp();

	//! Destroys an entity
	/*!
	* \param id The ID of the entity to destroy
	*/
	void destroyEntity(uint64_t id);
	
	//! Creates a new entity
	/*!
	* \return The ID of the newly created entity
	*/
	uint64_t createEntity();
	
	//! Registers a component with an entity
	/*!
	* \param entity The entity ID to create the component for
	* \param componentType The type of component to create
	*/
	void registerComponent(uint64_t entity, Component::TComponentPtr component);
	
	//! Removec a component from an entity
	/*!
	* \param entity The entity ID to remove the component for
	* \param componentType The type of component to remove
	*/
    void removeComponent(uint64_t entity, Component::Type componentType);
	
	//! Get the key of the entity (all registered components)
	/*!
	* \param entity The entity ID to get the key for
	* \return The component key of the entity
	*/
	//uint64_t getKey(uint64_t entity);
	
	// Adds a system 
	/*!
	* \tparam T The system type to add
	*/
	template <typename T>
	void addSystem()
	{
		systems.push_back(std::make_shared<T>());
		
		//check interests for each entity
		for(uint64_t entity : entities)
		{
			checkSystemInterests(entity);
		}

		//! sorts the systems (based on enum type value) \sa ISystem::Type
		std::sort(systems.begin(), systems.end(),
			[] (const std::shared_ptr<ISystem>& a, const std::shared_ptr<ISystem>& b)
			{
				return (uint32_t)a->getType() < (uint32_t)b->getType();
			});
	}
	
	//! Updates the ECS
	/*!
	* \param delta Deltatime since last update
	*/
	void update(float delta);

	//! Get keymap of entity
	/*!
	* \param entity The entity ID to get the keymap for
	*/
	uint64_t getEntityKey(uint64_t entity) const;
	
	//! Gets all components of a specific entity
	/*!
	* \param entity ID of entity to get components for
	*/
	std::map<Component::Type, Component::TComponentPtr>& getComponents(uint64_t entity);
	
	template<typename T>
	std::shared_ptr<T> getComponent(uint64_t entity, Component::Type type)
	{
		auto component = entity_component_map[entity][type];
		return std::static_pointer_cast<T>(component);
	}

	//! Sends a message to other systems
	/*!
	 * \param type The system type the message is for (None for all)
	 * \param message The message to send
	 */
	void sendSystemMessage(ISystem::Type type, SystemMessage::TMessagePtr message);

	//! Gets all messages for s specified system type
	/*!
	 * \param type The type to get the messages for
	 * \return The messages for the intended system
	 */
	const std::vector<SystemMessage::TMessagePtr>& getMessages(ISystem::Type type);

	//! Sets the player entity for easy acces
	/*!
	 * \param id The ID of the player entity
	 */
	void setPlayerId(uint64_t id);

	//! Gets the player entity ID
	/*!
	 * \return The entity ID of the player
	 */
	const uint64_t getPlayerId() const;

	//! Returns an std::shared_ptr to the current map
	/*!
	 * \return An shared_ptr to the current map
	 */
	std::shared_ptr<Map> getMap() const;

	//! Determine if an entity exists
	/*!
	 * \return True if the entity exists, otherwise false
	 */
	bool entityExists(uint64_t entity) const;
private:
	static uint64_t current_max_id; /*! Current max id used */

	std::vector<uint64_t> entities; /*! List of entities */
	std::vector<uint64_t> reusable_ids; /*! List of id's that can be reused when creating a new entity */
	std::map<uint64_t, uint64_t> entity_key_map; /*! Entity -> Key mapping */
	std::map<uint64_t, std::map<Component::Type, Component::TComponentPtr> > entity_component_map; /*! Maps entities to components */

	std::map<ISystem::Type, std::vector<SystemMessage::TMessagePtr>> system_messages; /*! Messages sent between systems */
	std::vector<std::shared_ptr<ISystem>> systems; /*! List of systems */
	
	std::shared_ptr<Map> map_ptr; /*! shared_ptr to current map object used by game */


	uint64_t player_id; /*! Player ID kept for easy access */

	//! Creates a new ID
	/*!
	* \return The newly created ID
	*/
	uint64_t getId();
	
	//! Checks all systems for interest and adds/removes entity accordingly
	/*!
	* \param entity The entity ID to check interest for
	*/
	void checkSystemInterests(uint64_t entity);
};

#endif /* INCLUDE_ECS_H_ */

