//Strip class declaration
// Date: 2021/10/12
//create a strip class with header guard
#ifndef __STRIP_H__
#define __STRIP_H__

#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

uint32_t  Wheel( int index, Adafruit_NeoPixel *strip);

class Strip {
    public:
        Strip( int pin , int numRows, int numColumns , String topic);

        void abinsulaLights();
        void randomLights();
        void rainbowLights(  );
        void setDefaultColor(   );
        void showPixels(  );
        void setRawPixelCoords(  String  p_color, int row, int column, bool save = true, bool show = true );
        void setRawPixelCoords(  uint32_t  p_color, int row, int column, bool save = true, bool show = true );
        void setRawPixel(  uint32_t  p_color, int p_rawIndex , bool save = true, bool show = true);
        void setRawPixel(  const String &p_color, int p_rawIndex , bool save = true, bool show = true );
        void setLightPixel(  const String &p_color, int p_index );
        void setAllLightPixelInt(  int p_color, bool save = true );
        void setAllLightPixel(  const String &p_color, bool save = true );
        void setLightPixels(  const JsonArray &arr  );
        void setBrightness( uint32_t  bright );
        void setTemperature( int temp );

        void setRawPixel(uint16_t n, uint8_t r, uint8_t g, uint8_t b , bool save = true, bool show = false);

        void setIsOn( bool ison );
        void displayText( String text );
        void setOff();
        void setOn();
        void restoreColor();

        int numLeds();

        bool isOn();

        String topic;
    private:
        Adafruit_NeoPixel *_strip;

        uint32_t  *_savedColors;
        uint32_t  _savedBrightness;

        String _color;

        int _stripRows;
        int _stripColumns;

        int _numPixels;

        bool _isOn;

        
};

#endif // __STRIP_H__
