#ifndef EFFECTSMANAGER_H
#define EFFECTSMANAGER_H
 #include "effects.h"

#define MAX_EFFECTS 5

class EffectsManager
{
public:
    EffectsManager();

    void Loop();

    void addEffect( BaseEffect *effect );

    void setActiveEffect( int effect );

protected:

    int m_activeEffect = -1;

    int m_numEffects = 0;

    BaseEffect *m_effects[MAX_EFFECTS]; 
};

 #endif // EFFECTSMANAGER_H