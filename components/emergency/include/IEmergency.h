/*
 * IEmergency.h
 *
 *  Created on: Mar 17, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_EMERGENCY_INCLUDE_IEMERGENCY_H_
#define COMPONENTS_EMERGENCY_INCLUDE_IEMERGENCY_H_

#include "stdint.h"

typedef enum
{
	EMERGENCY_TRUE = 0,
	EMERGENCY_FALSE = 1
} emergency_state_t;


class IEmergency
{
private:
	static uint8_t emergencyCnt;
/*
protected:
	volatile emergency_state_t state = EMERGENCY_FALSE;
*/
public:
	const char* name;
	const uint8_t id = emergencyCnt++;

	static uint8_t count();

	virtual ~IEmergency() {emergencyCnt--;}

	virtual emergency_state_t getState();
	virtual void emergencyTriggeredCallback();
};

#endif /* COMPONENTS_EMERGENCY_INCLUDE_IEMERGENCY_H_ */
