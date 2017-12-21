/*
 * mappgenerator.h
 *
 *  Created on: Dec 21, 2017
 *      Author: joshua
 */

#ifndef INCLUDE_GENERATORS_MAPGENERATOR_H_
#define INCLUDE_GENERATORS_MAPGENERATOR_H_

#include <cstdint>
#include <memory>
#include <vector>

struct MapGeneratorConfig
{
public:
	//! Determines type of map to generate
	enum Type
	{
		RANDOM = 0, /*!< Don't care */
		CAVE,       /*!< Cavelike */
		DUNGEON,    /*!< Dungeonlike (rooms + hallways) */
		PLANES      /*!< Plane-like (large empty spaces) */
	};

	Type type; /*! Type of map to generate */

	bool create_secret_rooms;    /*! Determines if secret rooms should be created or not */
	bool include_premade_pieces; /*! Determines if prefabs should be used */

	uint16_t min_width;          /*! Minimum width of the map */
	uint16_t max_width;          /*! Maximum width of the map */
	uint16_t min_height;         /*! Minimum width of the map */
	uint16_t max_height;         /*! Maximum width of the map */

	uint16_t min_room_width;     /*! Minimum room width */
	uint16_t min_room_height;    /*! Minimum room height */

	uint8_t max_bsp_recursion;   /*! Determines max recursion if a BSP tree is used */

	bool generate_down_stairs;	 /*! Determines if down stairs should be generated */
	bool generate_up_stairs;     /*! Determines if up stairs should be generated */

	uint16_t depth_level;		 /*! Depth level, determines difficulty */

	//! Ctor
	MapGeneratorConfig() :
		type(Type::RANDOM)
	  , create_secret_rooms(true)
	  , include_premade_pieces(true)
	  , min_width(40)
	  , max_width(100)
	  , min_height(24)
	  , max_height(100)
	  , min_room_width(10)
	  , min_room_height(10)
	  , max_bsp_recursion(4)
	  , generate_down_stairs(true)
	  , generate_up_stairs(true)
	  , depth_level(0)
	{}

};

struct BSPNode {
	uint16_t x0;
	uint16_t y0;
	uint16_t x1;
	uint16_t y1;

	BSPNode* parent;
	BSPNode* left;
	BSPNode* right;

	BSPNode(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, BSPNode* parent) :
		x0(x0), y0(y0), x1(x1), y1(y1)
	  , parent(parent), left(nullptr), right(nullptr)
	{}

	~BSPNode()
	{
		if(left)  delete left;
		if(right) delete right;
	}
};


class Map;
class MapGenerator
{
public:

	MapGenerator(const MapGeneratorConfig& config);

	std::shared_ptr<Map> generate() const;
private:
	const MapGeneratorConfig& config;

	void split_bsp(std::vector<BSPNode*>& leafs, BSPNode* node, uint8_t level = 1) const;
};


#endif /* INCLUDE_GENERATORS_MAPGENERATOR_H_ */
