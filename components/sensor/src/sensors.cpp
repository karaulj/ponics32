/*
 * sensors.cpp
 *
 *  Created on: Mar 17, 2021
 *      Author: Jacob
 */

#include "sensors.h"

#include "ISensor.h"
#include "EZO_RTD_I2C_Sensor.h"

#include "hal/gpio_types.h"

#include "esp_log.h"

static const char *TAG = "sensors";


float* latestSensorReadings;

ISensor* tempSensor;
ISensor* allSensors;

void sensorsInit()
{
	ESP_LOGI(TAG, "initializing sensors");
	tempSensor = new EZO_RTD_I2C_Sensor(EZO_RTD_SENSOR_NAME,
										"&deg;F",	// HTML degree symbol
										DEFAULT_SENSOR_READING_INTERVAL_MS,
										(gpio_num_t)EZO_RTD_SENSOR_I2C_SDA_PIN,
										(gpio_num_t)EZO_RTD_SENSOR_I2C_SCL_PIN);

	allSensors = (ISensor*)malloc(sizeof(ISensor)*ISensor::count());
	allSensors = {
			tempSensor
	};

	// dynamically allocate array for latest sensor readings
	latestSensorReadings = (float*)calloc(ISensor::count(), sizeof(float));
	ESP_LOGI(TAG, "sensors initialized");
}


uint32_t getSensorIdxFromID(uint32_t sensorId)
{
	for (uint32_t i=0; i<ISensor::count(); i++)
	{
		if (allSensors[i].id == sensorId) return i;
	}
	return SENSOR_IDX_NOT_FOUND;
}
