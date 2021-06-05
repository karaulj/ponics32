idf_component_register(SRCS "src/wifi_conn.c"
							"src/wifi_mdns.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_wifi esp_event log nvs_flash lwip mdns)