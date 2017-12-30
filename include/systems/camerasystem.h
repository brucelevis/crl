/*
 * camerasystem.h
 *
 *  Created on: Dec 22, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#ifndef INCLUDE_SYSTEMS_CAMERASYSTEM_H_
#define INCLUDE_SYSTEMS_CAMERASYSTEM_H_

#include "systems/isystem.h"

class CameraSystem : public ISystem
{
public:
	CameraSystem();
    ~CameraSystem();

    virtual void update(ECS* ecs, float delta) /* = 0 */;
};



#endif /* INCLUDE_SYSTEMS_CAMERASYSTEM_H_ */
