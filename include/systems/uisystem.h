/*
 * uisystem.h
 *
 *  Created on: Dec 30, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#ifndef INCLUDE_SYSTEMS_UISYSTEM_H_
#define INCLUDE_SYSTEMS_UISYSTEM_H_

#include "systems/isystem.h"

class UISystem : public ISystem
{
public:
	UISystem();
    ~UISystem();

    virtual void update(ECS* ecs, float delta) /* = 0 */;
};

#endif /* INCLUDE_SYSTEMS_UISYSTEM_H_ */
