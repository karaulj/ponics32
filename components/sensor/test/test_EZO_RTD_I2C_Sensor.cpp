/*
 * test_EZO_RTD_I2C_Sensor.cpp
 *
 *  Created on: Apr 13, 2021
 *      Author: Jacob
 */

#include "unity.h"

#include "EZO_RTD_I2C_Sensor.h"
#include "sensors.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"


TEST_CASE("name, units, reading interval, state set after initialization", "[EZO_RTD_I2C_SENSOR]")
{
	const char* name = "test ezo rtd i2c sensor";
	const char* unitStr = "test units";
	EZO_RTD_I2C_Sensor* ezo1 = new EZO_RTD_I2C_Sensor(name, unitStr,
													DEFAULT_SENSOR_READING_INTERVAL_MS,
													(gpio_num_t)EZO_RTD_SENSOR_I2C_SDA_PIN,
													(gpio_num_t)EZO_RTD_SENSOR_I2C_SCL_PIN);

	TEST_ASSERT_EQUAL_STRING(name, ezo1->name);
	TEST_ASSERT_EQUAL_STRING(unitStr, ezo1->units);
	TEST_ASSERT_EQUAL(DEFAULT_SENSOR_READING_INTERVAL_MS, ezo1->readingIntervalMs);
	TEST_ASSERT_EQUAL(SENSOR_READY, ezo1->getState());
	delete ezo1;
}


TEST_CASE("getReading() error (i.e. disconnected) returns SENSOR_ERR_OCCURRED", "[EZO_RTD_I2C_SENSOR]")
{
	EZO_RTD_I2C_Sensor* ezo1 = new EZO_RTD_I2C_Sensor("test ezo rtd i2c sensor",
													"test units",
													DEFAULT_SENSOR_READING_INTERVAL_MS,
													(gpio_num_t)EZO_RTD_SENSOR_I2C_SDA_PIN,
													(gpio_num_t)EZO_RTD_SENSOR_I2C_SCL_PIN);

	TEST_ASSERT_EQUAL_FLOAT(SENSOR_ERR_OCCURRED, ezo1->getReading());
	delete ezo1;
}


TEST_CASE("getReading() returns reading within bounds", "[EZO_RTD_I2C_SENSOR][i2c]")
{
	EZO_RTD_I2C_Sensor* ezo1 = new EZO_RTD_I2C_Sensor("test ezo rtd i2c sensor",
													"test units",
													DEFAULT_SENSOR_READING_INTERVAL_MS,
													(gpio_num_t)EZO_RTD_SENSOR_I2C_SDA_PIN,
													(gpio_num_t)EZO_RTD_SENSOR_I2C_SCL_PIN);

	float tempRangeF = (RTD_MAX_TEMP_F - (RTD_MIN_TEMP_F>0 ? RTD_MIN_TEMP_F : -RTD_MIN_TEMP_F));
	float midVal = RTD_MAX_TEMP_F-(tempRangeF/2.0);
	TEST_ASSERT_FLOAT_WITHIN(tempRangeF/2.0, midVal, ezo1->getReading());
	delete ezo1;
}
