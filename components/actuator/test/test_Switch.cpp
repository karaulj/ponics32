/*
 * test_Switch.cpp
 *
 *  Created on: Apr 13, 2021
 *      Author: Jacob
 */

#include "unity.h"

#include "Switch.h"
#include "status.h"

#include "hal/gpio_types.h"
#include "driver/gpio.h"


#define GPIO_PIN		26

/*
 * Pin to connect to Switch output pin to test for proper functionality.
 *
 * Must put a jumper between this pin and GPIO_PIN
 */
#define TEST_INPUT_PIN	27


const gpio_config_t gpioConfig = {
			.pin_bit_mask = 1ULL<<TEST_INPUT_PIN,
			.mode = GPIO_MODE_INPUT,
			.pull_up_en = GPIO_PULLUP_DISABLE,
			.pull_down_en = GPIO_PULLDOWN_DISABLE,
			.intr_type = GPIO_INTR_DISABLE
	};


TEST_CASE("Switch on and off", "[SWITCH][jumper needed]")
{
	gpio_config(&gpioConfig);
	Switch* sw = new Switch("test switch", (gpio_num_t)GPIO_PIN);
	TEST_ASSERT_EQUAL(0, gpio_get_level((gpio_num_t)TEST_INPUT_PIN));	// off initially

	TEST_ASSERT_EQUAL(STATUS_OK, sw->turnOn());
	TEST_ASSERT_EQUAL(1, gpio_get_level((gpio_num_t)TEST_INPUT_PIN));
	TEST_ASSERT_EQUAL(1, sw->getDriveVal());
	TEST_ASSERT_EQUAL(ACTUATOR_ON, sw->getState());

	TEST_ASSERT_EQUAL(STATUS_OK, sw->turnOff());
	TEST_ASSERT_EQUAL(0, gpio_get_level((gpio_num_t)TEST_INPUT_PIN));
	TEST_ASSERT_EQUAL(0, sw->getDriveVal());
	TEST_ASSERT_EQUAL(ACTUATOR_OFF, sw->getState());

	delete sw;
	gpio_reset_pin((gpio_num_t)TEST_INPUT_PIN);
}


TEST_CASE("Driving with any value above 0 sets a drive value of 1", "[SWITCH]")
{
	gpio_config(&gpioConfig);
	Switch* sw = new Switch("test switch", (gpio_num_t)GPIO_PIN);

	TEST_ASSERT_EQUAL(STATUS_OK, sw->drive(777));		// not 0 or 1 drive val
	TEST_ASSERT_EQUAL(1, sw->getDriveVal());

	delete sw;
}
