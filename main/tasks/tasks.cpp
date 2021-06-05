/*
 * tasks.cpp
 *
 *  Created on: Mar 18, 2021
 *      Author: Jacob
 */

#include "tasks.h"
#include "queues.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"

#include "settings.h"
#include "status.h"

#ifdef CREATE_WEB_SERVER_TASK
#include "tinypico.h"
#include "sensors.h"
#include "actuators.h"
#include "wifi_conn.h"
#include "wifi_mdns.h"
#include "APIHandler.h"
#include "WebServer.h"
#endif

#ifdef CREATE_ACTUATOR_TASK
#include "IActuator.h"
#include "actuators.h"
#endif

#ifdef CREATE_SENSOR_TASK
#include "ISensor.h"
#include "sensors.h"
#endif

#ifdef CREATE_EMERGENCY_TASK
#include "IEmergency.h"
#include "emergencies.h"
#endif


static const char *TAG = "tasks";


#ifdef CREATE_WEB_SERVER_TASK
void WebServerTask(void *pvParameter)
{
	ESP_LOGI(TAG, "start %s", WEB_SERVER_TASK);

	TinyPICO* tp = TinyPICO::getInstance();
	tp->setLEDColor(0x00, 0x00, 0x00);

	if (do_wifi_init(WIFI_SSID, WIFI_PASS) == ESP_OK)
	{
		ESP_LOGI(TAG, "connected!");
		do_mdns_init(HOSTNAME, MDNS_INSTANCE);
		ESP_LOGI(TAG, "mDNS started");
		WebServer ws;
		ws.serve();
		ESP_LOGI(TAG, "initialized web server");

		// blink LED to signal wifi connected
		for (int i=0; i<3; i++)
		{
			tp->setLEDColor(0x00, 0xFF, 0x00);
			vTaskDelay(150 / portTICK_PERIOD_MS);
			tp->setLEDColor(0x00, 0x00, 0x00);
			vTaskDelay(150 / portTICK_PERIOD_MS);
		}

		// main task loop
		while (true)
		{
			vTaskDelay(10 / portTICK_PERIOD_MS);
		}

	}
	else	// wifi connection error
	{
		// blink LED forever to signal wifi connection error
		while (true)
		{
			ESP_LOGE(TAG, "wifi connection failed");
			for (int i=0; i<3; i++)
			{
				tp->setLEDColor(0x0F, 0x00, 0x00);
				vTaskDelay(250 / portTICK_PERIOD_MS);
				tp->setLEDColor(0x00, 0x00, 0x00);
				vTaskDelay(250 / portTICK_PERIOD_MS);
			}
			vTaskDelay(500 / portTICK_PERIOD_MS);
		}
	}
}
#endif


#ifdef CREATE_ACTUATOR_TASK
void actuatorControlTask(void *pvParameter)
{
	ESP_LOGI(TAG, "start %s", ACTUATOR_CONTROL_TASK);

	actuatorsInit();

	TinyPICO* tp = TinyPICO::getInstance();

	uint32_t actuatorId = 0, driveVal = 0;
	IActuator* actuator;
	while(true)
	{
		// wait indefinitely for actuator command and drive val
		xQueueReceive(actuatorCmdQueue, &actuatorId, portMAX_DELAY);
		xQueueReceive(actuatorCmdQueue, &driveVal, portMAX_DELAY);
		ESP_LOGV(TAG, "actuatorId: %u", actuatorId);
		ESP_LOGV(TAG, "driveVal:   %u", driveVal);

		for (uint32_t i=0; i<IActuator::count(); i++)
		{
			actuator = &allActuators[i];

			if (actuatorId == actuator->id)
			{
				if ( actuator->drive(driveVal) == STATUS_OK )
				{
					ESP_LOGI(TAG, "drove actuator '%s' with val %u", actuator->name, driveVal);
					tp->setLEDColor(0x00, 0xFF, 0x00);
					vTaskDelay(100 / portTICK_PERIOD_MS);
					tp->setLEDColor(0x00, 0x00, 0x00);
				}
				else
				{
					ESP_LOGW(TAG, "failed to drive actuator '%s' with val %u", actuator->name, driveVal);
					tp->setLEDColor(0xFF, 0x00, 0x00);
					vTaskDelay(100 / portTICK_PERIOD_MS);
					tp->setLEDColor(0x00, 0x00, 0x00);
				}
			}

		}
	}
}
#endif


#ifdef CREATE_SENSOR_TASK
void sensorDataCollectionTask(void *pvParameter)
{
	ESP_LOGI(TAG, "start %s", SENSOR_DATA_COLLECTION_TASK);

	sensorsInit();

	// large value so first reading is taken immediately
	TickType_t lastTickCounts[ISensor::count()] = { 0xFFFF };
	TickType_t tickCount;
	uint32_t timeSinceLastReadingMs;

	ISensor* sensor;
	while (true)
	{

		for (int i=0; i<ISensor::count(); i++)
		{
			sensor = &allSensors[i];

			tickCount = xTaskGetTickCount();
			timeSinceLastReadingMs = (tickCount - lastTickCounts[i]) * portTICK_PERIOD_MS;

			// if sensor reading time interval has passed, take reading and post to queue
			if (timeSinceLastReadingMs >= sensor->readingIntervalMs)
			{
				float reading = sensor->getReading();
				if (reading != SENSOR_ERR_OCCURRED)
				{
					sensor->lastReading = reading;
					latestSensorReadings[i] = reading;
				}
				lastTickCounts[i] = tickCount;
			}
			vTaskDelay(10 / portTICK_PERIOD_MS);	// wait a bit for reading to be processed between sensor readings
		}

	}

}
#endif


#ifdef CREATE_EMERGENCY_TASK
void emergencyDetectionTask(void *pvParameter)
{
	ESP_LOGI(TAG, "start %s", EMERGENCY_DETECTION_TASK);

	//allEmergencies = malloc(ISensor::count());
	emergenciesInit();
	vTaskDelay(10 / portTICK_PERIOD_MS);	// feed watchdog

	IEmergency* currentEmergency;
	while (true)
	{
		for (int i=0; i<IEmergency::count(); i++)
		{
			currentEmergency = &allEmergencies[i];

			// check for emergency, callback if triggered
			if (currentEmergency->getState() == EMERGENCY_TRUE)
			{
				currentEmergency->emergencyTriggeredCallback();
				ESP_LOGI(TAG, "emergency '%s' triggered", currentEmergency->name);
			}
		}
		vTaskDelay(10 / portTICK_PERIOD_MS);	// feed watchdog
	}
}
#endif


