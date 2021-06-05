/*
 * wifi_mdns.h
 *
 *  Created on: Apr 4, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_WIFI_UTILS_INCLUDE_WIFI_MDNS_H_
#define COMPONENTS_WIFI_UTILS_INCLUDE_WIFI_MDNS_H_

#ifdef __cplusplus
extern "C" {
#endif


void do_mdns_init(const char* hostname, const char* instanceName);


#ifdef __cplusplus
}
#endif

#endif /* COMPONENTS_WIFI_UTILS_INCLUDE_WIFI_MDNS_H_ */
