/*
 * GrowBedOverflow.h
 *
 *  Created on: Mar 29, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_EMERGENCY_INCLUDE_GROW_BED_OVERFLOW_H_
#define COMPONENTS_EMERGENCY_INCLUDE_GROW_BED_OVERFLOW_H_

#include "IEmergency.h"

#include "stdint.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"


static volatile emergency_state_t floatSwitchEmState = EMERGENCY_FALSE;


class GrowBedOverflow: public IEmergency {
private:
	gpio_num_t pin;
public:
	GrowBedOverflow(const char* emName,
							gpio_num_t pinNo,
							gpio_int_type_t intrType,
							gpio_pullup_t pullupType,
							gpio_pulldown_t pulldownType);
	~GrowBedOverflow();
	emergency_state_t getState();
	void emergencyTriggeredCallback();
};


#endif /* COMPONENTS_EMERGENCY_INCLUDE_GROW_BED_OVERFLOW_H_ */
