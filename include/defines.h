#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define USE_MNDS 1
#define VERBOSE 1
//#define LOG_TO_SERIAL 1
#define USE_ESPNOW 1

#define RETRY_INTERVAL 10000

#define UFFICIO 1

#define DELAY_MQTT_PUBLISH 100

#define AMBILIGHT_PIN D2
#define AMBILIGHT_PIXELS 60

#define LIGHT_PIN D3
#define LIGHT_PIXELS 156

#define AMBILIGHT_ROWS 1
#define AMBILIGHT_COLUMNS (AMBILIGHT_PIXELS/AMBILIGHT_ROWS)

#define LIGHT_ROWS 6
#define LIGHT_COLUMNS (LIGHT_PIXELS/LIGHT_ROWS)

//#define WATER_TEMP D6
#define WATER_TEMP D1

#define FLUX_PIN D7

#define LUM_PIN A0

//#define SDA_PIN D5
//#define SCL_PIN D4

#define MAX_BRIGHTNESS 255

typedef enum eStrips {
  eNone = -1,
  eAmbilight = 0,
  eLight,

  eNumStrips

} eStrips;

typedef enum  eTemperatures {
  TEMP_1000K = 0,
  TEMP_1300K,
  TEMP_1500K,
  TEMP_1800K,
  TEMP_2000K,
  TEMP_2300K,
  TEMP_2500K,
  TEMP_2800K,
  TEMP_3000K,
  TEMP_3500K,
  TEMP_4000K,
  TEMP_4500K,
  TEMP_5000K,
  TEMP_5500K,
  TEMP_6000K,
  TEMP_6500K,
  TEMP_7000K,
  TEMP_8000K,
  TEMP_9000K,
  TEMP_10000K

} eTemperatures;




extern unsigned long int g_dht_read ;
extern unsigned long int g_water_read ;
extern unsigned long int g_light_read  ;
extern unsigned long int g_mqtt_loop ;
extern unsigned long int g_wifi_loop ;
extern unsigned long int g_status_loop ;


extern uint32_t temperatures_rgb[] ;


//a c matrix containing the characters from Ascii 48  to ascii  90 coded as bitmap like an LCD in a matrix of 6 rows and 4 columns, add a comment containing the actual character
extern const byte ascii[43][6] ;

#define DHT_UPDATE_INTERVAL 60000
#define LIGHT_UPDATE_INTERVAL 10000
#define WATER_UPDATE_INTERVAL 60000
#define SEND_STATUS_INTERVAL 30000

#ifdef UFFICIO
#define AMBILIGHT_TOPIC  "/ufficio28/acquario/ambilight"
#define MAINLIGHT_TOPIC  "/ufficio28/acquario/mainlight"
#define SENSORS_TOPIC    "/ufficio28/acquario/sensors"
#else
#define AMBILIGHT_TOPIC  "/casa/acquario/ambilight"
#define MAINLIGHT_TOPIC  "/casa/acquario/mainlight"
#define SENSORS_TOPIC  "/casa/acquario/sensors"
#endif

extern const char *AMBILIGHT_TOPIC_COMMAND ;
extern const char *MAINLIGHT_TOPIC_COMMAND ;

extern const char *AMBILIGHT_TOPIC_STATUS  ;
extern const char *MAINLIGHT_TOPIC_STATUS  ;

extern const char *TEMPERATURE_TOPIC       ;
extern const char *WATERTEMP_TOPIC         ;
extern const char *HUMIDITY_TOPIC          ;
extern const char *LUMINOSITY_TOPIC  ;

extern const char *STATUS_TOPIC ; 
extern const char *LOG_TOPIC    ; 

extern const char *NTP_1 ;
extern const char *NTP_2 ;
extern const char *NTP_3 ;

#ifdef UFFICIO
extern const char *BROKER ;
extern const char *ssid ;
extern const char *password ;
#else
extern const char *BROKER ;
extern const char *ssid ;
extern const char *password ;
#endif


#ifdef USE_MNDS
extern const char *HOST ;
#endif


#endif

