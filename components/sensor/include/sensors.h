/*
 * sensors.h
 *
 *  Created on: Mar 17, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_SENSOR_INCLUDE_SENSORS_H_
#define COMPONENTS_SENSOR_INCLUDE_SENSORS_H_

#include "ISensor.h"


// EZO RTD Sensor: detects temperature over I2C
#define EZO_RTD_SENSOR_NAME			"EZO RTD I2C Temp. Sensor"
#define EZO_RTD_SENSOR_I2C_SDA_PIN	21
#define EZO_RTD_SENSOR_I2C_SCL_PIN	22
extern ISensor* tempSensor;

/* Array of all sensors in this system */
extern ISensor* allSensors;

/* Array of latest sensor readings, matches with allSensors */
extern float* latestSensorReadings;


/* main function */
void sensorsInit();


/* helper function for API calls */
#define SENSOR_IDX_NOT_FOUND				0xFFFFFFFF
uint32_t getSensorIdxFromID(uint32_t id);


#endif /* COMPONENTS_SENSOR_INCLUDE_SENSORS_H_ */
