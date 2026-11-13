#pragma once

#include "include/audio.h"
#include "include/canvas.h"

#include <string>

namespace engine
{
    struct Voice
    {
        int beep = -1; //id beep
        float pitch = 1.0f; //oktawa
        float variation = 0.1f; //rozrzut wysokosc
        float speed = 25.0f; //litery na sekunde
        float volume = 1.0f;
    };

    class Talk
    {
        public:
        void say(const std::string& text, const Voice& voice);
        void update(float dt, Audio& audio);
        void draw(Canvas& c, Vec2i pos, Color fg, Color bg) const;

        void skip();    //pokaz od razu caly tekst
        bool done() const; //czy caly tekst widac

        private:
        std::string m_text;
        Voice m_voice;
        int m_shown = 0;
        float m_timer = 0.0f;
    };
}