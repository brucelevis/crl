/*
 * containersystem.h
 *
 *  Created on: Dec 30, 2017
 *      Author: Joshua Lettink
 */

#ifndef INCLUDE_SYSTEMS_CONTAINERSYSTEM_H_
#define INCLUDE_SYSTEMS_CONTAINERSYSTEM_H_

#include "systems/systemmessages.h"
#include "systems/isystem.h"

class ContainerSystem : public ISystem
{
public:
  ContainerSystem();
    ~ContainerSystem();

    virtual void update(ECS* ecs, float delta) /* = 0 */;

private:
    void handleMessage(SystemMessage::TMessagePtr message, ECS* ecs);

    void store  (SystemMessage::TMessagePtr message, ECS* ecs);
    void remove (SystemMessage::TMessagePtr message, ECS* ecs);
    void pickup (SystemMessage::TMessagePtr message, ECS* ecs);
    void drop (SystemMessage::TMessagePtr message, ECS* ecs);
};


#endif /* INCLUDE_SYSTEMS_CONTAINERSYSTEM_H_ */
