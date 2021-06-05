/*
 * wifi_conn.h
 *
 *  Created on: Mar 17, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_WIFI_UTILS_INCLUDE_WIFI_CONN_H_
#define COMPONENTS_WIFI_UTILS_INCLUDE_WIFI_CONN_H_

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1


void wifi_init_nvs(void);
esp_err_t wifi_init_sta(const char* ssid, const char* pass);
esp_err_t do_wifi_init(const char* ssid, const char* pass);

#ifdef __cplusplus
}
#endif

#endif /* COMPONENTS_WIFI_UTILS_INCLUDE_WIFI_CONN_H_ */
