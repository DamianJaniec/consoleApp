#pragma once

#include <string>
#include <vector>

struct ma_engine;

namespace engine
{
    class Audio
    {
        public:
        Audio();    
        ~Audio();
        Audio(const Audio&) = delete;
        Audio& operator=(const Audio&) = delete;   

        void play(const std::string& path); //gra mp3, nic nie generuje

        //proste generowanie
        int makeBeep(float freq, float duration); //zapisz dzwiek
        //freq - czestotliwosc w Hz
        //duration - w sekundach
        void playBeep(int id, float pitch=1.0f); //odegraj dzwiek

        private:
        ma_engine* m_engine = nullptr;
        struct Beep;
        std::vector<Beep*> m_beeps;

    };
}