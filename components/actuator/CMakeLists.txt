idf_component_register(SRCS "src/actuators.cpp"
							"src/Switch.cpp"
							"src/IActuator.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES log status)