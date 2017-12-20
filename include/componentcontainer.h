/*
 * inputsystem.h
 *
 *  Created on: Dec 15, 2017
 *      Author: Joshua Lettink
 */

#ifndef INCLUDE_COMPONENTCONTAINER_H_
#define INCLUDE_COMPONENTCONTAINER_H_

#include <vector>
#include <map>
#include <cstdint>
#include <memory>

#include "components.h"

class ComponentContainer
{
public:
    ~ComponentContainer();

    //! Gets the ComponentContainer instance
    /*!
     * \return The instance of the componentcontainer
     */
    static std::shared_ptr<ComponentContainer> Instance();

    //! Register a new component in the container
    /*!
     * Registers a component of the specified type for the specified entity
     *
     * \sa Component::Type
     * \param entity The entity ID to create the component for
     * \param componetType The type of component to create
     */
    void registerComponent(uint64_t entity, Component::Type componentType);

    //! Removes a component from the container
    /*!
     * Registers a component of the specified type for the specified entity
     *
     * \sa Component::Type
     * \param entity The entity ID to create the component for
     * \param componetType The type of component to create
     */
    void removeComponent(uint64_t entity, Component::Type componentType);

    Component::TComponentPtr getComponent(uint64_t entity, Component::Type componentType);

    uint64_t getKey(uint64_t entity);

private:
    typedef std::map<uint64_t, std::shared_ptr<Component::Component>> EntityComponentMap;

    std::map<uint64_t, uint64_t> entity_key_map;
    std::map<Component::Type, EntityComponentMap> component_map;

    static std::shared_ptr<ComponentContainer> component_container;

    ComponentContainer();
};

#endif /* INCLUDE_COMPONENTCONTAINER_H_ */
