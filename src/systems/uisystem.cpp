/*
 * uisystem.cpp
 *
 *  Created on: Dec 30, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
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
	IConsole::Instance()->put_string(41, 10, "Test string");
}




