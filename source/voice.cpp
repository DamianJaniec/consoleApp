#include "include/voice.h"

#include <cstdlib>

namespace engine
{
    void Talk::say(const std::string& text, const Voice& voice)
    { 
        m_text = text;
        m_shown = 0;
        m_voice = voice;
        m_timer = 0.0f;
    }
    void Talk::update(float dt, Audio& audio)
    {
        if(done() || m_voice.speed <= 0.0f) return;
        
        float perLetter = 1.0f / m_voice.speed;
        m_timer +=dt;

        while(m_timer>= perLetter && !done())
        {
            m_timer -=1.0f/m_voice.speed;
            char ch = m_text[m_shown++];

            while(m_shown < m_text.size() && ((unsigned char)m_text[m_shown] & 0xC0) == 0x80)
                m_shown++;
            
            if(ch==',') m_timer-=0.15f;//krotka pauza
            if(ch=='.' || ch=='!' || ch=='?')   m_timer-=0.3f; 
            if(ch != ' ' && ch!= ',' && ch != '.' && ch!= '!' && ch!='?')
            {
                float r = (rand()% 201 -100)/100.0f; //-1...1
                float pitch =  m_voice.pitch + (1.0f + r * m_voice.variation);
                //audio.playBeep(voice,pitch);
                audio.playBeep(m_voice.beep, pitch, m_voice.volume);
            }
        }
    }
    void Talk::draw(engine::Canvas &c, engine::Vec2i pos, engine::Color fg, engine::Color bg) const
    {
        c.drawText(pos,m_text.substr(0,m_shown),fg,bg);
    }
    void Talk::skip()
    {
        m_shown=m_text.size();
    }
    bool Talk::done() const
    {
        return m_shown >= m_text.size();
    }

}