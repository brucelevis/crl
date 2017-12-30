/*
 * destructiblesystem.h
 *
 *  Created on: Dec 22, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#ifndef INCLUDE_SYSTEMS_DESTRUCTIBLESYSTEM_H_
#define INCLUDE_SYSTEMS_DESTRUCTIBLESYSTEM_H_

#include "systems/systemmessages.h"
#include "systems/isystem.h"

class DestructibleSystem : public ISystem
{
public:
	DestructibleSystem();
    ~DestructibleSystem();

    virtual void update(ECS* ecs, float delta) /* = 0 */;

private:
    void handleMessage(SystemMessage::TMessagePtr message, ECS* ecs);
};


#endif /* INCLUDE_SYSTEMS_DESTRUCTIBLESYSTEM_H_ */
