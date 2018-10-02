/*
 * historygenerator.cpp
 *
 *  Created on: Jan 14, 2018
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#include "randomgenerator.h"
#include "generators/historygenerator.h"
#include "ecs.h"

#include <vector>
#include <map>
#include <string>

HistoryGenerator::HistoryGenerator()
{

}

HistoryGenerator::~HistoryGenerator()
{

}

void HistoryGenerator::step(ECS* ecs)
{

}

void HistoryGenerator::generate(ECS* ecs)
{

	//std::map<uint64_t, uint64_t> spouses;
	//std::map<uint64_t, std::pair<uint64_t, uint64_t>> parents;
	//uint64_t start_year = 0;

	auto rng = RandomGenerator::Instance();
	bool only_one_initial_entity = rng->randBool(50);

	if(only_one_initial_entity)
	{
		uint64_t entity = ecs->createEntity();

		ecs->registerComponent(entity, Component::TComponentPtr(new Component::Deity()));
		ecs->registerComponent(entity, Component::TComponentPtr(new Component::Name("1")));

		entities.push_back(entity);
	}
	else
	{
		uint8_t num_entities = rng->randBetween(1, 6);

		for(uint8_t i = 0; i < num_entities; ++i)
		{
			uint64_t entity = ecs->createEntity();

			ecs->registerComponent(entity, Component::TComponentPtr(new Component::Deity()));
			ecs->registerComponent(entity, Component::TComponentPtr(new Component::Name(std::to_string(i))));

			entities.push_back(entity);
		}
	}
}

void HistoryGenerator::generateUniverse(ECS* ecs)
{

}
