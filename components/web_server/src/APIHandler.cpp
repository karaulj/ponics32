/*
 * WebServerAPI.cpp
 *
 *  Created on: Apr 7, 2021
 *      Author: Jacob
 */

#include "queues.h"
#include "sensors.h"
#include "actuators.h"
#include "status.h"

#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_err.h"

#include <iostream>
#include <string.h>
#include "APIHandler.h"

static const char *TAG = "WebServerAPI";


APIHandler::APIHandler(httpd_handle_t serverHandle)
{
	this->server = serverHandle;
}


static esp_err_t sensor_reading_get_handler(httpd_req_t *req)
{
	ESP_LOGV(TAG, "getReading request received");

	httpd_resp_set_type(req, "text/plain");

	// get URL query
	size_t urlQueryLen = httpd_req_get_url_query_len(req);
	char urlQuery[urlQueryLen];
	httpd_req_get_url_query_str(req, urlQuery, urlQueryLen);
	ESP_LOGV(TAG, "URL query str (len): %s (%d)", urlQuery, urlQueryLen);

	// get query param sensor ID
	char idStr[SENSOR_ID_STR_LEN];
	if (httpd_query_key_value(urlQuery, API_PARAM_ID, idStr, SENSOR_ID_STR_LEN) == ESP_ERR_NOT_FOUND)
	{
		// set response to not found and send
		httpd_resp_set_status(req, "400");
		const char* respStr = "NO_SENSOR_ID";
		httpd_resp_send(req, respStr, strlen(respStr));
		ESP_LOGW(TAG, "no sensor ID specified in getReading request");
		return ESP_OK;
	}
	// last char in urlQuery gets truncated for unknown reason
	if (!strcmp(idStr, "" ) || !strcmp(idStr , " "))
	{
		httpd_resp_set_status(req, "400");
		const char* respStr = "MISSING_ID_PARAM";
		httpd_resp_send(req, respStr, strlen(respStr));
		ESP_LOGW(TAG, "sensor ID has no value in getReading request");
		return ESP_OK;
	}
	// convert id to int
	uint32_t id = std::stoi(idStr);
	uint32_t idx = getSensorIdxFromID(id);
	if (idx == ACTUATOR_IDX_NOT_FOUND)
	{
		// set response to not found and send
		httpd_resp_set_status(req, "400");
		const char* respStr = "SENSOR_NOT_FOUND";
		httpd_resp_send(req, respStr, strlen(respStr));
		ESP_LOGW(TAG, "sensor ID %u specified in getReading request was not found", id);
		return ESP_OK;
	}

	// send reading as response
	char readingStr[SENSOR_READING_STR_LEN];
	//int readingStrLen = sprintf(readingStr, "%.2f", allSensors[idx]->lastReading);
	int readingStrLen = sprintf(readingStr, "%.2f", latestSensorReadings[idx]);
	httpd_resp_send(req, readingStr, readingStrLen);
	ESP_LOGI(TAG, "getReading (id=%u) %.2f OK", id, latestSensorReadings[idx]);
	return ESP_OK;
}
esp_err_t APIHandler::registerSensorReadingURI() {
	httpd_uri_t getReadingUri;
	getReadingUri.uri = "/getReading";
	getReadingUri.method = HTTP_GET; // @suppress("Symbol is not resolved")
	getReadingUri.handler = sensor_reading_get_handler;
	ESP_LOGI(TAG, "registering sensorReading URI");
	return httpd_register_uri_handler(server, &getReadingUri);
}




static esp_err_t actuator_drive_post_handler(httpd_req_t *req)
{
	ESP_LOGV(TAG, "driveActuator request received");

	httpd_resp_set_type(req, "text/plain");
	const char* respStr;

	// get URL query params
	size_t urlQueryLen = httpd_req_get_url_query_len(req);
	char urlQuery[urlQueryLen];
	httpd_req_get_url_query_str(req, urlQuery, urlQueryLen);
	ESP_LOGV(TAG, "URL query str (len): %s (%d)", urlQuery, urlQueryLen);

	// actuator ID
	char idStr[ACTUATOR_ID_STR_LEN];
	if (httpd_query_key_value(urlQuery, API_PARAM_ID, idStr, ACTUATOR_ID_STR_LEN) == ESP_ERR_NOT_FOUND)
	{
		// set response to not found and send
		httpd_resp_set_status(req, "400");
		respStr = "NO_ACTUATOR_ID";
		httpd_resp_send(req, respStr, strlen(respStr));
		ESP_LOGW(TAG, "no actuator ID specified in driveActuator request");
		return ESP_OK;
	}
	// last char in urlQuery gets truncated for unknown reason
	if (!strcmp(idStr, "") || !strcmp(idStr, " "))
	{
		httpd_resp_set_status(req, "400");
		respStr = "MISSING_ID_PARAM";
		httpd_resp_send(req, respStr, strlen(respStr));
		ESP_LOGW(TAG, "actuator ID has no value in driveActuator request");
		return ESP_OK;
	}
	uint32_t id = std::stoi(idStr);

	// actuator drive val
	char driveValStr[ACTUATOR_DRIVE_VAL_STR_LEN];
	if (httpd_query_key_value(urlQuery, API_PARAM_DRIVE_VAL, driveValStr, ACTUATOR_DRIVE_VAL_STR_LEN) == ESP_ERR_NOT_FOUND)
	{
		// set response to not found and send
		httpd_resp_set_status(req, "400");
		respStr = "NO_ACTUATOR_DRIVE_VAL";
		httpd_resp_send(req, respStr, strlen(respStr));
		ESP_LOGW(TAG, "no actuator drive val specified in driveActuator request");
		return ESP_OK;
	}
	// same last char truncate problem
	if (!strcmp(driveValStr, "") || !strcmp(driveValStr, " "))
	{
		httpd_resp_set_status(req, "400");
		respStr = "MISSING_DRIVE_VAL_PARAM";
		httpd_resp_send(req, respStr, strlen(respStr));
		ESP_LOGW(TAG, "actuator drive val has no value in driveActuator request");
		return ESP_OK;
	}
	uint32_t driveVal = std::stoi(driveValStr);
	if (driveVal > DRIVE_VAL_MAX)
	{
		httpd_resp_set_status(req, "400");
		respStr = "DRIVE_VAL_TOO_LARGE";
		httpd_resp_send(req, respStr, strlen(respStr));
		ESP_LOGW(TAG, "actuator drive val (%u) is too large (max %u)", driveVal, DRIVE_VAL_MAX);
		return ESP_OK;
	}

	// do drive
	xQueueSend(actuatorCmdQueue, (void*) &id, 0);
	xQueueSend(actuatorCmdQueue, (void*) &driveVal, 0);

	respStr = "OK";
	httpd_resp_send(req, respStr, strlen(respStr));
	ESP_LOGI(TAG, "driveActuator (id=%u) (dV=%u) OK", id, driveVal);
	return ESP_OK;
}
esp_err_t APIHandler::registerActuatorDriveURI() {
	httpd_uri_t driveActuatorUri;
	driveActuatorUri.uri = "/driveActuator";
	driveActuatorUri.method = HTTP_POST; // @suppress("Symbol is not resolved")
	driveActuatorUri.handler = actuator_drive_post_handler;
	ESP_LOGI(TAG, "registering driveActuator URI");
	return httpd_register_uri_handler(server, &driveActuatorUri);
}




esp_err_t APIHandler::registerAllURIs() {
	// sensor reading API endpoint
	esp_err_t retS = registerSensorReadingURI();

	// actuator drive API endpoint
	esp_err_t retA = registerActuatorDriveURI();

	return (retS==ESP_OK && retA==ESP_OK) ? ESP_OK : ESP_FAIL;
}

