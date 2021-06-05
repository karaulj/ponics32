idf_component_register(SRCS "src/sensors.cpp"
							"src/EZO_RTD_I2C_Sensor.cpp"
							"src/ISensor.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES log)