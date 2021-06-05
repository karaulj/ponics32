/*
 * IActuator.h
 *
 *  Created on: Mar 17, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_ACTUATOR_INCLUDE_IACTUATOR_H_
#define COMPONENTS_ACTUATOR_INCLUDE_IACTUATOR_H_

#include "status.h"

#include "stdint.h"


#define DEFAULT_ACTUATOR_NAME					"actuator"
#define DRIVE_VAL_MAX							0xFFFFFFFF

typedef enum {
	ACTUATOR_ON = 0,
	ACTUATOR_OFF,
	ACTUATOR_BUSY,
	ACTUATOR_ERR,
	ACTUATOR_NOT_IMPL
} actuator_state_t;

typedef uint32_t drive_val_t;


class IActuator {
private:
	static uint32_t actuatorCnt;
protected:
	drive_val_t driveVal = 0;
	actuator_state_t state = ACTUATOR_OFF;
public:
	const char* name;
	const uint32_t id = actuatorCnt++;

	bool useBooleanDrive = false;

	static uint32_t count();
	actuator_state_t getState();
	drive_val_t getDriveVal();

	virtual ~IActuator() {actuatorCnt--;};

	virtual status_t drive(drive_val_t driveVal);
	virtual status_t turnOn();
	virtual status_t turnOff();
};


#endif /* COMPONENTS_ACTUATOR_INCLUDE_IACTUATOR_H_ */
