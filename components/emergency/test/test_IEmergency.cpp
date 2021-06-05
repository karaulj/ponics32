/*
 * test_IEmergency.cpp
 *
 *  Created on: Apr 13, 2021
 *      Author: Jacob
 */

#include "unity.h"

#include "IEmergency.h"


TEST_CASE("count() increments & id assigned automatically", "[IEMERGENCY]")
{
	IEmergency* ie1 = new IEmergency;
	TEST_ASSERT_EQUAL(0, ie1->id);
	TEST_ASSERT_EQUAL(1, IEmergency::count());

	IEmergency* ie2 = new IEmergency;
	TEST_ASSERT_EQUAL(1, ie2->id);
	TEST_ASSERT_EQUAL(2, IEmergency::count());

	delete ie1;
	delete ie2;
}


TEST_CASE("not implemented functions return expected types", "[IEMERGENCY]")
{
	IEmergency* ie1 = new IEmergency;
	TEST_ASSERT_EQUAL(EMERGENCY_TRUE, ie1->getState());
	delete ie1;
}


