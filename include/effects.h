#ifndef __EFFECTS_H__
#define __EFFECTS_H__

#include "defines.h"
class Strip;

class BaseEffect 
{
    public:
        BaseEffect( Strip *strip );

        virtual void Loop() = 0;

    protected:
        Strip *m_strip;
         int m_numLeds = 0;
};


class Cylon : public BaseEffect
{
    public:
        Cylon( Strip *strip );
        
        virtual void Loop() override;

    protected:
        int m_eyeSize = 4;
        int m_red = 255;
        int m_green = 0;
        int m_blue = 0;

        int m_currentLed = 0;
        int m_direction = 1;
        unsigned long m_delay = 100;
        unsigned long m_previousMillis  = 0;
};




#endif

