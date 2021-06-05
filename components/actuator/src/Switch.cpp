/*
 * Switch.cpp
 *
 *  Created on: Mar 30, 2021
 *      Author: Jacob
 */

#include "Switch.h"

#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "esp_log.h"


static const char *TAG = "switchActuator";


Switch::Switch(const char* switchName, gpio_num_t pinNo) {
	ESP_LOGI(TAG, "initializing switch '%s' on pin %d", switchName, (int)pinNo);

	this->name = switchName;
	this->pin = pinNo;
	this->useBooleanDrive = true;
	// switch pin setup
	gpio_config_t pinConfig;
	pinConfig.pin_bit_mask = 1UL<<pin;
	pinConfig.mode = GPIO_MODE_OUTPUT;
	pinConfig.pull_up_en = GPIO_PULLUP_DISABLE;
	pinConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
	pinConfig.intr_type = GPIO_INTR_DISABLE;
	gpio_config(&pinConfig);

	this->turnOff();
}


Switch::~Switch()
{
	this->turnOff();
	gpio_reset_pin((gpio_num_t)this->pin);
}


status_t Switch::drive(drive_val_t driveValue) {
	state = ACTUATOR_BUSY;

	uint8_t boolDriveVal = driveValue>0;
	if ( gpio_set_level((gpio_num_t)pin, boolDriveVal) == ESP_OK)
	{
		ESP_LOGV(TAG, "drove switch '%s' with value %u", this->name, boolDriveVal);
		this->driveVal = boolDriveVal;
		state = boolDriveVal ? ACTUATOR_ON : ACTUATOR_OFF;
		return STATUS_OK;
	}
	else
	{
		ESP_LOGE(TAG, "drive on switch '%s' failed", name);
		state = ACTUATOR_ERR;
		return STATUS_FAIL;
	}

}


status_t Switch::turnOn() {
	return drive(1);
}


status_t Switch::turnOff() {
	return drive(0);

}
