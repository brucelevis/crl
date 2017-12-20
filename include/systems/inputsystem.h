/*
 * inputsystem.h
 *
 *  Created on: Dec 10, 2017
 *      Author: joshua
 */

#ifndef INCLUDE_INPUTSYSTEM_H_
#define INCLUDE_INPUTSYSTEM_H_

#include <cstdint>

#include "systems/systemmessages.h"
#include "systems/isystem.h"

class ECS;

class InputSystem : public ISystem
{
public:
	InputSystem();
    ~InputSystem();

    virtual void update(ECS* ecs, float delta) /* = 0 */;

private:
    void handleMessage(SystemMessage::TMessagePtr message, uint64_t entity, ECS* ecs);
};


#endif /* INCLUDE_INPUTSYSTEM_H_ */
