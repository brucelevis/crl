/*
 * historygenerator.h
 *
 *  Created on: Jan 14, 2018
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#ifndef INCLUDE_GENERATORS_HISTORYGENERATOR_H_
#define INCLUDE_GENERATORS_HISTORYGENERATOR_H_

#include <cstdint>

class ECS;

class HistoryGenerator
{
public:
  HistoryGenerator();
  ~HistoryGenerator();

  void generate(ECS* ecs);

private:
  enum States
  {
    ST_VOID,
    ST_DEITY_CREATION,
    ST_HISTORY_STEP,
    ST_UNIVERSE_CREATION,
    ST_DONE
  };

  /*
   *
   *     Initial state: Void -> Deity_creation
    Deity_creation -> History_step || Universe_creation
    History_step -> History_step || Universe_creation (if no universe yet) || Done (if universe plus some additional history (x steps))
    Universe_creation -> History_step
    Done -> Done
   */

  std::vector<uint64_t> entities;

  void generateUniverse(ECS* ecs);
  void step(ECS* ecs);
};


#endif /* INCLUDE_GENERATORS_HISTORYGENERATOR_H_ */
