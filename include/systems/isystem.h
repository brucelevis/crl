#pragma once

#include <cstdint>
#include <vector>
#include <algorithm>

class ECS;

class ISystem
{
public:
	enum Type
	{
		NONE = 0,
		INPUT,
		RENDER
	};

	//! Constructor
	/*!
	* \param interest_mask Mask of components that the system will be interested in
	* \param type The type of the system
	*/
    ISystem(uint64_t interest_mask, Type type) :
    	interest_mask(interest_mask)
	  , type(type)
	{}
    virtual ~ISystem() {}

	//! Steps the system
	/*!
	* \param delta Deltatime since last update
	*/
    virtual void update(ECS* ecs, float delta) = 0;
	
	//! Gets the interest mask of this system
	/*!
	* \return The interest mask of this system
	*/
	inline uint64_t get_interest_mask() const { return interest_mask; }
	
protected:
	uint64_t interest_mask; //! The mask of components that the system is interested in
	Type type; //! Type of the system
	
	std::vector<uint64_t> interested_entities; //! List of entities that the system is interested is

public:
	//! Adds an interesting entity to the list
	/*!
	* \param entity The entity to add
	*/
	inline virtual void addInterest(uint64_t entity) {
		interested_entities.push_back(entity);
		std::sort(interested_entities.begin(), interested_entities.end());
	}
	
	//! Removes an entity that the system is no longer interested in
	/*!
	* \param entity The entity to remove
	*/
	inline virtual void removeInterest(uint64_t entity) {
		auto pr = 
			std::equal_range(std::begin(interested_entities)
			               , std::end(interested_entities)
						   , entity);
						   
		interested_entities.erase(pr.first, pr.second);
	}

	//! Get the system type
	/*!
	 * \return The systems type
	 */
	Type getType() const { return type; }

	bool operator < (const ISystem& sys) const
	{
		return (type < sys.type);
	}
};