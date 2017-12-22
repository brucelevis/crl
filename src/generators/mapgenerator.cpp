/*
 * mapgenerator.cpp
 *
 *  Created on: Dec 21, 2017
 *      Author: joshua
 */

#include <vector>

#include "generators/mapgenerator.h"
#include "map.h"
#include "randomgenerator.h"

MapGenerator::MapGenerator(const MapGeneratorConfig& config) :
	config(config)
{

}

void MapGenerator::split_bsp(std::vector<BSPNode*>& leafs, BSPNode* node, uint8_t level /* = 1 */) const
{
	if(node == nullptr) return;
	if(level >= config.max_bsp_recursion)
	{
		node->is_leaf = true;
		leafs.push_back(node);
		return;
	}

	auto rng = RandomGenerator::Instance();

	float split_range = 0.2f;
	// if current node is < size * 2 we cannot split anymore
	bool min_horizontal_reached = node->x1 - node->x0 < config.min_room_width * 2;
	bool min_vertical_reached   = node->y1 - node->y0 < config.min_room_width * 2;

	// we really cannot split in any way
	if(min_horizontal_reached && min_vertical_reached)
	{
		node->is_leaf = true;
		leafs.push_back(node);
		return;
	}


	bool horizontal_split = rng->randBool(50, true);

	// we want to split horizontally, but we make sure we can!
	if(horizontal_split && !min_vertical_reached)
	{
		// split bounds (if we go higher or lower a new room is too small)
		uint16_t min_y  = node->y0 + config.min_room_height;
		uint16_t max_y  = node->y1 - config.min_room_height;

		if(min_y > max_y)
		{
			node->is_leaf = true;
			leafs.push_back(node);
			return;
		}

		uint16_t diff   = max_y - min_y;
		uint16_t rand_y = 0;

		if(diff == 0) // split in center if diff == 0
		{
			rand_y = min_y;
		}
		else
		{
			// Determined split range is based on the provided split_range
			// Example: split_range of 0.3 means a value between 0.3 and 0.7
			// for determining splitting position within the size of diff
			rand_y = min_y + rng->randBetween(
					(uint16_t)(diff * split_range)
				  , (uint16_t)(diff * (1.0f - split_range)));
		}

		node->left  = new BSPNode(node->x0, node->y0, node->x1, rand_y,   node);
		node->right = new BSPNode(node->x0, rand_y,   node->x1, node->y1, node);

		node->left->sister  = node->right;
		node->right->sister = node->left;
	}
	else
	{
		// split bounds (if we go higher or lower a new room is too small)
		uint16_t min_x  = node->x0 + config.min_room_width;
		uint16_t max_x  = node->x1 - config.min_room_width;

		if(min_x > max_x)
		{
			node->is_leaf = true;
			leafs.push_back(node);
			return;
		}

		uint16_t diff   = max_x - min_x;
		uint16_t rand_x = 0;

		if(diff == 0) // split in center if diff == 0
		{
			rand_x = min_x;
		}
		else
		{
			// Determined split range is based on the provided split_range
			// Example: split_range of 0.3 means a value between 0.3 and 0.7
			// for determining splitting position within the size of diff
			rand_x = min_x + rng->randBetween(
					(uint16_t)(diff * split_range)
				  , (uint16_t)(diff * (1.0f - split_range)));
		}

		node->left  = new BSPNode(node->x0, node->y0, rand_x,   node->y1, node);
		node->right = new BSPNode(rand_x,   node->y0, node->x1, node->y1, node);

		node->left->sister  = node->right;
		node->right->sister = node->left;
	}

	if(node->left)  split_bsp(leafs, node->left,  level + 1);
	if(node->right) split_bsp(leafs, node->right, level + 1);
}

std::shared_ptr<Map> MapGenerator::generate() const
{
	auto rng = RandomGenerator::Instance();

	uint16_t width  = rng->randBetween(config.min_width,  config.max_width,  true);
	uint16_t height = rng->randBetween(config.min_height, config.max_height, true);

	std::shared_ptr<Map> new_map = std::make_shared<Map>(width, height);

	new_map->init();

	std::vector<BSPNode*> leafs;
	BSPNode root (0, 0, width - 1, height - 1, nullptr);
	split_bsp(leafs, &root);

	for(auto node : leafs)
	{
		for(int x = node->x0; x < node->x1; ++x)
		{
			for(int y = node->y0; y < node->y1; ++y)
			{
				if(x == node->x0 || x == node->x1 -1 || y == node->y0 || y == node->y1 -1)
				{
					new_map->tiles[x][y] = 1;
				}
				else
				{
					new_map->tiles[x][y] = 2;
				}
			}
		}

		// Continue going up
		while(node && node->sister)
		{
			uint16_t node_center_x   = node->x0 + (node->x1 - node->x0) / 2;
			uint16_t node_center_y   = node->y0 + (node->y1 - node->y0) / 2;

			uint16_t sister_center_x = node->sister->x0 + (node->sister->x1 - node->sister->x0) / 2;
			uint16_t sister_center_y = node->sister->y0 + (node->sister->y1 - node->sister->y0) / 2;

			uint16_t start_x = node_center_x <= sister_center_x ? node_center_x : sister_center_x;
			uint16_t start_y = node_center_y <= sister_center_y ? node_center_y : sister_center_y;
			uint16_t end_x   = node_center_x >= sister_center_x ? node_center_x : sister_center_x;
			uint16_t end_y   = node_center_y >= sister_center_y ? node_center_y : sister_center_y;

			for(uint16_t x = start_x; x <= end_x; ++x)
			{
				for(uint16_t y = start_y; y <= end_y; ++y)
				{
					new_map->tiles[x][y] = 2;
				}
			}

			node = node->parent;
		}
	}

	leafs.clear();

	for(uint16_t x = 0; x < width; ++x)
	{
		for(uint16_t y = 0; y < height; ++y)
		{
			new_map->visibility_map[x][y] = Map::Visibility::VISIBLE;
		}
	}


	return new_map;
}

