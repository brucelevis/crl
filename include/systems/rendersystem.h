/*
 * inputsystem.h
 *
 *  Created on: Dec 15, 2017
 *      Author: Joshua Lettink
 */

#ifndef INCLUDE_RENDERSYSTEM_H_
#define INCLUDE_RENDERSYSTEM_H_

#include "systems/isystem.h"

class RenderSystem : public ISystem
{
public:
    RenderSystem();
    ~RenderSystem();

    virtual void update(ECS* ecs, float delta) /* = 0 */;
};

#endif /* INCLUDE_RENDERSYSTEM_H_ */
