/*
 * containersystem.h
 *
 *  Created on: Dec 30, 2017
 *      Author: joshua
 */

#ifndef INCLUDE_SYSTEMS_CONTAINERSYSTEM_H_
#define INCLUDE_SYSTEMS_CONTAINERSYSTEM_H_

#include "systems/isystem.h"

class ContainerSystem : public ISystem
{
public:
	ContainerSystem();
    ~ContainerSystem();

    virtual void update(ECS* ecs, float delta) /* = 0 */;
};


#endif /* INCLUDE_SYSTEMS_CONTAINERSYSTEM_H_ */
