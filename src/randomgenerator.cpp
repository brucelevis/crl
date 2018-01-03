/*
 * randomgenerator.cpp
 *
 *  Created on: Dec 21, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#include <cfloat>
#include <exception>
#include <ctime>
#include <sstream>

#include "randomgenerator.h"
#include "logger.h"

/* static */ bool RandomGenerator::created = false;

/* static */ std::shared_ptr<RandomGenerator> RandomGenerator::random_generator;

RandomGenerator::RandomGenerator(uint_least64_t seed) :
	seed(seed)
  , map_seed(static_cast<uint32_t>(seed >> 32))
  , global_seed(static_cast<uint32_t>(seed & 0x00000000FFFFFFFF))
  , rand_engine_map(map_seed)
  , rand_engine_global(global_seed)
{

}

/* static */ void RandomGenerator::create(uint_least64_t seed)
{
	random_generator = std::shared_ptr<RandomGenerator>(new RandomGenerator(seed));

	std::stringstream sstream;
	sstream << "Created RandomGenerator with main seed '" << seed
			<< "', map_seed: '" << random_generator->map_seed
			<< "' and global_seed: '" << random_generator->global_seed << "'";

	Logger::Instance()->logLine(Logger::Level::LINFO, sstream.str());

	created = true;
}

/* static */ std::shared_ptr<RandomGenerator>& RandomGenerator::Instance()
{
	if(!created)
	{
		std::time_t result = std::time(nullptr);
		uint_least64_t seed = result << 32;
		seed += result / 2;

		create(seed);
	}

	return random_generator;
}

uint_least64_t RandomGenerator::getSeed() const
{
	return seed;
}

int RandomGenerator::randDelta(bool use_map_seed /* = false */)
{
	std::uniform_int_distribution<int> uniform_dist(-1, 1);

	return uniform_dist(use_map_seed ? rand_engine_map : rand_engine_global);
}

int RandomGenerator::randBetween(int min, int max, bool use_map_seed /* = false */)
{
	if(min >= max) return min;

	 std::uniform_int_distribution<int> uniform_dist(min, max);

	 return uniform_dist(use_map_seed ? rand_engine_map : rand_engine_global);
}

float RandomGenerator::randFloat(float min, float max, bool use_map_seed /* = false */)
{
	if(min >= max) return min;

	// Nextafter is used to make max inclusive
	std::uniform_real_distribution<float> uniform_dist(min, std::nextafter(max, FLT_MAX));

	return uniform_dist(use_map_seed ? rand_engine_map : rand_engine_global);
}

bool RandomGenerator::randBool(uint8_t chance /* = 50 */, bool use_map_seed /* false */ )
{
	 std::uniform_int_distribution<uint8_t> uniform_dist(0, 100);

	 return uniform_dist(use_map_seed ? rand_engine_map : rand_engine_global) >= chance;
}



