/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0: Henrik EKblad
 * Version 1.1 - 2016-07-20: Converted to MySensors v2.0 and added various improvements - Torben Woltjen (mozzbozz)
 * 
 * DESCRIPTION
 * This sketch provides an example of how to implement a humidity/temperature
 * sensor using a DHT11/DHT-22.
 *  
 * For more information, please visit:
 * http://www.mysensors.org/build/humidity
 * 
 */

#define CHILD_ID_SOIL_HUM 0
#define CHILD_ID_SOIL_TEMP 1

// Enable debug prints
#define MY_DEBUG
#define MY_RADIO_RFM69
#define MY_RFM69_NEW_DRIVER
#define MY_RFM69_FREQUENCY RFM69_433MHZ
//#define MY_OTA_FIRMWARE_FEATURE

#include <SPI.h>
#include <MySensors.h>  

#include "Adafruit_seesaw.h"
Adafruit_seesaw ss;

bool metric = true;

static const uint64_t UPDATE_INTERVAL = 30000; // milliseconds

MyMessage msgSoilHum(CHILD_ID_SOIL_HUM, V_LEVEL);
MyMessage msgSoilTemp(CHILD_ID_SOIL_TEMP, V_TEMP);

void presentation()  
{ 
  // Send the sketch version information to the gateway
  sendSketchInfo("OutdoorSolar", "1.1");

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_SOIL_HUM, S_MOISTURE);
  present(CHILD_ID_SOIL_TEMP, S_TEMP);

  metric = getControllerConfig().isMetric;
}


void setup()
{
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while(1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }
}


void loop()      
{
  float soil_temperature = ss.getTemp();
  uint16_t soil_humidity = ss.touchRead(0);

  send(msgSoilTemp.set(soil_temperature, 1));

  #ifdef MY_DEBUG
  Serial.print("ST: ");
  Serial.println(soil_temperature);
  #endif

  send(msgSoilHum.set(soil_humidity, 1));

  #ifdef MY_DEBUG
  Serial.print("SH: ");
  Serial.println(soil_humidity);
  #endif

  // Sleep for a while to save energy
  sleep(UPDATE_INTERVAL); 
}


