#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define USE_MNDS 1
#define VERBOSE 1
#define LOG_TO_SERIAL 1

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




inline unsigned long int g_dht_read = 0;
inline unsigned long int g_water_read = 0;
inline unsigned long int g_light_read = 0;
inline unsigned long int g_mqtt_loop = 0;
inline unsigned long int g_wifi_loop = 0;
inline unsigned long int g_status_loop = 0;


inline uint32_t temperatures_rgb[] = {
  0xff3800, //1000K
  0xff5d00, //1300K
  0xff6d00, //1500K
  0xff7e00, //1800K
  0xff8912, //2000K
  0xff9836, //2300K
  0xffa148, //2500K
  0xffad5e, //2800K
  0xffb46b, //3000K
  0xffc987, //3500K
  0xffd1a3, //4000K
  0xffdbba, //4500K
  0xffe4ce, //5000K
  0xffece0, //5500K
  0xfff3ef, //6000K
  0xfff9fd, //6500K
  0xf5f3ff, //7000K
  0xe3e9ff, //8000K
  0xd6e1ff, //9000K
  0xccdbff  //10000K
};


//a c matrix containing the characters from Ascii 48  to ascii  90 coded as bitmap like an LCD in a matrix of 6 rows and 4 columns, add a comment containing the actual character
inline const byte ascii[43][6] = {
  {B01110, B10001, B10011, B10101, B11001, B01110}, //0
  {B00100, B01100, B00100, B00100, B00100, B01110}, //1
  {B01110, B10001, B00001, B00110, B01000, B11111}, //2
  {B11111, B00010, B00100, B00010, B10001, B01110}, //3
  {B00010, B00110, B01010, B10010, B11111, B00010}, //4
  {B11111, B10000, B11110, B00001, B10001, B01110}, //5
  {B00110, B01000, B10000, B11110, B10001, B01110}, //6
  {B11111, B00001, B00010, B00100, B01000, B01000}, //7
  {B01110, B10001, B10001, B01110, B10001, B01110}, //8
  {B01110, B10001, B10001, B01111, B00001, B01110}, //9
  {B00000, B00100, B01010, B00000, B00000, B00000}, //:
  {B00000, B00100, B01010, B00000, B00100, B01000}, //;
  {B00000, B00001, B00010, B00100, B00010, B00001}, //<
  {B00000, B00000, B11111, B00000, B11111, B00000}, //=
  {B00000, B01000, B00100, B00010, B00100, B01000}, //>
  {B01110, B10001, B00001, B00110, B00000, B00100}, //?
  {B01110, B10001, B10111, B10111, B10000, B01110}, //@
  {B01110, B10001, B10001, B11111, B10001, B10001}, //A
  {B11110, B10001, B10001, B11110, B10001, B11110}, //B
  {B01110, B10001, B10000, B10000, B10001, B01110}, //C
  {B11110, B10001, B10001, B10001, B10001, B11110}, //D
  {B11111, B10000, B10000, B11110, B10000, B11111}, //E
  {B11111, B10000, B10000, B11110, B10000, B10000}, //F
  {B01110, B10001, B10000, B10111, B10001, B01111}, //G
  {B10001, B10001, B10001, B11111, B10001, B10001}, //H
  {B01110, B00100, B00100, B00100, B00100, B01110}, //I
  {B00001, B00001, B00001, B00001, B10001, B01110}, //J
  {B10001, B10010, B10100, B11000, B10100, B10010}, //K
  {B10000, B10000, B10000, B10000, B10000, B11111}, //L
  {B10001, B11011, B10101, B10101, B10001, B10001}, //M
  {B10001, B11001, B10101, B10011, B10001, B10001}, //N
  {B01110, B10001, B10001, B10001, B10001, B01110}, //O
  {B11111, B10001, B10001, B11111, B10000, B10000}, //P
  {B01110, B10001, B10001, B10001, B10010, B01101}, //Q
  {B11111, B10001, B10001, B11111, B10100, B10010}, //R
  {B01110, B10001, B10000, B01110, B00001, B11110}, //S
  {B11111, B00100, B00100, B00100, B00100, B00100}, //T
  {B10001, B10001, B10001, B10001, B10001, B01110}, //U
  {B10001, B10001, B10001, B10001, B01010, B00100}, //V
  {B10001, B10001, B10101, B10101, B11011, B10001}, //W
  {B10001, B10001, B01010, B00100, B01010, B10001}, //X
  {B10001, B10001, B01010, B00100, B00100, B00100}, //Y
  {B11111, B00001, B00010, B00100, B01000, B11111}, //Z
};


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

inline const char *AMBILIGHT_TOPIC_COMMAND = AMBILIGHT_TOPIC "/command";
inline const char *MAINLIGHT_TOPIC_COMMAND = MAINLIGHT_TOPIC "/command";

inline const char *AMBILIGHT_TOPIC_STATUS  = AMBILIGHT_TOPIC "/status";
inline const char *MAINLIGHT_TOPIC_STATUS  = MAINLIGHT_TOPIC "/status";

inline const char *TEMPERATURE_TOPIC       = SENSORS_TOPIC "/temperature";
inline const char *WATERTEMP_TOPIC         = SENSORS_TOPIC "/watertemperature";
inline const char *HUMIDITY_TOPIC          = SENSORS_TOPIC "/humidity";
inline const char *LUMINOSITY_TOPIC        = SENSORS_TOPIC "/luminosity";



inline const char *STATUS_TOPIC = "/aquarino/status/";
inline const char *LOG_TOPIC    = "/aquarino/log";

inline const char *NTP_1 = "ntp1.inrim.it";
inline const char *NTP_2 = "pool.ntp.org";
inline const char *NTP_3 = "time.nist.gov";

#include "secrets.h"


#ifdef USE_MNDS
inline const char *HOST = "aquarino";
#endif


#endif

