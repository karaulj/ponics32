/*
 * test_IEmergency.cpp
 *
 *  Created on: Apr 13, 2021
 *      Author: Jacob
 */

#include "unity.h"

#include "IEmergency.h"
#include "GrowBedOverflow.h"
#include "Switch.h"
#include "actuators.h"
#include "status.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"

#define FLOAT_SWITCH_PIN		25


TEST_CASE("detect switch trigger (5 sec delay)", "[GROW_BED_OVERFLOW][jumper needed]")
{
	GrowBedOverflow* gbo1 = new GrowBedOverflow("test grow bed overflow",
												(gpio_num_t)FLOAT_SWITCH_PIN,
												GPIO_INTR_POSEDGE,
												GPIO_PULLUP_ENABLE,
												GPIO_PULLDOWN_DISABLE);

	int triggered = 0;
	for (int i=0; i<500; i++)
	{
		if (gbo1->getState() == EMERGENCY_TRUE)
		{
			triggered = 1;
			break;
		}
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	delete gbo1;
	TEST_ASSERT_EQUAL(1, triggered);
}


TEST_CASE("water pump shuts off on overflow (5 sec delay)", "[GROW_BED_OVERFLOW][jumper needed][GROW_BED_WATER_PUMP]")
{
	// turn on grow bed water pump
	growBedWaterPump = new Switch(GROW_BED_WATER_PUMP_NAME,
										(gpio_num_t)GROW_BED_WATER_PUMP_PIN);
	TEST_ASSERT_EQUAL(ACTUATOR_OFF, growBedWaterPump->getState());

	TEST_ASSERT_EQUAL(STATUS_OK, growBedWaterPump->turnOn());
	TEST_ASSERT_EQUAL(ACTUATOR_ON, growBedWaterPump->getState());

	GrowBedOverflow* gbo1 = new GrowBedOverflow("test grow bed overflow",
												(gpio_num_t)FLOAT_SWITCH_PIN,
												GPIO_INTR_POSEDGE,
												GPIO_PULLUP_ENABLE,
												GPIO_PULLDOWN_DISABLE);

	// check for trigger
	int triggered = 0;
	for (int i=0; i<500; i++)
	{
		if (gbo1->getState() == EMERGENCY_TRUE)
		{
			triggered = 1;
			gbo1->emergencyTriggeredCallback();
			TEST_ASSERT_EQUAL(0, growBedWaterPump->getDriveVal());
			TEST_ASSERT_EQUAL(ACTUATOR_OFF, growBedWaterPump->getState());
			break;
		}
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}

	delete gbo1;
	delete growBedWaterPump;
	TEST_ASSERT_EQUAL(1, triggered);
}

