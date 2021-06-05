/*
 * WebServerAPI.h
 *
 *  Created on: Apr 7, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_WEB_SERVER_INCLUDE_APIHANDLER_H_
#define COMPONENTS_WEB_SERVER_INCLUDE_APIHANDLER_H_

#include "esp_http_server.h"

#include "esp_err.h"


//float* latestSensorReadings;

#define API_PARAM_ID				"id"
#define API_PARAM_DRIVE_VAL			"driveVal"

#define ACTUATOR_ID_STR_LEN			20
#define ACTUATOR_DRIVE_VAL_STR_LEN	20

#define SENSOR_ID_STR_LEN			20
#define SENSOR_READING_STR_LEN		20


class APIHandler {
private:
	httpd_handle_t server;
	esp_err_t registerActuatorDriveURI();
	esp_err_t registerSensorReadingURI();
public:
	APIHandler(httpd_handle_t serverHandle);
	esp_err_t registerAllURIs();
};


#endif /* COMPONENTS_WEB_SERVER_INCLUDE_APIHANDLER_H_ */
