#define MINIAUDIO_IMPLEMENTATION
#include "include/miniaudio.h"
#include "include/audio.h"

#include <cmath>

namespace engine
{
    struct Audio::Beep
    {
        std::vector<float> samples; //nasza "fala"
        ma_audio_buffer buffer; //czyta z samples
        ma_sound sound; //to, co sie da odtworzyc
    };




    
    Audio::Audio()
    {
        m_engine = new ma_engine;
        if(ma_engine_init(nullptr,m_engine) != MA_SUCCESS)
        {
            delete m_engine;
            m_engine = nullptr;
        }
    }

    Audio::~Audio()
    {
        if(!m_engine) return;

        for(Beep* b : m_beeps)
        {
            ma_sound_uninit(&b->sound);
            ma_audio_buffer_uninit(&b->buffer);
            delete b;
        }

        ma_engine_uninit(m_engine);
        delete m_engine;
    }

    void Audio::play(const std::string& path)
    {
        if(m_engine)
            ma_engine_play_sound(m_engine,path.c_str(), nullptr);
    }


    //generacja dźwięku
    

    int Audio::makeBeep(float freq, float duration)
    {
        if(!m_engine) return -1;

        Beep* b = new Beep;

        int rate = ma_engine_get_sample_rate(m_engine);   //ile probek na sekunde np 48000
        //bosz aż się zajęcia z cps przypominają
        //straszna katorga, osobiście nie polecam
        int n = (int)(duration * rate);
        b->samples.resize(n);

        for(int i=0; i<n; i++)
        {
            float phase = std::fmod(i*freq / rate,1.0f); //miejsce w fali 0-1
            float wave = phase < 0.5f ? 1.0f : -1.0f; //fala prostokatna
            float fade = 1.0f - (float)i/n; //wyciszenie od konca
            b->samples[i] = wave * fade * 0.2f; //glosnosc
        }

        ma_audio_buffer_config cfg= ma_audio_buffer_config_init(ma_format_f32,1,n,b->samples.data(),nullptr);
        ma_audio_buffer_init(&cfg, &b->buffer);
        ma_sound_init_from_data_source(m_engine,&b->buffer,0,nullptr,&b->sound);

        m_beeps.push_back(b);
        return (int)m_beeps.size() - 1;
    }

    void Audio::playBeep(int id, float pitch)
    {
        if(!m_engine || id < 0 || id >= m_beeps.size()) return;
        ma_sound* s = &m_beeps[id]->sound;
        ma_sound_set_pitch(s,pitch);
        ma_sound_seek_to_pcm_frame(s,0);
        ma_sound_start(s);

    }
}