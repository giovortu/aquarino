//Strip class definition
// Date: 2021/10/12


#include "strip.h"
#include "defines.h"
#include "tools.h"


Strip::Strip( int pin , int numRows, int numColumns , String topic)
{
    _stripRows = numRows;
    _stripColumns = numColumns;
    _numPixels = numRows * numColumns;
    this->topic = topic;
    _savedColors = new uint32_t [_numPixels];
    _isOn = false;
    _savedBrightness = 0;
    _color = "0000ff";


    _strip = new Adafruit_NeoPixel(_numPixels, pin, NEO_GRB + NEO_KHZ800);
    _strip->begin();

    for ( int i = 0; i < _numPixels; i++ )
    {
      _savedColors[i] = temperatures_rgb[ TEMP_7000K ];
    }
  
}

int Strip::numLeds()
{
    return _numPixels;
}

bool Strip::isOn()
{
    return _isOn;
}

uint32_t  Wheel( int index, Adafruit_NeoPixel *strip)
{
  index = strip->numPixels() - index;

  int onethird =  strip->numPixels() * 1 / 3;
  int twothirds = strip->numPixels() * 2 / 3;
  
  if ( index < onethird )
  {
    return strip->Color(strip->numPixels() - index * 3, 0, index * 3);
  }
  if ( index < twothirds )
  {
    index -= onethird;
    return strip->Color(0, index * 3, strip->numPixels() - index * 3);
  }
  index -= twothirds;
  return strip->Color(index * 3, strip->numPixels() - index * 3, 0);
}


void Strip::displayText( String text )
{

  _strip->clear();

  text.toUpperCase();

  for ( uint32_t  i = 0; i < text.length(); i++ )
  {
    int startColumn = i * 6; // space between letters
    int charIndex = (int)text.charAt( i ) - '0';
    
    for ( int r = 0; r < 6; r++ )
    {
      for ( int c = 0; c < 5; c++ )
      {              
        int value = ascii[ charIndex ][r] & (1 << c) ? 0xFFFFFF : 0x000000;
        setRawPixelCoords( value, r, startColumn + 5 - c,  true, false);
        //String st = "Col " + String( startColumn + c ) + " Char " + String( '0' + charIndex ) + " row " + String( r )+ " col " + String( c ) + " value " + value;
      }
    }
  }
  showPixels(  );
}

void Strip::rainbowLights( )
{
    for ( int i = 0; i <  _strip->numPixels(); i++ )
    {
      uint32_t  color = Wheel(i, _strip );
      _strip->setPixelColor(i, color);
      _savedColors[ i ] = color;
    }
    _strip->show();
}


void Strip::abinsulaLights(  )
{
  int start = 0;
  int onethird =  _strip->numPixels() * 1 / 3 - 1;
  int twothirds = _strip->numPixels() * 2 / 3 - 1;
  int end = _strip->numPixels();

  uint32_t  color = 0xfc3020;
  for ( int i = start; i < onethird ; i++ )
  {
    color = 0xfc3020;
    _strip->setPixelColor(i, color);
    _savedColors[ i ] = color;
  }
  for ( int i = onethird; i < twothirds -1; i++ )
  {
    color = 0x7d7610;
    _strip->setPixelColor(i, color);
    _savedColors[ i ] = color;
  }

  for ( int i = twothirds; i < end; i++ )
  {
    color = 0x1a44ff;
    _strip->setPixelColor(i, color);
    _savedColors[ i ] = color;
  }

  _strip->show();
}

void Strip::randomLights(  )
{
    for ( int i = 0; i < _strip->numPixels(); i++ )
    {
      uint32_t  color = random(0, 0xffffff);
      _strip->setPixelColor(i, color );
      _savedColors[ i ] = color;
    }
    _strip->show();

}

void Strip::setTemperature( int temp )
{
   for ( int i =0; i < _strip->numPixels() ; i++)
   {
      setRawPixel(temperatures_rgb[temp] , i , true, false);
   }
   _strip->show();
}

void Strip::setDefaultColor(  )
{
  for ( int i = 0; i <  _strip->numPixels(); i++ )
  {
    _strip->setPixelColor(i, 0x0000ff);
  }
  _strip->show();
}

void Strip::showPixels( )
{
    _strip->show();
}

void Strip::setRawPixelCoords(  String  p_color, int row, int column, bool save, bool show )
{
  uint32_t  color = ( uint32_t  )strtol( p_color.c_str(), 0, 16 );

  setRawPixelCoords( color, row, column, save, show );
}

void Strip::setRawPixelCoords(  uint32_t  p_color, int row, int column, bool save, bool show )
{
     int stripColumn = _strip->numPixels() / _stripRows;
    if ( row < _stripRows && column < stripColumn )
    {
      
      int p_rawIndex = 0;
     
      if ( row % 2 == 0 )
      {
        p_rawIndex = row * stripColumn + column;
      }
      else
      {
         p_rawIndex = row * stripColumn + ( stripColumn - column - 1 );
      }


      if ( save )
      {    
          _savedColors[ p_rawIndex ] = p_color;
      }

      _strip->setPixelColor(p_rawIndex, p_color);
      if ( show )
      {
        _strip->show();
      }
    }
  
}

void Strip::setRawPixel(  uint32_t  p_color, int p_rawIndex , bool save, bool show)
{
    if ( p_rawIndex < _strip->numPixels() )
    {
      if ( save )
      {    
          _savedColors[ p_rawIndex ] = p_color;
      }

      _strip->setPixelColor(p_rawIndex, p_color);
      if ( show )
      {
        _strip->show();
      }
    }
 }

void Strip::setRawPixel(uint16_t p_rawIndex, uint8_t r, uint8_t g, uint8_t b, bool save, bool show )
 {
    if ( p_rawIndex < _strip->numPixels() )
    {
      if ( save )
      {    
          //_savedColors[ p_rawIndex ] =;
      }

      _strip->setPixelColor(p_rawIndex, r,g,b );
      if ( show )
      {
        _strip->show();
      }
    }
 }



void Strip::setRawPixel(  const String &p_color, int p_rawIndex , bool save, bool show )
{
    if ( p_color.length() != 6 )
    {
        return;
    }
#if 0    
    String rString = p_color.substring(0, 2);
    String gString = p_color.substring(2, 4);
    String bString = p_color.substring(4, 6);

    uint8_t r = hexToInt(rString[0]) * 16 + hexToInt(rString[1]);
    uint8_t g = hexToInt(gString[0]) * 16 + hexToInt(gString[1]);
    uint8_t b = hexToInt(bString[0]) * 16 + hexToInt(bString[1]);
#endif
    uint32_t  col = ( uint32_t  )strtol( p_color.c_str(), 0, 16 );


    if ( p_rawIndex < _strip->numPixels() )
    {
        if ( save )
        {    
            _savedColors[ p_rawIndex ] = col;
        }

        _strip->setPixelColor(p_rawIndex, col);
        if ( show )
        {
            _strip->show();
        }
    }

}

void Strip::setLightPixel(  const String &p_color, int p_index )
{
   setRawPixel( p_color, p_index );
}

void Strip::setAllLightPixelInt(  int p_color, bool save )
{
    for ( int i =0; i < _strip->numPixels() ; i++)
    {
        setRawPixel(p_color, i , save, false);
    }
    showPixels( );
}

void Strip::setAllLightPixel(  const String &p_color, bool save )
{
    if ( p_color.length() != 6 )
    {
        return;
    }
    for ( int i =0; i < _strip->numPixels() ; i++)
    {
        setRawPixel(p_color, i , save, false);
    }
    showPixels( );
}         

void Strip::setLightPixels(  const JsonArray &arr  )
{
    int sz = arr.size();

    for( int i= 0; i < _strip->numPixels() && i < sz; i++ )
    {
    String color = arr[i].as<String>();  
    setRawPixel( color, i , true, false);
    }
    showPixels( );
}

void Strip::setBrightness( uint32_t  bright )
{
    _savedBrightness = bright;
    if ( _isOn )
    {
        _strip->setBrightness( bright );
    }
    else
    {
        _strip->setBrightness( 0 );
    }
}


void Strip::setOn(  )
{
    setIsOn( true );
}

void Strip::setOff(  )
{
    setIsOn( false );
}

void Strip::setIsOn( bool isOn )
{
    _isOn = isOn;
    if ( _isOn)
    {
        _strip->setBrightness( _savedBrightness );
    }
    else
    {
        _strip->setBrightness( 0 );
    }
}

void Strip::restoreColor( )
{
    for ( int i = 0; i < _strip->numPixels() ; i++ )
    {
      setRawPixel( _savedColors[i], i, false ,false);
    }
    showPixels( );
}


