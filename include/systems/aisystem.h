/*
 * aisystem.h
 *
 *  Created on: Dec 20, 2017
 *      Author: Joshua Lettink
 */

#ifndef INCLUDE_SYSTEMS_AISYSTEM_H_
#define INCLUDE_SYSTEMS_AISYSTEM_H_

#include "systems/isystem.h"

class AISystem : public ISystem
{
public:
	AISystem();
    ~AISystem();

    virtual void update(ECS* ecs, float delta) /* = 0 */;

private:
    void runDumbAI			(ECS* ecs, uint64_t entity, float delta);
    void runAnimalAI		(ECS* ecs, uint64_t entity, float delta);
    void runIntelligentAI	(ECS* ecs, uint64_t entity, float delta);
    void runOmniscientAI	(ECS* ecs, uint64_t entity, float delta);
    void runPlayerAI		(ECS* ecs, uint64_t entity, float delta);
};



#endif /* INCLUDE_SYSTEMS_AISYSTEM_H_ */
