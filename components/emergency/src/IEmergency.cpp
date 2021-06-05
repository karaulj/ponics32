/*
 * IEmergency.cpp
 *
 *  Created on: Apr 1, 2021
 *      Author: Jacob
 */

#include "IEmergency.h"

#include "stdint.h"

#include "esp_log.h"

static const char *TAG = "IEmergency";


uint8_t IEmergency::emergencyCnt = 0;


uint8_t IEmergency::count() {
	return IEmergency::emergencyCnt;
}


emergency_state_t IEmergency::getState()
{
	ESP_LOGE(TAG, "IEmergency getState() method not implemented");
	return EMERGENCY_TRUE;
}


void IEmergency::emergencyTriggeredCallback()
{
	ESP_LOGE(TAG, "IEmergency emergencyTriggeredCallback() method not implemented");
}
