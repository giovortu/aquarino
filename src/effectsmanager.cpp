#include "effectsmanager.h"

EffectsManager::EffectsManager()
{
    m_effects[0] = nullptr;
}   


void EffectsManager::addEffect( BaseEffect *effect )
{
    if (m_numEffects < MAX_EFFECTS )    
    {
        m_effects[m_numEffects] = effect;
        m_numEffects++;
    }

}

void EffectsManager::setActiveEffect( int effect )
{
    if ( effect < m_numEffects )
    {
        m_activeEffect = effect;
    }
}


void EffectsManager::Loop()
{
    if ( m_activeEffect >= 0 )
    {
        m_effects[m_activeEffect]->Loop();
    }
}