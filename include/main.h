#ifndef __MAIN_H__
#define __MAIN_H__

#include "defines.h"
#include "effects.h"

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <Adafruit_Sensor.h>
//#include "SHT21.h"
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <time.h>



#ifdef USE_MNDS
#include <ESP8266mDNS.h>
#endif

void startMDNS();
void LOG( String message );

void mqttCallback(const char *topic, byte *message, unsigned int length);
void sendWithDelay( const char *strTopic, String messageString, int p_delay  = DELAY_MQTT_PUBLISH );
void sendFloatData( String topic, String type, String unit, float value );
void sendJsonData( String topic, String _json );
void sendStatus(bool force = false);
void ReadWaterTemperature(bool force = false);
void ReadLight(bool force = false);
void ReadDHT(bool force = false);

void setTime();


 BaseEffect *effect = nullptr;


#endif