/*
 * randomgenerator.h
 *
 *  Created on: Dec 21, 2017
 *      Author: joshua
 */

#ifndef INCLUDE_RANDOMGENERATOR_H_
#define INCLUDE_RANDOMGENERATOR_H_

#include <cstdint>
#include <memory>
#include <random>

//! Randomgenerator class
/*!
 * This class is used for global RNG stuff
 * It can be re-used and provides a base for determinism
 */
class RandomGenerator
{
public:
	//! Creates a new instance with the specified seed
	/*!
	 * \param seed The seed to use (at least 64 bit)
	 */
	static void create(uint_least64_t seed);

	//! Gets the Singleton instance
	/*!
	 * \return A shared_ptr to the instance
	 */
	static std::shared_ptr<RandomGenerator>& Instance();

	//! Get the seed that was used to create the RNG
	/*!
	 * \return The seed used to create the RNG
	 */
	uint_least64_t getSeed() const;

	int   randDelta(bool use_map_seed = false);
	int   randBetween(int min, int max, bool use_map_seed = false);
	float randFloat(float min, float max, bool use_map_seed = false);
	bool  randBool(uint8_t chance = 50, bool use_map_seed = false);
private:
	static bool created; /*! True if create was called, otherwise false */
	static std::shared_ptr<RandomGenerator> random_generator; /*! shared_ptr to RNG instance */

	uint_least64_t seed; 		/*! Seed used by RNG */
	uint32_t 	   map_seed; 	/*! Seed used to generate maps */
	uint32_t       global_seed; /*! Seed used for other stuff (e.g. AI) */

	std::mt19937 rand_engine_map; 	 /*! Mersenne twister 32 bit engine for map RNG */
	std::mt19937 rand_engine_global; /*! Mersenne twister 32 bit engine for global RNG */

	//! Ctor
	/*!
	 * \param seed The seed to feed the RNG with
	 */
	RandomGenerator(uint_least64_t seed);
};


#endif /* INCLUDE_RANDOMGENERATOR_H_ */
