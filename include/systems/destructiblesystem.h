/*
 * destructiblesystem.h
 *
 *  Created on: Dec 22, 2017
 *      Author: joshua
 */

#ifndef INCLUDE_SYSTEMS_DESTRUCTIBLESYSTEM_H_
#define INCLUDE_SYSTEMS_DESTRUCTIBLESYSTEM_H_


#include "systems/isystem.h"

class DestructibleSystem : public ISystem
{
public:
	DestructibleSystem();
    ~DestructibleSystem();

    virtual void update(ECS* ecs, float delta) /* = 0 */;
};


#endif /* INCLUDE_SYSTEMS_DESTRUCTIBLESYSTEM_H_ */
