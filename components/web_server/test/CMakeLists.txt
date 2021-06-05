idf_component_register(SRCS "test_WebServer.cpp" "test_WebServerAPI.cpp"
					INCLUDE_DIRS "."
					REQUIRES unity web_server esp_http_client esp_http_server wifi_utils)