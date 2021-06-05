/*
 * WebServer.h
 *
 *  Created on: Apr 4, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_WEB_SERVER_INCLUDE_WEBSERVER_H_
#define COMPONENTS_WEB_SERVER_INCLUDE_WEBSERVER_H_

#include "APIHandler.h"
#include "HomePageHandler.h"

#include "esp_http_server.h"
#include "esp_err.h"


//extern httpd_handle_t* serverHandle;


class WebServer {
private:
	httpd_handle_t server;		// initialized in serve()
	APIHandler* APIh;
	HomePageHandler* hph;
public:
	WebServer();
	~WebServer();
	esp_err_t serve();
};


//float latestSensorReadings;

/* Web Server URI handlers */
//esp_err_t home_get_handler(httpd_req_t *req);

// main init function, should be called after setting up wifi/mdns



#endif /* COMPONENTS_WEB_SERVER_INCLUDE_WEBSERVER_H_ */
