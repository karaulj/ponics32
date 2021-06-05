/*
 * TestActuator.h
 *
 *  Created on: Apr 13, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_ACTUATOR_TEST_TESTACTUATOR_H_
#define COMPONENTS_ACTUATOR_TEST_TESTACTUATOR_H_

#include "IActuator.h"
#include "status.h"


class TestActuator : public IActuator {
public:
	TestActuator(const char* actName) {this->name = actName;}

	actuator_state_t getState() {return ACTUATOR_ON;}
	drive_val_t getDriveVal() {return 0;}
	status_t drive(drive_val_t driveVal) {return (driveVal==0) ? STATUS_OK : STATUS_FAIL;}
	status_t turnOn() {return STATUS_OK;}
	status_t turnOff() {return STATUS_OK;}
};


#endif /* COMPONENTS_ACTUATOR_TEST_TESTACTUATOR_H_ */
