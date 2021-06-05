/*
 * actuators.h
 *
 *  Created on: Mar 30, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_ACTUATOR_INCLUDE_ACTUATORS_H_
#define COMPONENTS_ACTUATOR_INCLUDE_ACTUATORS_H_

#include "IActuator.h"


/*
 * All actuators must be included here.
 */
extern IActuator* allActuators;


// Grow bed water pump actuator: pumps water from fish tank to grow bed
#define GROW_BED_WATER_PUMP_NAME	"Grow Bed Water Pump"
#define GROW_BED_WATER_PUMP_PIN		26
extern IActuator* growBedWaterPump;



void actuatorsInit();


#define ACTUATOR_IDX_NOT_FOUND				0xFFFFFFFF
uint32_t getActuatorIdxFromID(uint32_t id);


#endif /* COMPONENTS_ACTUATOR_INCLUDE_ACTUATORS_H_ */
