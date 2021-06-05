/*
 * EZO_RTD_I2C_Sensor.cpp
 *
 *  Created on: Mar 30, 2021
 *      Author: Jacob
 */

#include "EZO_RTD_I2C_Sensor.h"

#include "ISensor.h"

#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "hal/gpio_types.h"
//#include "sdkconfig.h"
#include "esp_log.h"

static const char *TAG = "EZO_RTD_I2C_Sensor";


EZO_RTD_I2C_Sensor::EZO_RTD_I2C_Sensor(const char* sensorName, const char* unitStr, uint32_t readingInterval, gpio_num_t sdaPin, gpio_num_t sclPin)
{
	ESP_LOGI(TAG, "Initializing sensor '%s' with ID %u", sensorName, id);

	name = sensorName;
	units = unitStr;
	readingIntervalMs = readingInterval;

	// I2C init
	i2c_config_t i2cConfig;
	i2cConfig.mode = I2C_MODE_MASTER;
	i2cConfig.scl_io_num = sclPin;
	i2cConfig.sda_io_num = sdaPin;
	i2cConfig.scl_pullup_en = GPIO_PULLUP_ENABLE;
	i2cConfig.sda_pullup_en = GPIO_PULLUP_ENABLE;
	i2cConfig.master.clk_speed = I2C_MASTER_FREQ_HZ;
	ESP_ERROR_CHECK(i2c_param_config(I2C_PORT_NUM, &i2cConfig));
	ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT_NUM, I2C_MODE_MASTER, 0, 0, 0)); // @suppress("Invalid arguments")
	state = SENSOR_READY;
}


EZO_RTD_I2C_Sensor::~EZO_RTD_I2C_Sensor() {
	i2c_driver_delete(I2C_PORT_NUM);
}


sensor_state_t EZO_RTD_I2C_Sensor::getState() {
	return state;
}


float EZO_RTD_I2C_Sensor::getReading()
{
	float retVal = SENSOR_ERR_OCCURRED;
	if (getState() == SENSOR_READY)
	{
		state = SENSOR_BUSY;

		// send READ command
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (RTD_I2C_ADDR_DEFAULT<<1) | I2C_MASTER_WRITE, true);
		i2c_master_write_byte(cmd, RTD_I2C_Cmd_Read, true);
		i2c_master_stop(cmd);
		i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
		i2c_cmd_link_delete(cmd);

		// processing delay
		vTaskDelay(RTD_I2C_LONG_DELAY_MS / portTICK_PERIOD_MS);

		uint8_t readBuffer[RTD_I2C_BUFFER_MAX_SZ] = { '\0' };
		uint8_t resCode = 0;
		// get response
		cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (RTD_I2C_ADDR_DEFAULT<<1) | I2C_MASTER_READ, true);
		i2c_master_read_byte(cmd, &resCode, I2C_MASTER_ACK);
		i2c_master_read(cmd, &readBuffer[0], RTD_I2C_BUFFER_MAX_SZ, I2C_MASTER_ACK);
		i2c_master_stop(cmd);
		i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
		i2c_cmd_link_delete(cmd);

		// handle response
		switch((RTD_i2cResCode)resCode) {
		case NoData:
			ESP_LOGW(TAG, "no data to send from sensor");
			break;
		case StillProcessing:
			ESP_LOGW(TAG, "still processing sensor reading");
			break;
		case SyntaxErr:
			ESP_LOGW(TAG,  "syntax error in sent command");
			break;
		case Success:
			retVal = (float)getReadingFromI2CBuf(readBuffer);
			ESP_LOGI(TAG, "got sensor '%s' reading: %f", name, retVal);
			break;
		default:
			ESP_LOGW(TAG, "unknown response code %u", resCode);
			break;
		}

		state = SENSOR_READY;
	}
	else
	{
		ESP_LOGW(TAG, "Sensor '%s' is busy; cannot get reading", name);
	}
	return retVal;

}


double EZO_RTD_I2C_Sensor::getReadingFromI2CBuf(uint8_t* buf)
{
	uint8_t readingArr[RTD_I2C_READ_BUFFER_MAX_SZ] = {'0'};
	uint8_t readingArrIdx = 0, bufIdx = 0;
	// all values after data string are null
	while (buf[bufIdx] != '\0')
	{
		if ((buf[bufIdx] >= '0' && buf[bufIdx] <= '9') || buf[bufIdx] == '-' || buf[bufIdx] == '.')
		{
			readingArr[readingArrIdx++] = buf[bufIdx];
		}
		bufIdx++;
	}
	return atof((const char*)readingArr);
}


void EZO_RTD_I2C_Sensor::sendCmd(const char* cmd, uint8_t resCode, uint8_t* readBuffer, bool useLongDelay)
{
	//uint8_t readBuffer[RTD_I2C_BUFFER_MAX_SZ] = {'\0'};
	//uint8_t resCode = 0;

	//return (uint8_t*)readBuffer;
}


