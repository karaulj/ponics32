/*
 * HomePageHandler.h
 *
 *  Created on: Apr 5, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_WEB_SERVER_INCLUDE_HOMEPAGEHANDLER_H_
#define COMPONENTS_WEB_SERVER_INCLUDE_HOMEPAGEHANDLER_H_

#include "esp_err.h"
#include "esp_http_server.h"

#include "stdint.h"


extern const char* HOME_PAGE_HTML_TEMPLATE;


class HomePageHandler {
private:
	httpd_handle_t server;
	void getActuatorSection(char* buf);
	void getSensorSection(char* buf);
	void generateHTML();
public:
	HomePageHandler(httpd_handle_t serverHandle);
	//static char htmlBuf[];
	//static uint32_t htmlBufSz;
	//void generateHTML();
	esp_err_t registerHomeURI();
	//HomePageHandler();

};

//extern uint32_t generate_home_page_html(char* htmlBuf);


#endif /* COMPONENTS_WEB_SERVER_INCLUDE_HOMEPAGEHANDLER_H_ */
