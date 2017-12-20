/*
 * mapsystem.h
 *
 *  Created on: Dec 20, 2017
 *      Author: joshua
 */

#ifndef INCLUDE_SYSTEMS_MAPSYSTEM_H_
#define INCLUDE_SYSTEMS_MAPSYSTEM_H_


#include "systems/isystem.h"

class MapSystem : public ISystem
{
public:
	MapSystem();
    ~MapSystem();

    virtual void update(ECS* ecs, float delta) /* = 0 */;
};



#endif /* INCLUDE_SYSTEMS_MAPSYSTEM_H_ */
