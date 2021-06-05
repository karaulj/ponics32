/*
 * IActuator.cpp
 *
 *  Created on: Apr 1, 2021
 *      Author: Jacob
 */

#include "IActuator.h"

#include "stdint.h"
#include "esp_log.h"

static const char *TAG = "IActuator";


uint32_t IActuator::actuatorCnt = 0;


uint32_t IActuator::count()
{
	return IActuator::actuatorCnt;
}


actuator_state_t IActuator::getState()
{
	return this->state;
}


drive_val_t IActuator::getDriveVal()
{
	return this->driveVal;
}


status_t IActuator::turnOn()
{
	ESP_LOGE(TAG, "IActuator turnOn() method not implemented");
	return STATUS_FAIL;
}


status_t IActuator::turnOff()
{
	ESP_LOGE(TAG, "IActuator turnOff() method not implemented");
	return STATUS_FAIL;
}


status_t IActuator::drive(drive_val_t driveVal)
{
	ESP_LOGE(TAG, "IActuator drive() method not implemented");
	return STATUS_FAIL;
}
