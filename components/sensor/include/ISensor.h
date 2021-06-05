/*
 * ISensor.h
 *
 *  Created on: Mar 30, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_SENSOR_INCLUDE_ISENSOR_H_
#define COMPONENTS_SENSOR_INCLUDE_ISENSOR_H_

#include "stdint.h"

#define DEFAULT_SENSOR_NAME						"sensor"
#define DEFAULT_SENSOR_READING_INTERVAL_MS		5000		//5s

#define SENSOR_ERR_OCCURRED						0xFFFFFFFF


typedef enum {
	SENSOR_READY = 0,
	SENSOR_OFF,
	SENSOR_BUSY,
	SENSOR_NOT_IMPL
} sensor_state_t;


class ISensor {
private:
	static uint32_t sensorCnt;
protected:
	sensor_state_t state = SENSOR_OFF;
public:
	const char* name;
	const char* units;
	const uint32_t id = sensorCnt++;

	static uint32_t count();

	float lastReading = -1.0;
	uint32_t readingIntervalMs;

	virtual ~ISensor() {sensorCnt--;}

	virtual sensor_state_t getState();
	virtual float getReading();
};

#endif /* COMPONENTS_SENSOR_INCLUDE_ISENSOR_H_ */
