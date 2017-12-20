/*
 * map.h
 *
 *  Created on: Dec 10, 2017
 *      Author: joshua
 */

#ifndef INCLUDE_MAP_H_
#define INCLUDE_MAP_H_

#include <cstdint>

class ECS;

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
	void update(ECS* ecs, float delta);
	void render();

	bool isSolid(uint16_t x, uint16_t y) const;
	bool isOpaque(uint16_t x, uint16_t y) const;
	bool isVisible(uint16_t x, uint16_t y) const;
private:
	static const uint16_t width  = 40;
	static const uint16_t height = 24;

	bool initialized;

	uint16_t   tiles           [width][height];
	Visibility visibility_map  [width][height];

	void castLight(
			uint16_t x, uint16_t y,
			uint8_t radius, uint16_t row,
			float start_slope, float end_slope,
			int16_t xx, int16_t xy,
			int16_t yx, int16_t yy);
};



#endif /* INCLUDE_MAP_H_ */
