
#include "main.h"
#include "strip.h"
#include "tools.h"


Strip *mainLight = new Strip( LIGHT_PIN, LIGHT_ROWS, LIGHT_COLUMNS, MAINLIGHT_TOPIC );
Strip *ambiLight = new Strip( AMBILIGHT_PIN, AMBILIGHT_ROWS, AMBILIGHT_COLUMNS , AMBILIGHT_TOPIC);

Strip *strips[ eNumStrips ] = { ambiLight, mainLight };

String m_id = "aqclient_001";


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

void sendWithDelay( const char *strTopic, String messageString, int p_delay  )
{
    messageString = "{" + messageString + ",\"clientid\":\"" + m_id + "\"}";
    mqtt.publish( strTopic, messageString.c_str() , true );
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

    //Avoid self-send
    if ( doc.containsKey("clientid") )
    {
      String id = doc["clientid"].as<String>();
      if ( id == m_id )
      {
        return;
      }
    }

    Strip *strip = topicToStrip( strTopic );
    
    if ( nullptr == strip )
    {
      LOG( "NULL STRIP!" );
      return;
    }

    //first time we receive a message from the we unsubscribe from the status topic
    if ( strTopic.indexOf( AMBILIGHT_TOPIC_STATUS ) != -1 || 
         strTopic.indexOf( MAINLIGHT_TOPIC_STATUS ) != -1 )
    {

      if (doc.containsKey("value"))
      {
        if ( strTopic.indexOf("status") != -1 )
        {
          String status = doc["value"].as<String>();
          strip->setIsOn( status == "on" );
        }

        if ( strTopic.indexOf("color") != -1 )
        {
          String color = doc["value"].as<String>();
          strip->setAllLightPixel( color );
        }

        if ( strTopic.indexOf("temperature") != -1 )
        {
          int temp = doc["value"].as<int>();
          if ( temp >= TEMP_1000K  && temp <= TEMP_10000K )
          {
            strip->setAllLightPixelInt( temperatures_rgb[temp]  );
          }
        }

        if ( strTopic.indexOf("brightness") != -1 )
        {
          int val = doc["value"].as<int>();
          int bright = MAX_BRIGHTNESS * (float)val / 100.0;

          strip->setBrightness(  bright );
        }
      }

    }


  if ( ( strTopic.indexOf( AMBILIGHT_TOPIC_COMMAND ) != -1 || 
         strTopic.indexOf( MAINLIGHT_TOPIC_COMMAND ) != -1) )
  {
      if (doc.containsKey("status"))
      {      
        String status = doc["status"].as<String>();
        strip->setIsOn(status == "on" );
        messageString="\"value\":\""+ status + "\"";
        String st = strip->topic + "/status/status";
        sendWithDelay( st.c_str(), messageString );
        
      }

      if (doc.containsKey("color"))
      {
          String color = doc["color"].as<String>();
          strip->setAllLightPixel( color );

          messageString="\"value\":\"" + color + "\"";   

          String st = strip->topic + "/status/color";
          sendWithDelay( st.c_str(), messageString );          
      }

      if ( doc.containsKey("temperature") )
      {
        int temp = doc["temperature"].as<int>();
        if ( temp >= TEMP_1000K  && temp <= TEMP_10000K )
        {

          strip->setAllLightPixelInt( temperatures_rgb[temp]  );
          messageString="\"value\":"+  String( temp  ) ;
          String st = strip->topic + "/status/temperature";
          sendWithDelay( st.c_str(), messageString );
        }
      } 

      if (doc.containsKey("brightness"))
      {      
        int val = doc["brightness"].as<int>();
        if ( val >=0 && val <=100 )
        {
          int bright = MAX_BRIGHTNESS * (float)val / 100.0;
          strip->setBrightness( bright );     
          messageString="\"value\":"+ String( val ) ;          
          String st = strip->topic + "/status/brightness";
          sendWithDelay( st.c_str(), messageString);
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

            messageString="\"value\":"+ String( ledIndex ) + ",\"color\":\"" + color + "\"";   
            
            String st = strip->topic + "/status/index";
            sendWithDelay( st.c_str(), messageString );
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

            messageString="\"value\": {\"row\":"+ String( row ) + ",\"col\":"+ String( col ) + ",\"color\":\"" + color + "\"}";   
            
          String st = strip->topic + "/status/pixel";
          sendWithDelay( st.c_str(), messageString );
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
          messageString="\"value\":true";   
          
          String st = strip->topic + "/status/random";
          sendWithDelay( st.c_str(), messageString );
        }        
      }
      else
      if ( doc.containsKey("abinsula"))
      {
        bool abin = doc["abinsula"].as<bool>();

        if ( abin )
        {
          strip->abinsulaLights(  );
          messageString="\"value\":true";           
          String st = strip->topic + "/status/abinsula";
          sendWithDelay( st.c_str(), messageString );
        }

      }
      else
      if ( doc.containsKey("rainbow"))
      {
        bool rainb = doc["rainbow"].as<bool>();

        if ( rainb )
        {
          strip->rainbowLights( );
          messageString="\"value\":true";
          
          String st = strip->topic + "/status/rainbow";
          sendWithDelay( st.c_str(), messageString );
        }  
      }
      else
      if ( doc.containsKey("text"))
      {
        String txt = doc["text"].as<String>();
        strip->displayText(  txt);
        messageString="\"value\":\""+ txt + "\"";
        
        String st = strip->topic + "/status/text";
        sendWithDelay( st.c_str(), messageString );
        
      }
      else
      if ( doc.containsKey("default"))
      {
        bool def = doc["default"].as<bool>();

        if ( def )
        {
          strip->setDefaultColor( );
          messageString="\"value\":true";
          
        String st = strip->topic + "/status/default";
        sendWithDelay( st.c_str(), messageString );
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
  while (!mqtt.connected())
  {
    if (mqtt.connect( m_id.c_str() ))
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
        sprintf(st, "Started ok, id is %s", m_id.c_str() );           
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

void sendFloatData( String topic, String type, String unit, float value )
{
    float val = isnan(value)?0:value;

    String json = "{ \"timestamp\":\"" + getTime() +  "\",\"unit\":\"" + unit + "\",\"type\":\"" + type + "\",\"value\":" + val + ",\"clientid\":\"" + m_id +  "}" ;
    mqtt.publish(topic.c_str(), json.c_str());
}

void sendJsonData( String topic, String _json )
{
    String json = "{ \"timestamp\":\"" + getTime() +  "\"," + _json + ",\"clientid\":\"" + m_id +  "}" ;
    mqtt.publish(topic.c_str(), json.c_str());
}

void sendStatus(bool force)
{
  if (force || millis() - g_status_loop > SEND_STATUS_INTERVAL)
  {
    g_status_loop = millis();
    String json = "{\"timestamp\":\"" + getTime() + "\",\"address\":\"" + WiFi.localIP().toString() + "\",\"clientid\":\"" + m_id + "\"}" ;
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
   mqtt.loop();  
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



void receiveESPNOWCallBackFunction(uint8_t *senderMac, uint8_t *incomingData, uint8_t len) 
{
#ifdef VERBOSE  
    Serial.println( (char*) incomingData );
#endif   
    uint8_t msg[] = "OK";
    uint8_t msg_len = sizeof(msg);
    esp_now_send(senderMac, msg, msg_len);
    parseAndSend( (char*) incomingData , msg_len );
}

void parseAndSend( char *data, uint8_t len )
{
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, data);
  if (error)
  {
    LOG( "Deserialization error" );//error.c_str());
    return;
  }

  if ( doc.containsKey("type") && doc["type"].as<String>() == "agri" &&  doc.containsKey("id") )
  {
    String id =  doc["id"].as<String>();
    String msg ="";
    String header = "{\"timestamp\":\""+ getTime() + "\"";

    String topic = String( SENSORS_TOPIC ) + "/" + id;
    String send_topic = "";
    String command = "";

    float temp = doc["temp"].as<float>();
    float soil = doc["soil"].as<float>();
    float lum  = doc["lum"].as<float>();
    float battlev = doc["bl"].as<float>();
    float hum = doc["hum"].as<float>();

    send_topic = topic + "/luminosity";
    command = header + ",\"value\":" + lum + ",\"type\":\"luminosity\"}";
    mqtt.publish(send_topic.c_str(), command.c_str());

#ifdef VERBOSE
    Serial.println( send_topic + " " + command);
#endif

    send_topic = topic + "/temperature";
    command = header + ",\"value\":" + temp + ",\"type\":\"temperature\"}";
    mqtt.publish(send_topic.c_str(), command.c_str());

#ifdef VERBOSE
    Serial.println( send_topic + " " + command);
#endif

    send_topic = topic + "/soil_moisture";
    command = header + ",\"value\":" + soil + ",\"type\":\"soil_moisture\"}";
    mqtt.publish(send_topic.c_str(), command.c_str());

#ifdef VERBOSE
    Serial.println( send_topic + " " + command);
#endif

    send_topic = topic + "/battery_level";
    command = header + ",\"value\":" + battlev + ",\"type\":\"battery_level\"}";
    mqtt.publish(send_topic.c_str(), command.c_str());

#ifdef VERBOSE
    Serial.println( send_topic + " " + command);
#endif

    send_topic = topic + "/humidity";
    command = header + ",\"value\":" + hum + ",\"type\":\"humidity\"}";
    mqtt.publish(send_topic.c_str(), command.c_str());

#ifdef VERBOSE
    Serial.println( send_topic + " " + command);
#endif


  }

}

void setupESPNow()
{
#ifdef VERBOSE
  Serial.println("ESP-Now Receiver");
  Serial.printf("Transmitter mac: %s\n", WiFi.macAddress().c_str());
#endif

  if (esp_now_init() != 0) 
  {
#ifdef VERBOSE   
    Serial.println("ESP_Now init failed...");
#endif    
    delay(RETRY_INTERVAL);
    setupESPNow();
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(receiveESPNOWCallBackFunction);
#ifdef VERBOSE
  Serial.println("MASTER ready. Waiting for messages...");
#endif
}

void wifiConnect( bool force = false )
{
  if (WiFi.status() != WL_CONNECTED)
  {
    //WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      Serial.print(".");
      attempts++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("\nConnected to Wi-Fi");
      m_id = WiFi.macAddress();
      m_id.replace( ":", "" );

      setupESPNow();

      
    }
    else
    {
      Serial.println("\nFailed to connect to Wi-Fi. Please check your credentials.");
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
  wifiConnect(  );
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
  //ReadDHT();
  ReadWaterTemperature();
  //ReadLight();
  sendStatus();
#ifdef USE_MNDS
  MDNS.update();
#endif
 
}
