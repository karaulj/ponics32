/*
 * GrowBedOverflow.cpp
 *
 *  Created on: Mar 29, 2021
 *      Author: Jacob
 */

#include "GrowBedOverflow.h"

#include "actuators.h"

#include "stdint.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "esp_log.h"

static const char *TAG = "growBedOverflow";


static void IRAM_ATTR floatSwitchISR(void* arg)
{
	floatSwitchEmState = EMERGENCY_TRUE;
}


GrowBedOverflow::GrowBedOverflow(const char* emName,
											gpio_num_t pinNo,
											gpio_int_type_t intrType,
											gpio_pullup_t pullupType,
											gpio_pulldown_t pulldownType)
{
	ESP_LOGI(TAG, "Initializing emergency '%s' w/ pin %u", emName, pinNo);

	this->name = emName;
	this->pin = pinNo;
	// float switch pin setup
	gpio_config_t floatSwitchConfig;
	floatSwitchConfig.mode = GPIO_MODE_INPUT;
	floatSwitchConfig.pin_bit_mask = 1ULL<<pinNo;
	floatSwitchConfig.intr_type = intrType;
	floatSwitchConfig.pull_up_en = pullupType;
	floatSwitchConfig.pull_down_en = pulldownType;
	if (gpio_config(&floatSwitchConfig) == ESP_OK) {
		ESP_LOGI(TAG, "initialized pin %u", pinNo);
	} else {
		ESP_LOGW(TAG, "failed to initialize pin %u", pinNo);
	}

	// float switch pin interrupt
	esp_err_t ret = gpio_install_isr_service(0);
	if (ret == ESP_OK) {
		ESP_LOGI(TAG, "ISR service installed on pin %u", pinNo);
	} else if (ret == ESP_ERR_NO_MEM) {
		ESP_LOGW(TAG, "failed to install ISR service on pin %u: no memory", pinNo);
	} else if (ret == ESP_ERR_INVALID_STATE) {
		ESP_LOGW(TAG, "failed to install ISR service on pin %u: already installed", pinNo);
	} else if (ret == ESP_ERR_NOT_FOUND) {
		ESP_LOGW(TAG, "failed to install ISR service on pin %u: no free interrupt found", pinNo);
	} else if (ret == ESP_ERR_INVALID_ARG) {
		ESP_LOGW(TAG, "failed to install ISR service on pin %u: GPIO error", pinNo);
	} else {
		ESP_LOGW(TAG, "failed to install ISR service on pin %u: unknown reason %d", pinNo, ret);
	}

	if (gpio_isr_handler_add((gpio_num_t)pinNo, floatSwitchISR, (void*) pinNo) == ESP_OK)
	{
		ESP_LOGI(TAG, "ISR handler added on pin %u", pinNo);
	} else {
		ESP_LOGW(TAG, "failed to add ISR handler on pin %u", pinNo);
	}

}


GrowBedOverflow::~GrowBedOverflow()
{
	gpio_reset_pin(this->pin);

	gpio_intr_disable(this->pin);
	gpio_isr_handler_remove(this->pin);
	gpio_uninstall_isr_service();

}


emergency_state_t GrowBedOverflow::getState()
{
	// state is set in the ISR, only needs to be read
	return floatSwitchEmState;
}


void GrowBedOverflow::emergencyTriggeredCallback()
{
	// shut off water pump actuator
	growBedWaterPump->turnOff();

	// sound alarm as long as switch is high
	// actually, omit for now

	ESP_LOGI(TAG, "float switch '%s' triggered", name);
	floatSwitchEmState = EMERGENCY_FALSE;
}
