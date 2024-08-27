#include "defines.h"

#include "secrets.h"


 unsigned long int g_dht_read = 0;
 unsigned long int g_water_read = 0;
 unsigned long int g_light_read = 0;
 unsigned long int g_mqtt_loop = 0;
 unsigned long int g_wifi_loop = 0;
 unsigned long int g_status_loop = 0;


 uint32_t temperatures_rgb[] = {
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
 const byte ascii[43][6] = {
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


 const char *AMBILIGHT_TOPIC_COMMAND = AMBILIGHT_TOPIC "/command";
 const char *MAINLIGHT_TOPIC_COMMAND = MAINLIGHT_TOPIC "/command";

 const char *AMBILIGHT_TOPIC_STATUS  = AMBILIGHT_TOPIC "/status";
 const char *MAINLIGHT_TOPIC_STATUS  = MAINLIGHT_TOPIC "/status";

 const char *TEMPERATURE_TOPIC       = SENSORS_TOPIC "/temperature";
 const char *WATERTEMP_TOPIC         = SENSORS_TOPIC "/watertemperature";
 const char *HUMIDITY_TOPIC          = SENSORS_TOPIC "/humidity";
 const char *LUMINOSITY_TOPIC        = SENSORS_TOPIC "/luminosity";

 const char *STATUS_TOPIC = "/aquarino/status/";
 const char *LOG_TOPIC    = "/aquarino/log";

 const char *NTP_1 = "ntp1.inrim.it";
 const char *NTP_2 = "pool.ntp.org";
 const char *NTP_3 = "time.nist.gov";



#ifdef USE_MNDS
 const char *HOST = "aquarino";
#endif

