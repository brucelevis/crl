/*
 * movementsystem.h
 *
 *  Created on: Dec 20, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#ifndef INCLUDE_SYSTEMS_MOVEMENTSYSTEM_H_
#define INCLUDE_SYSTEMS_MOVEMENTSYSTEM_H_

#include "systems/systemmessages.h"
#include "systems/isystem.h"

class MovementSystem : public ISystem
{
public:
  MovementSystem();
    ~MovementSystem();

    virtual void update(ECS* ecs, float delta) /* = 0 */;

private:
    void handleMessage(SystemMessage::TMessagePtr message, ECS* ecs);
};


#endif /* INCLUDE_SYSTEMS_MOVEMENTSYSTEM_H_ */
