/*
 * test_WebServer.cpp
 *
 *  Created on: Apr 14, 2021
 *      Author: Jacob
 */

#include <stdio.h>
#include "unity.h"

#include "esp_http_client.h"
#include "esp_http_server.h"

#include "WebServer.h"
#include "wifi_conn.h"


esp_err_t _http_event_handle(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
        	printf("HTTP_EVENT_ERROR\n");
            break;
        case HTTP_EVENT_ON_CONNECTED:
        	printf("HTTP_EVENT_ON_CONNECTED\n");
            break;
        case HTTP_EVENT_HEADER_SENT:
        	printf("HTTP_EVENT_HEADER_SENT\n");
            break;
        case HTTP_EVENT_ON_HEADER:
        	printf("HTTP_EVENT_ON_HEADER\n");
            printf("%.*s", evt->data_len, (char*)evt->data);
            break;
        case HTTP_EVENT_ON_DATA:
        	printf("HTTP_EVENT_ON_DATA (len=%d)", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                printf("%.*s", evt->data_len, (char*)evt->data);
            }

            break;
        case HTTP_EVENT_ON_FINISH:
        	printf("HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
        	printf("HTTP_EVENT_DISCONNECTED");
            break;
    }
    return ESP_OK;
}


TEST_CASE("serve() hosts home page", "[WEB_SERVER]")
{
	// do wifi init
	const char* SSID = "neptune";
	const char* PASS = "coldandwindy";
	do_wifi_init(SSID, PASS);

	// start server
	//WebServer* ws1 = new WebServer;
	ws1->serve();

	// create http client
	esp_http_client_config_t clientConfig;
	//clientConfig.url = "http://192.168.254.25/";
	clientConfig.url = "http://httpbin.org/redirect/2";
	//clientConfig.method = HTTP_METHOD_GET;
	clientConfig.event_handler = _http_event_handle;
	esp_http_client_handle_t client = esp_http_client_init(&clientConfig);
	if (esp_http_client_perform(client) == ESP_OK) {
	   printf("Status = %d, content_length = %d\n",
	           esp_http_client_get_status_code(client),
	           esp_http_client_get_content_length(client));
	}

	// get Content-Type header val
	char* contentType;
	esp_http_client_get_header(client, "Content-Type", &contentType);
	TEST_ASSERT_NOT_NULL(contentType);
	printf("Content-Type: %s\n", contentType);

	// test home page
	TEST_ASSERT_EQUAL_STRING("text/html", contentType);
	TEST_ASSERT_EQUAL(200, esp_http_client_get_status_code(client));



	esp_http_client_cleanup(client);
	delete ws1;

}
