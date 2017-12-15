/*
 * map.h
 *
 *  Created on: Dec 10, 2017
 *      Author: joshua
 */

#ifndef INCLUDE_MAP_H_
#define INCLUDE_MAP_H_

#include <cstdint>

class Map
{
public:
	enum Visibility
	{
		INVISIBLE = 0,
		VISIBLE   = (1 << 0),
		SEEN      = (1 << 1)
	};

	Map();

	void init();
	void calculateFOV(uint16_t x, uint16_t y);
	void update(float delta);
	void render();

	bool isVisible(uint16_t x, uint16_t y) const;
	bool isSolid(uint16_t x, uint16_t y) const;
private:
	static const uint16_t width  = 10;
	static const uint16_t height = 10;

	bool initialized;

	uint16_t   tiles           [width][height];
	Visibility visibility_map  [width][height];

};



#endif /* INCLUDE_MAP_H_ */
