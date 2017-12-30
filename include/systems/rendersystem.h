/*
 * inputsystem.h
 *
 *  Created on: Dec 15, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#ifndef INCLUDE_RENDERSYSTEM_H_
#define INCLUDE_RENDERSYSTEM_H_

#include <cstdint>
#include "systems/isystem.h"

class RenderSystem : public ISystem
{
public:
    RenderSystem();
    ~RenderSystem();

    virtual void update(ECS* ecs, float delta) /* = 0 */;

private:
    //! Renders the viewport for a camera
    /*!
     * \param camera The camera entity to render for
     * \param ecs The ECS that is used
     */
    void renderView(uint64_t camera, ECS* ecs);
};

#endif /* INCLUDE_RENDERSYSTEM_H_ */
