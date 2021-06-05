/*
 * wifi_mdns.c
 *
 *  Created on: Apr 4, 2021
 *      Author: Jacob
 */

#include "wifi_mdns.h"
#include "mdns.h"
#include "lwip/apps/netbiosns.h"


#ifdef __cplusplus
extern "C" {
#endif


void do_mdns_init(const char* hostname, const char* instanceName)
{
    mdns_init();
    mdns_hostname_set(hostname);
    mdns_instance_name_set(instanceName);

    mdns_txt_item_t serviceTxtData[] = {
        {"board", "esp32"},
        {"path", "/"}
    };

    ESP_ERROR_CHECK(mdns_service_add(NULL, "_http", "_tcp", 80, serviceTxtData,
                                     sizeof(serviceTxtData) / sizeof(serviceTxtData[0])));

    // initialize netBIOS as well. Access via hostname only on Windows
    // i.e. "http://{HOSTNAME}/" not "http://{HOSTNAME}.local/"
    netbiosns_init();
    netbiosns_set_name(hostname);
}


#ifdef __cplusplus
}
#endif
