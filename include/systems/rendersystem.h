#pragma once

#include "isystem.h"

class RenderSystem : public ISystem
{
public:
    RenderSystem();
    ~RenderSystem();

    virtual void update(ECS* ecs, float delta) /* = 0 */;
};