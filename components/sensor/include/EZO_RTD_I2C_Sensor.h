/*
 * EZO_RTD_I2C_Sensor.h
 *
 * Driver for using Atlas Scientific's EZO RTD temperature sensor with I2C.
 *
 *  Created on: Mar 30, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_SENSOR_INCLUDE_EZO_RTD_I2C_SENSOR_H_
#define COMPONENTS_SENSOR_INCLUDE_EZO_RTD_I2C_SENSOR_H_

#include "ISensor.h"

#include "stdint.h"
//#include <iostream>

#include "driver/i2c.h"


#define I2C_MASTER_FREQ_HZ			100000
#define I2C_PORT_NUM				I2C_NUM_0

#define RTD_I2C_ADDR_DEFAULT		(uint8_t) 0x66
#define RTD_I2C_READ_BUFFER_MAX_SZ	(uint8_t) 8
#define RTD_I2C_BUFFER_MAX_SZ		(uint8_t) 40		// from datasheet

#define RTD_MAX_TEMP_F				2290.0
#define RTD_MIN_TEMP_F				-195.0

#define RTD_I2C_LONG_DELAY_MS		600UL


// first byte of I2C response indicates status
typedef enum {
	NoData 			= 255,
	StillProcessing	= 254,
	SyntaxErr		= 2,
	Success			= 1
} RTD_i2cResCode;


class EZO_RTD_I2C_Sensor: public ISensor {
private:
	const char RTD_I2C_Cmd_Read = 'R';
	double getReadingFromI2CBuf(uint8_t* buf);
	void sendCmd(const char* cmd, uint8_t resCode, uint8_t* readBuffer, bool useLongDelay);
public:
	EZO_RTD_I2C_Sensor(const char* sensorName, const char* unitStr, const uint32_t readingInterval, gpio_num_t sdaPin, gpio_num_t sclPin);
	~EZO_RTD_I2C_Sensor();

	sensor_state_t getState();
	float getReading();
};


#endif /* COMPONENTS_SENSOR_INCLUDE_EZO_RTD_I2C_SENSOR_H_ */
