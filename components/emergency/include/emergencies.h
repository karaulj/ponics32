/*
 * emergencies.h
 *
 *  Created on: Mar 29, 2021
 *      Author: Jacob
 */

#ifndef MAIN_EMERGENCIES_H_
#define MAIN_EMERGENCIES_H_

#include "IEmergency.h"
#include "GrowBedOverflow.h"

// all user-defined emergencies. emergenciesInit() sets this value
extern IEmergency* allEmergencies;


#define TANK_OVERFLOW_EMERGENCY_NAME	"Tank Overflow Emergency"
#define TANK_OVERFLOW_FLOAT_SWITCH_PIN	25
extern IEmergency* tankOverflowEmergency;


void emergenciesInit();


#endif /* MAIN_EMERGENCIES_H_ */
