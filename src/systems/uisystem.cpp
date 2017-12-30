/*
 * uisystem.cpp
 *
 *  Created on: Dec 30, 2017
 *      Author: joshua
 */

#include "systems/uisystem.h"

#include "console/iconsole.h"
#include "components.h"
#include "ecs.h"

UISystem::UISystem() :
    ISystem(Component::Type::NONE
    		, ISystem::Type::UI)
{

}

UISystem::~UISystem()
{

}

/* virtual */ void UISystem::update(ECS* ecs, float delta) /* = 0 */
{
	//IConsole::Instance()->set_char()
}




