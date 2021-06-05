/*
 * emergencies.cpp
 *
 *  Created on: Mar 29, 2021
 *      Author: Jacob
 */

#include "emergencies.h"

#include "GrowBedOverflow.h"
#include "hal/gpio_types.h"

#include "esp_log.h"

static const char *TAG = "emergencies";


/*
// Tank overflow emergency: float switch that detects grow bed overflow
IEmergency* tankOverflowEmergency = new GrowBedOverflow(TANK_OVERFLOW_EMERGENCY_NAME,
															(uint8_t)TANK_OVERFLOW_FLOAT_SWITCH_PIN,
															GPIO_INTR_POSEDGE,
															GPIO_PULLUP_ENABLE,
															GPIO_PULLDOWN_DISABLE);
*/
/*
 * All emergencies defined here.
 */
/*
IEmergency* allEmergencies[] = {
		tankOverflowEmergency
};
*/
//IEmergency* allEmergencies = NULL;
//IEmergency* tankOverflowEmergency = NULL;

IEmergency* allEmergencies;
IEmergency* tankOverflowEmergency;



void emergenciesInit()
{
	ESP_LOGI(TAG, "initializing emergencies");
	tankOverflowEmergency = new GrowBedOverflow(TANK_OVERFLOW_EMERGENCY_NAME,
																(gpio_num_t)TANK_OVERFLOW_FLOAT_SWITCH_PIN,
																GPIO_INTR_POSEDGE,
																GPIO_PULLUP_ENABLE,
																GPIO_PULLDOWN_DISABLE);

	allEmergencies = (IEmergency*)malloc(sizeof(IEmergency)*IEmergency::count());
	allEmergencies = {
			tankOverflowEmergency
	};
	ESP_LOGI(TAG, "emergencies initialized");
}
