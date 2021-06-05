/*
 * test_ISensor.cpp
 *
 *  Created on: Apr 13, 2021
 *      Author: Jacob
 */

#include "unity.h"

#include "ISensor.h"


TEST_CASE("count() increments & id assigned automatically", "[ISENSOR]")
{
	ISensor* is1 = new ISensor;
	TEST_ASSERT_EQUAL(0, is1->id);
	TEST_ASSERT_EQUAL(1, ISensor::count());

	ISensor* is2 = new ISensor;
	TEST_ASSERT_EQUAL(1, is2->id);
	TEST_ASSERT_EQUAL(2, ISensor::count());

	delete is1;
	delete is2;
}


TEST_CASE("not implemented functions return expected types", "[ISENSOR]")
{
	ISensor* is1 = new ISensor;
	TEST_ASSERT_EQUAL(SENSOR_NOT_IMPL, is1->getState());
	TEST_ASSERT_EQUAL_FLOAT((float)SENSOR_ERR_OCCURRED, is1->getReading());
	delete is1;
}



