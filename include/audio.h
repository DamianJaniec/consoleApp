#pragma once

#include <string>
#include <vector>
#include <unordered_map>

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

        void setVolume(float v);

        void play(const std::string& path,float volume = 1.0f); //gra mp3, nic nie generuje

        //proste generowanie
        int makeBeep(float freq, float duration); //zapisz dzwiek
        //freq - czestotliwosc w Hz
        //duration - w sekundach
        void playBeep(int id, float pitch=1.0f,float volume = 1.0f); //odegraj dzwiek
        
        //zatrzymywanie dzwieku
        void stop(const std::string& path);
        void stopBeep(int id);
        void stopAll();

        private:
        struct Sound;
        std::vector<Sound*> m_sounds;
        std::unordered_map<std::string, int> m_files; //sciezka -> numer do plikow

        void playSound(int id, float pitch, float volume);
        ma_engine* m_engine = nullptr;



    };
}