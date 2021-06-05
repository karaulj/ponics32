/*
 * Switch.h
 *
 * An actuator that drives a single GPIO pin.
 *
 *  Created on: Mar 30, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_ACTUATOR_INCLUDE_SWITCH_H_
#define COMPONENTS_ACTUATOR_INCLUDE_SWITCH_H_

#include "IActuator.h"
#include "status.h"

#include "hal/gpio_types.h"

#include "stdint.h"


class Switch: public IActuator {
private:
	gpio_num_t pin;
	//actuator_state_t state = ACTUATOR_OFF;
public:
	Switch(const char* switchName, gpio_num_t pinNo);
	~Switch();

	actuator_state_t getState();
	drive_val_t getDriveVal();

	status_t turnOn();
	status_t turnOff();
	status_t drive(drive_val_t driveVal);
};


#endif /* COMPONENTS_ACTUATOR_INCLUDE_SWITCH_H_ */
