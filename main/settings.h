/*
 * config.h
 *
 *  Created on: Mar 29, 2021
 *      Author: Jacob
 */

#ifndef MAIN_SETTINGS_H_
#define MAIN_SETTINGS_H_


/* Wifi settings */
#define WIFI_SSID 			"SSID"
#define WIFI_PASS 			"PASSWORD"

#define HOSTNAME 			"ponics32"
#define MDNS_INSTANCE 		"Ponics32 Instance"

/* Define to host a web server for monitoring & control */
#define CREATE_WEB_SERVER_TASK

/* Define if you are using actuators */
#define CREATE_ACTUATOR_TASK

/* Define if you are using sensors */
#define CREATE_SENSOR_TASK

/* Define if you are using emergencies */
#define CREATE_EMERGENCY_TASK


#endif /* MAIN_SETTINGS_H_ */
