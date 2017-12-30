/*
 * mapsystem.h
 *
 *  Created on: Dec 20, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
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

private:
    //! Renders the viewport for a camera
    /*!
     * \param camera The camera entity to render for
     * \param ecs The ECS that is used
     * \param delta Delta time
     */
    void renderView(uint64_t camera, ECS* ecs, float delta);
};



#endif /* INCLUDE_SYSTEMS_MAPSYSTEM_H_ */
