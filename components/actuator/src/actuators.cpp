/*
 * actuators.cpp
 *
 *  Created on: Mar 30, 2021
 *      Author: Jacob
 */

#include "actuators.h"
#include "IActuator.h"

#include "Switch.h"

#include "hal/gpio_types.h"
#include "esp_log.h"

static const char *TAG = "actuators";


// Grow bed water pump actuator
IActuator* growBedWaterPump;
IActuator* allActuators;


void actuatorsInit()
{
	ESP_LOGI(TAG, "initializing actuators");

	growBedWaterPump = new Switch(GROW_BED_WATER_PUMP_NAME,
									(gpio_num_t)GROW_BED_WATER_PUMP_PIN);

	allActuators = {
		growBedWaterPump
	};

	ESP_LOGI(TAG, "actuators initialized");
}


uint32_t getActuatorIdxFromID(uint32_t id)
{
	for (uint32_t i=0; i<IActuator::count(); i++)
	{
		if (allActuators[i].id == id)
		{
			return i;
		}
	}
	return ACTUATOR_IDX_NOT_FOUND;
}
