/*
 * ISensor.cpp
 *
 *  Created on: Mar 31, 2021
 *      Author: Jacob
 */

#include "ISensor.h"

#include "stdint.h"

#include "esp_log.h"

static const char *TAG = "ISensor";


uint32_t ISensor::sensorCnt = 0;


uint32_t ISensor::count() {
	return ISensor::sensorCnt;
}


sensor_state_t ISensor::getState()
{
	ESP_LOGE(TAG, "ISensor getState() method not implemented");
	return SENSOR_NOT_IMPL;
}


float ISensor::getReading()
{
	ESP_LOGE(TAG, "ISensor getReading() method not implemented");
	return (float)SENSOR_ERR_OCCURRED;
}
