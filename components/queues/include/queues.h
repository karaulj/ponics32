/*
 * queues.h
 *
 *  Created on: Apr 2, 2021
 *      Author: Jacob
 */

#ifndef MAIN_QUEUES_QUEUES_H_
#define MAIN_QUEUES_QUEUES_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

/*
// sensor data pipeline
#define SENSOR_DATA_QUEUE_WAIT_MS		500	// wait this time in ms for queue to free up
#define SENSOR_DATA_QUEUE_WAIT_TICKS	SENSOR_DATA_QUEUE_WAIT_MS / portTICK_PERIOD_MS
#define SENSOR_DATA_QUEUE_MSG_SZ		2	// 1 for sensor ID, 1 for float reading
#define SENSOR_DATA_QUEUE_NUM_ITEMS		5 * SENSOR_DATA_QUEUE_MSG_SZ
#define SENSOR_DATA_QUEUE_ITEM_SZ		4	// 4 bytes for 32-bit float
extern QueueHandle_t sensorDataQueue;
*/


// actuator drive command queue
#define ACTUATOR_CMD_QUEUE_MSG_SZ		2	// 1 for actuator ID, 1 for drive value
#define ACTUATOR_CMD_QUEUE_NUM_ITEMS	5 * ACTUATOR_CMD_QUEUE_MSG_SZ
#define ACTUATOR_CMD_QUEUE_ITEM_SZ		1
extern QueueHandle_t actuatorCmdQueue;


#endif /* MAIN_QUEUES_QUEUES_H_ */
