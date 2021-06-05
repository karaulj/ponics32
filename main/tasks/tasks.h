/*
 * tasks.h
 *
 *  Created on: Mar 18, 2021
 *      Author: Jacob
 */

#ifndef MAIN_TASKS_H_
#define MAIN_TASKS_H_


const char* const BLINK_LED_TASK = "blinkLEDTask";
void blinkLEDTask(void *pvParameter);

const char* const WEB_SERVER_TASK = "webServerTask";
void WebServerTask(void *pvParameter);

const char* const ACTUATOR_CONTROL_TASK = "actuatorControlTask";
void actuatorControlTask(void* pvParameter);

const char* const SENSOR_DATA_COLLECTION_TASK = "sensorDataCollectionTask";
void sensorDataCollectionTask(void *pvParameter);

const char* const EMERGENCY_DETECTION_TASK = "emergencyDetectionTask";
void emergencyDetectionTask(void *pvParameter);


#endif /* MAIN_TASKS_H_ */
