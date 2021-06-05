idf_component_register(SRCS "src/WebServer.cpp"
							"src/HomePageHandler.cpp"
							"src/APIHandler.cpp"
                    INCLUDE_DIRS	"include"
                    REQUIRES log esp_http_server esp_wifi sensor actuator queues status)