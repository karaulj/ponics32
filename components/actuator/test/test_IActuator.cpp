/*
 * test_IActuator.cpp
 *
 *  Created on: Apr 12, 2021
 *      Author: Jacob
 */

#include "unity.h"
#include "IActuator.h"
//#include "TestActuator.h"

//IActuator* ta1;
//IActuator* ta2;
TEST_CASE("count() increments & id assigned automatically", "[IACTUATOR]")
{
	IActuator* ia1 = new IActuator;
	TEST_ASSERT_EQUAL(0, ia1->id);
	TEST_ASSERT_EQUAL(1, IActuator::count());

	IActuator* ia2 = new IActuator;
	TEST_ASSERT_EQUAL(1, ia2->id);
	TEST_ASSERT_EQUAL(2, IActuator::count());

	delete ia1;
	delete ia2;
}


TEST_CASE("not implemented functions return expected types", "[IACTUATOR]")
{
	IActuator* ia1 = new IActuator;
	TEST_ASSERT_EQUAL(ACTUATOR_OFF,  ia1->getState());
	TEST_ASSERT_EQUAL(0,  ia1->getDriveVal());
	TEST_ASSERT_EQUAL(STATUS_FAIL,  ia1->drive(1));
	TEST_ASSERT_EQUAL(STATUS_FAIL,  ia1->turnOn());
	TEST_ASSERT_EQUAL(STATUS_FAIL,  ia1->turnOff());
	delete ia1;
}

/*
TEST_CASE("test class TestActuator methods return expected types", "[IACTUATOR]")
{
	IActuator* ta1 = new TestActuator("test actuator #1");
	TEST_ASSERT_EQUAL(ACTUATOR_ON, ta1->getState());
	TEST_ASSERT_EQUAL((drive_val_t)0, ta1->getDriveVal());
	TEST_ASSERT_EQUAL(STATUS_OK, ta1->drive(0));
	TEST_ASSERT_EQUAL(STATUS_OK, ta1->turnOn());
	TEST_ASSERT_EQUAL(STATUS_OK, ta1->turnOff());
	delete ta1;
}
*/
