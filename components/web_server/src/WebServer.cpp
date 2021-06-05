/*
 * WebServer.c
 *
 *  Created on: Apr 4, 2021
 *      Author: Jacob
 */

#include "stdint.h"
#include "string.h"

#include "esp_wifi.h"
#include "esp_http_server.h"
#include "esp_err.h"
#include "esp_log.h"
//#include "lwip.h"

#include "HomePageHandler.h"
#include "WebServer.h"
#include "APIHandler.h"


static const char *TAG = "WebServer";

//static httpd_handle_t* serverHandle = NULL;


//static char* homeHTMLBuf[4096];
//static uint32_t homeHTMLSz;
//static WebServer ws;


WebServer::WebServer()
{
	this->server = NULL;
}


WebServer::~WebServer()
{
	delete hph;
	delete APIh;
	httpd_stop(server);
}


/* Main function */
esp_err_t WebServer::serve()
{
	// config and start server
	httpd_config_t config = HTTPD_DEFAULT_CONFIG(); // @suppress("Invalid arguments")
	config.uri_match_fn = httpd_uri_match_wildcard;
	if (httpd_start(&server, &config) != ESP_OK) {
		ESP_LOGW(TAG, "could not start http server");
		return ESP_FAIL;
	} else {
		ESP_LOGI(TAG, "started http server");
	}

	// Register API URIs
	APIh = new APIHandler(server);
	if (APIh->registerAllURIs() != ESP_OK) {
		ESP_LOGW(TAG, "could not register API endpoints");
		return ESP_FAIL;
	} else {
		ESP_LOGI(TAG, "registered API endpoints");
	}

	// Register home URI
	hph = new HomePageHandler(server);
	if (hph->registerHomeURI() != ESP_OK)  {
		ESP_LOGW(TAG, "could not register home page");
		return ESP_FAIL;
	} else {
		ESP_LOGI(TAG, "registered home page");
	}

	 return ESP_OK;
}

