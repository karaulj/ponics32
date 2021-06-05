/*
 * queues.cpp
 *
 *  Created on: Apr 2, 2021
 *      Author: Jacob
 */

#include "queues.h"

#include "freertos/queue.h"

/*
#ifdef CREATE_SENSOR_TASK
QueueHandle_t sensorDataQueue = xQueueCreate(SENSOR_DATA_QUEUE_NUM_ITEMS,
											SENSOR_DATA_QUEUE_ITEM_SZ);
#endif
*/

QueueHandle_t actuatorCmdQueue = xQueueCreate(ACTUATOR_CMD_QUEUE_NUM_ITEMS,
											ACTUATOR_CMD_QUEUE_ITEM_SZ);

