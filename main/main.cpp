#include "settings.h"
#include "tasks.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "esp_log.h"

static const char *TAG = "main";


extern "C" void app_main(void);

void app_main(void)
{
#ifdef CREATE_EMERGENCY_TASK
	xTaskCreate(&emergencyDetectionTask, EMERGENCY_DETECTION_TASK, 4096, NULL, 24, NULL);	// max priority
#endif

#ifdef CREATE_SENSOR_TASK
	xTaskCreate(&sensorDataCollectionTask, SENSOR_DATA_COLLECTION_TASK, 4096, NULL, 12, NULL);
#endif

#ifdef CREATE_ACTUATOR_TASK
	xTaskCreate(&actuatorControlTask, ACTUATOR_CONTROL_TASK, 4096, NULL, 12, NULL);
#endif

#ifdef CREATE_WEB_SERVER_TASK
	xTaskCreate(&WebServerTask, WEB_SERVER_TASK, 8192, NULL, tskIDLE_PRIORITY, NULL);
#endif
}

