/*
 * HomePageHandler.cpp
 *
 *  Created on: Apr 5, 2021
 *      Author: Jacob
 */

#include "HomePageHandler.h"

#include "sensors.h"
#include "actuators.h"

#include "stdint.h"
#include <string.h>
#include <stdio.h>
#include <iostream>

#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "homePageHandler";

//uint32_t HomePageHandler::htmlBufSz = 0;
//char* HomePageHandler::htmlBuf = NULL;

static char* homeHTMLBuf[4096];
static uint32_t homeHTMLSz;


HomePageHandler::HomePageHandler(httpd_handle_t serverHandle)
{
	this->server = serverHandle;
}


static esp_err_t homeGetHandler(httpd_req_t* req)
{
	ESP_LOGI(TAG, "got client");

	httpd_resp_set_type(req, "text/html");
	return httpd_resp_send(req, (const char*)homeHTMLBuf, homeHTMLSz);
}


const char* HOME_PAGE_HTML_TEMPLATE = R"=====(
<html lang="en">
<head>
<title>32Ponics</title>

<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.1.1/css/bootstrap.min.css" integrity="sha384-WskhaSGFgHYWDcbwN70/dfYBj47jz9qbsMId/iRN3ewGhXQFZCSftd1LZCfmhktB" crossorigin="anonymous" />

<link rel="stylesheet" type="text/css" href="https://fonts.googleapis.com/css?family=Ubuntu+Mono" />
</head>

<body style="background-color:rgba(220,230,242,1);font-family:Ubuntu Mono">
<div class="container-fluid">

	<div class="row">
		<div class="col">
			<br><h1 style="text-align:center"><b>ESP32Ponics Web UI</b></h1><br>
		</div>		
	</div>		
	<div class="row">
		<div class="col-md-6" style="border-right:2px solid #ccc;">
			<div class="row"><h2 style="margin-left:10px">Sensors</h2></div>
			<!-- sensors go here -->
			%s
		</div>		
		<div class="col-md-6">
			<div class="row"><h2 style="margin-left:10px">Actuators</h2></div>
			<!-- actuators go here -->
			%s
		</div>
	</div>

</div>
<script>
setInterval(function() {
	<!-- total # of sensors goes here -->
	for (var i=0; i<%u; i++) {
		getSensorReading(i);
	}
}, 1000);
function getSensorReading(id) {
	var req = new XMLHttpRequest();
	req.onload = function() {
		if (req.status == 200) {
			console.log("Got sensor " + id + " reading " + this.responseText);
			document.getElementById("sensor"+id+"val").innerHTML = this.responseText;
		} else {
			console.log("Err on getSensorReading: " + req.status);
			console.log(this.response);
		}
	}
	var reqUrl = "getReading?id="+id + "&";
	//console.log("Sending request " + reqUrl);
	req.open("GET", reqUrl, true);
	req.send();
}
function driveActuator(id, val) {
	var req = new XMLHttpRequest();
	req.onload = function() {
		if (req.status == 200) {
			console.log("Drove actuator " + id + " with val " + val + " ("  + this.responseText + ")");
		} else {
			console.log("Err on driveActuator: " + req.status);
			console.log(this.response);
		}
	}
	var reqUrl = "driveActuator?id="+id + "&driveVal="+val + "&";
	//console.log("Sending request " + reqUrl);
	req.open("POST", reqUrl, true);
	req.send();
}
</script>
</body>

</html>
)=====";


void HomePageHandler::getActuatorSection(char* buf)
{
	std::string strBuf;
	IActuator* actuator;
	for (int i=0; i<IActuator::count(); i++)
	{
		actuator = &allActuators[i];
		const char* acId = std::to_string(actuator->id).c_str();
		if (actuator->useBooleanDrive) {
			strBuf.append(
				R"=====(
				<div class="row">
					<div class="col-8">)=====").append(actuator->name).append(R"=====(</div>
					<div class="col-4 style="display:flex;align-items:center;"><input type="checkbox" style="transform:scale(1.5);" onclick="driveActuator()=====").append(acId).append(R"=====(, this.checked ? 1 : 0)" checked></div>
				</div>
				)====="
			);
		} else {
			strBuf.append(
				R"=====(
				<div class="row">
					<div class="col-6">)=====").append(actuator->name).append(R"=====(</div>
					<div class="col-3"><input type="number" id="ac)=====").append(acId).append(R"=====(val" min="0" max=")=====").append(std::to_string(DRIVE_VAL_MAX)).append(R"=====(" value="0"></div>
					<div class="col-3"><input type="submit" value="Drive" onclick="driveActuator()=====").append(acId).append(R"=====(, document.getElementById('ac)=====").append(acId).append(R"=====(val').value)"></div>
				</div>
				)====="
			);
		}


	}
	//char buf[strBuf.size()];
	strcpy((char*)buf, strBuf.c_str());
	//return buf;
}


void HomePageHandler::getSensorSection(char* buf)
{

	std::string strBuf;
	ISensor* sensor;
	for (int i=0; i<ISensor::count(); i++)
	{
		sensor = &allSensors[i];
		strBuf.append(
				R"=====(
				<div class="row">
					<div class="col-8">)=====").append(sensor->name).append(R"=====(</div>
					<div class="col-4" style="font-size:125%;align-items:center;"><b><span id="sensor)=====").append(std::to_string(sensor->id)).append(R"=====(val">-1.0</span></b> )=====").append(sensor->units).append(R"=====(</div>
				</div>
				)====="
		);

	}
	//char buf[strBuf.size()];
	strcpy((char*)buf, strBuf.c_str());
	//return buf;
}

void HomePageHandler::generateHTML()
{
	ESP_LOGI(TAG, "starting HTML generation");

	char sensorSection[256*ISensor::count()];
	char actuatorSection[512*IActuator::count()];

	this->getSensorSection(sensorSection);
	this->getActuatorSection(actuatorSection);

	sprintf((char *)homeHTMLBuf, HOME_PAGE_HTML_TEMPLATE, sensorSection, actuatorSection, ISensor::count());
	homeHTMLSz = strlen((const char*)homeHTMLBuf);

	//strcpy(buf, HOME_PAGE_HTML_TEMPLATE);
	ESP_LOGI(TAG, "finished HTML generation: %u bytes", homeHTMLSz);
}


esp_err_t HomePageHandler::registerHomeURI()
{
	// generate home page HTML
	this->generateHTML();

	// default URI handler setup
	httpd_uri_t homeUri;
	homeUri.uri = "/";
	homeUri.method = HTTP_GET; // @suppress("Symbol is not resolved")
	homeUri.handler = homeGetHandler;
	return httpd_register_uri_handler(this->server, &homeUri);

}
