/*
 * attacksystem.h
 *
 *  Created on: Dec 22, 2017
 *      Author: Joshua Lettink
 */

#ifndef INCLUDE_SYSTEMS_ATTACKSYSTEM_H_
#define INCLUDE_SYSTEMS_ATTACKSYSTEM_H_

#include "systems/systemmessages.h"
#include "systems/isystem.h"

class AttackSystem : public ISystem
{
public:
	AttackSystem();
    ~AttackSystem();

    virtual void update(ECS* ecs, float delta) /* = 0 */;

private:
    void handleMessage(SystemMessage::TMessagePtr message, ECS* ecs);
};


#endif /* INCLUDE_SYSTEMS_ATTACKSYSTEM_H_ */
