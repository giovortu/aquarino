
#include "main.h"
#include "strip.h"
#include "tools.h"


Strip *mainLight = new Strip( LIGHT_PIN, LIGHT_ROWS, LIGHT_COLUMNS, MAINLIGHT_TOPIC );
Strip *ambiLight = new Strip( AMBILIGHT_PIN, AMBILIGHT_ROWS, AMBILIGHT_COLUMNS , AMBILIGHT_TOPIC);

Strip *strips[ eNumStrips ] = { ambiLight, mainLight };

const char *m_id = "aqclient_001";


ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

//SHT21 sht21;

WiFiClient wifiClient;
void mqttCallback(const char *topic, byte *message, unsigned int length);
PubSubClient mqtt = PubSubClient(BROKER, 1883, mqttCallback, wifiClient);
OneWire oneWire(WATER_TEMP);
DallasTemperature sensors(&oneWire);

Strip * topicToStrip( String strTopic )
{
  Strip *strip = nullptr;

  for ( int i = 0; i < eNumStrips; i++ )
  {
    if ( strTopic.indexOf( strips[i]->topic ) != -1 )
    {
      strip = strips[i];
      break;
    }
  } 


  return strip;
}

#ifdef VERBOSE
void LOG( String message )
{
  String msg;
  msg = "{\"timestamp\":\""+ getTime() + "\", \"log\":\"" + message +"\"}";
#ifdef LOG_TO_SERIAL
  Serial.println( msg );
#else

  mqtt.publish( LOG_TOPIC , msg.c_str() );
#endif
}
#else
void LOG( String message ){}
#endif

void sendWithDelay( const char *strTopic, const char *messageString, int p_delay  = DELAY_MQTT_PUBLISH )
{
    mqtt.publish( strTopic, messageString, true );
    delay(p_delay);
}

void mqttCallback(const char *topic, byte *message, unsigned int length)
{
    String messageString = (char*)message;

    String strTopic = topic;


    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, messageString);
    if (error)
    {
      LOG( "Deserialization error" );//error.c_str());
      return;
    }

    Strip *strip = topicToStrip( strTopic );
    
    if ( nullptr == strip )
    {
      LOG( "NULL STRIP!" );
      return;
    }

    bool fromStatus = false;
    //first time we receive a message from the we unsubscribe from the status topic
    if ( strTopic.indexOf( AMBILIGHT_TOPIC_STATUS ) != -1 || 
         strTopic.indexOf( MAINLIGHT_TOPIC_STATUS ) != -1 )
    {

      fromStatus = true;

      String s1 = String(AMBILIGHT_TOPIC_STATUS) + "/+";
      mqtt.unsubscribe(s1.c_str());
      s1 = String(MAINLIGHT_TOPIC_STATUS) + "/+";
      mqtt.unsubscribe(s1.c_str());
      LOG( "Unsubscribing from mainlight status");
      LOG( "Unsubscribing from ambilight status");
      
      if (doc.containsKey("value"))
      {
        if ( strTopic.indexOf("status") != -1 )
        {
          String status = doc["value"].as<String>();
          strip->setIsOn( status == "on" );
          LOG("IS on from status");
        }

        if ( strTopic.indexOf("color") != -1 )
        {
          String color = doc["value"].as<String>();
          strip->setAllLightPixel( color );
          LOG("Color from status");
        }

        if ( strTopic.indexOf("temperature") != -1 )
        {
          int temp = doc["value"].as<int>();
          if ( temp >= TEMP_1000K  && temp <= TEMP_10000K )
          {
            strip->setAllLightPixelInt( temperatures_rgb[temp]  );
            LOG("Temperature from status");
          }
        }

        if ( strTopic.indexOf("brightness") != -1 )
        {
          int val = doc["value"].as<int>();
          int bright = MAX_BRIGHTNESS * (float)val / 100.0;

          strip->setBrightness(  bright );
          LOG("brightness from status");
        }
      }

    }


    if ( ( strTopic.indexOf( AMBILIGHT_TOPIC_COMMAND ) != -1 || 
         strTopic.indexOf( MAINLIGHT_TOPIC_COMMAND ) != -1)  &&
         !fromStatus )
  {
      if (doc.containsKey("status"))
      {      
        String status = doc["status"].as<String>();
        strip->setIsOn(status == "on" );
        messageString="{\"value\":\""+ status + "\"}";
        String st = strip->topic + "/status/status";
        sendWithDelay( st.c_str(), messageString.c_str() );
        
      }

      if (doc.containsKey("color"))
      {
          String color = doc["color"].as<String>();
          strip->setAllLightPixel( color );

          messageString="{\"value\":\"" + color + "\"}";   

          String st = strip->topic + "/status/color";
          sendWithDelay( st.c_str(), messageString.c_str() );

          
      }

      if ( doc.containsKey("temperature") )
      {
        int temp = doc["temperature"].as<int>();
        if ( temp >= TEMP_1000K  && temp <= TEMP_10000K )
        {

          strip->setAllLightPixelInt( temperatures_rgb[temp]  );
          messageString="{\"value\":"+  String( temp  )  + "}";
          String st = strip->topic + "/status/temperature";
          sendWithDelay( st.c_str(), messageString.c_str() );
        }
      } 

      if (doc.containsKey("brightness"))
      {      
        int val = doc["brightness"].as<int>();
        if ( val >=0 && val <=100 )
        {
          int bright = MAX_BRIGHTNESS * (float)val / 100.0;
          strip->setBrightness( bright );     
          messageString="{\"value\":"+ String( val ) + "}";          
          String st = strip->topic + "/status/brightness";
          sendWithDelay( st.c_str(), messageString.c_str());
        }
      }

      //Mutualmente esclusivi
      if (doc.containsKey("led"))
      {
        JsonObject obj = doc["led"].as<JsonObject>();
        if ( !obj.isNull() )
        {
            int ledIndex = obj["index"].as<int>();
            String color = obj["color"].as<String>();
            strip->setLightPixel( color, ledIndex );     

            messageString="{\"value\":"+ String( ledIndex ) + ",\"color\":\"" + color + "\"}";   
            
            String st = strip->topic + "/status/index";
            sendWithDelay( st.c_str(), messageString.c_str() );
        }
      }
      else
      if ( doc.containsKey("pixel") )
      {
        LOG("PIXEL");
        JsonObject obj = doc["pixel"].as<JsonObject>();
        if ( !obj.isNull() )
        {
            int row = obj["row"].as<int>();
            int col = obj["col"].as<int>();
            
            String color = obj["color"].as<String>();
            strip->setRawPixelCoords( color, row, col );      

            messageString="{\"value\": {\"row\":"+ String( row ) + ",\"col\":"+ String( col ) + ",\"color\":\"" + color + "\"}}";   
            
          String st = strip->topic + "/status/pixel";
          sendWithDelay( st.c_str(), messageString.c_str() );
        }
      }      
      else
      if (doc.containsKey("colors"))
      {
        JsonArray arr = doc["colors"].as<JsonArray>();
        int sz = arr.size();
        String messageBack = "Ready to set " + String(sz);
        LOG( messageBack.c_str());
        strip->setLightPixels( arr );        
      }
      else
      if (doc.containsKey("random"))
      {      
        bool rand = doc["random"].as<bool>();

        if ( rand )
        {
          strip->randomLights(  );
          messageString="{\"value\":true}";   
          
          String st = strip->topic + "/status/random";
          sendWithDelay( st.c_str(), messageString.c_str() );
        }        
      }
      else
      if ( doc.containsKey("abinsula"))
      {
        bool abin = doc["abinsula"].as<bool>();

        if ( abin )
        {
          strip->abinsulaLights(  );
          messageString="{\"value\":true}";           
          String st = strip->topic + "/status/abinsula";
          sendWithDelay( st.c_str(), messageString.c_str() );
        }

      }
      else
      if ( doc.containsKey("rainbow"))
      {
        bool rainb = doc["rainbow"].as<bool>();

        if ( rainb )
        {
          strip->rainbowLights( );
          messageString="{\"value\":true}";
          
          String st = strip->topic + "/status/rainbow";
          sendWithDelay( st.c_str(), messageString.c_str() );
        }  
      }
      else
      if ( doc.containsKey("text"))
      {
        String txt = doc["text"].as<String>();
        strip->displayText(  txt);
        messageString="{\"value\":\""+ txt + "\"}";
        
        String st = strip->topic + "/status/text";
        sendWithDelay( st.c_str(), messageString.c_str() );
        
      }
      else
      if ( doc.containsKey("default"))
      {
        bool def = doc["default"].as<bool>();

        if ( def )
        {
          strip->setDefaultColor( );
          messageString="{\"value\":true}";
          
        String st = strip->topic + "/status/default";
        sendWithDelay( st.c_str(), messageString.c_str() );
        }  
      }
      else      
      {
          strip->restoreColor(  );
      }  

  }
   

}  




inline void mqttConnect()
{
  if (!mqtt.connected())
  {
    while (!mqtt.connected())
    {
      if (mqtt.connect( m_id ))
      {
        delay(150);
        bool res = mqtt.subscribe(AMBILIGHT_TOPIC_COMMAND);
        delay(150);
        res &= mqtt.subscribe(MAINLIGHT_TOPIC_COMMAND);
        delay(150);
        String s1 = String(AMBILIGHT_TOPIC_STATUS) + "/+";
        String s2 = String(MAINLIGHT_TOPIC_STATUS) + "/+";

        res &= mqtt.subscribe(s1.c_str());
        delay(150);
        res &= mqtt.subscribe(s2.c_str());
        delay(150);
        if (res)
        {
          char st[50];
          sprintf(st, "Started ok, id is %s", m_id);           
          LOG( st );
          Serial.println( st );
        }
        else
        {
          LOG( "Started error");
          Serial.println( "Started error" );
        }
        delay(150);
        mqtt.subscribe("/aquarino/cmnd/reset");

        LOG("HTTPUpdateServer ready!" );
        Serial.println( "HTTPUpdateServer ready!" );

        startMDNS();
      }
    }
  }
}

void sendFloatData( String topic, String type, String unit, float value )
{
    float val = isnan(value)?0:value;

    String json = "{ \"timestamp\":\"" + getTime() +  "\",\"unit\":\"" + unit + "\",\"type\":\"" + type + "\",\"value\":" + val + "}" ;
    mqtt.publish(topic.c_str(), json.c_str());
}

void sendJsonData( String topic, String _json )
{
    String json = "{ \"timestamp\":\"" + getTime() +  "\"," + _json + "}" ;
    mqtt.publish(topic.c_str(), json.c_str());
}

void sendStatus(bool force)
{
  if (force || millis() - g_status_loop > SEND_STATUS_INTERVAL)
  {
    g_status_loop = millis();
    String json = "{\"timestamp\":\"" + getTime() + "\",\"address\":\"" + WiFi.localIP().toString() + "\",\"id\":\"" + m_id + "\"}" ;
    String topic = String( STATUS_TOPIC ) + m_id;

    mqtt.publish(topic.c_str(), json.c_str());
  }
}

void ReadWaterTemperature(bool force)
{
  if (force || millis() - g_water_read > WATER_UPDATE_INTERVAL)
  {
    g_water_read = millis();
    sensors.requestTemperatures();
    float wat_Temp = sensors.getTempCByIndex(0);

    sendFloatData( WATERTEMP_TOPIC,"temperature", "°C", wat_Temp );
  }
}

void ReadLight(bool force)
{
  if (force || millis() - g_light_read > LIGHT_UPDATE_INTERVAL)
  {
    g_light_read = millis();
    float lux = analogRead(LUM_PIN) * 0.9765625;
    sendFloatData( LUMINOSITY_TOPIC, "luminosity", "lumen", lux );

  }
}

void ReadDHT(bool force)
{
  if (force || millis() - g_dht_read > DHT_UPDATE_INTERVAL)
  {
    g_dht_read = millis();
    float t = 0.0;//sht21.getHumidity();
    float h = 0.0;//sht21.getTemperature();
    sendFloatData( TEMPERATURE_TOPIC,"temperature", "°C", t);
    sendFloatData( HUMIDITY_TOPIC,"humidity", "%", h );
    char st[50];
    sprintf(st, "Temperature: %f, Humidity: %f", t, h);
    LOG( st );
  }
}

inline void mqttLoop( bool force = false )
{
  //if (force || millis() - g_mqtt_loop > 200)
  {
    mqtt.loop();  
  }
}

void startMDNS()
{
#ifdef USE_MNDS
  while( !MDNS.begin( "aquarino") )
  {
    Serial.println( "MDNS starting...");
    delay(1000);  
  }
  Serial.println( "MDNS started!");
  MDNS.addService("_http", "_tcp", 80);
  MDNS.addService("http", "tcp", 80);
#endif
}

void wifiConnect( bool force = false )
{
  if (force || millis() - g_wifi_loop > 50000)
  {
    g_wifi_loop = millis();
    if (WiFi.status() != WL_CONNECTED)
    {
      WiFi.mode(WIFI_AP_STA);
      WiFi.begin(ssid, password);

      while (WiFi.waitForConnectResult() != WL_CONNECTED)
      {
        WiFi.begin(ssid, password);    
      }

      wifi_set_sleep_type( NONE_SLEEP_T );

    }
    
  }
}




void setTime()
{  
  configTime(0, 0, NTP_1, NTP_2, NTP_3);
  while (!time(NULL))
  {
    delay(20);
  }  
}


void setup(void)
{
  Serial.begin(115200);
  Serial.println("Starting...");
  wifiConnect( true );
  pinMode(LUM_PIN, INPUT);
  //sht21.begin(SDA_PIN, SCL_PIN);
  httpUpdater.setup(&httpServer);
  httpServer.begin();
  sensors.begin();

  setTime();  

  g_dht_read = millis();
  g_water_read = millis();
  g_mqtt_loop = millis();
  g_wifi_loop = millis();
  g_status_loop = millis();

}


void loop(void)
{
  wifiConnect();
  mqttConnect();
  mqttLoop();
  httpServer.handleClient();
  ReadDHT();
  ReadWaterTemperature();
  ReadLight();
  sendStatus();
#ifdef USE_MNDS
  MDNS.update();
#endif
 
}