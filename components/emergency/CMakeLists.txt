idf_component_register(SRCS "src/emergencies.cpp"
							"src/GrowBedOverflow.cpp"
							"src/IEmergency.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES log actuator)