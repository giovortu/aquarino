#include "effects.h"
#include "strip.h"

BaseEffect::BaseEffect( Strip *strip )
{
    m_strip = strip;
    if ( nullptr != m_strip )
    {
      m_numLeds = m_strip->numLeds();
    }
}

Cylon::Cylon( Strip * strip ) : BaseEffect( strip )
{
}

void Cylon::Loop()
{

  unsigned long currentMillis = millis();

  if (currentMillis - m_previousMillis >= m_delay) 
  {
      m_previousMillis = currentMillis;
  
           if ( m_currentLed > m_numLeds)
          {
              m_currentLed = 0;
              m_direction = 1;
          }

          if ( m_currentLed < 0 )
          {
              m_currentLed = m_numLeds - 1;
              m_direction = -1;
          }

          m_strip->setAllLightPixelInt( 0 , false );

          for(int j = 0; j < m_eyeSize / 2; j++)
          {
              float brightness = map(j, 0, (float)m_eyeSize / 2 -1 , 10, 100) / 100;
              int index = m_currentLed + j;
              if ( index < m_numLeds && index >= 0 )
              {
                uint8_t r = m_red * brightness;
                uint8_t g = m_green * brightness;
                uint8_t b = m_blue * brightness;

                m_strip->setRawPixel(index, r, g, b );
              }
          }


          for(int j = 0; j < m_eyeSize / 2; j++)
          {
              float brightness = 1.0 - map(j, 0, (float)m_eyeSize / 2 -1 , 10, 100) / 100;
              int index = m_currentLed + j + m_eyeSize / 2;
              if ( index < m_numLeds && index >= 0 )
              {
                uint8_t r = m_red * brightness;
                uint8_t g = m_green * brightness;
                uint8_t b = m_blue * brightness;

                m_strip->setRawPixel(index, r, g, b );
              }
          }



          m_currentLed+=m_direction;

          m_strip->showPixels();

  }    
    
}
