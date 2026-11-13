#define MINIAUDIO_IMPLEMENTATION
#include "include/miniaudio.h"
#include "include/audio.h"

#include <cmath>

namespace engine
{
    struct Audio::Sound
    {
        std::vector<float> samples; //nasza "fala"
        ma_audio_buffer buffer; //czyta z samples
        ma_sound sound; //to, co sie da odtworzyc

        bool isBeep = false;
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

        for(Sound* s : m_sounds)
        {
            ma_sound_uninit(&s->sound);
            ma_audio_buffer_uninit(&s->buffer);
            delete s;
        }

        ma_engine_uninit(m_engine);
        delete m_engine;
    }
    void Audio::setVolume(float v)
    {
        if(m_engine)
            ma_engine_set_volume(m_engine,v);
    }
    void Audio::playSound(int id, float pitch, float volume)
    {
        if(!m_engine || id<0 || id >= m_sounds.size()) return;
        ma_sound* s = &m_sounds[id]->sound;
        ma_sound_set_volume(s,volume);
        ma_sound_set_pitch(s,pitch);
        ma_sound_seek_to_pcm_frame(s,0); //od poczatku
        ma_sound_start(s);

    }



    void Audio::play(const std::string& path,float volume)
    {
        //if(m_engine)
        //    ma_engine_play_sound(m_engine,path.c_str(), nullptr);
        if(!m_engine)
            return;
        
        auto it = m_files.find(path);
        if(it==m_files.end()) //pierwszy raz
        {
            Sound *s = new Sound;
            if(ma_sound_init_from_file(m_engine,path.c_str(),0,nullptr,nullptr,&s->sound)!=MA_SUCCESS)
            {
                delete s;
                m_files[path] = -1; //flaga aby nie probowal co klatke
                return;
            }
            m_sounds.push_back(s);
            it = m_files.emplace(path,m_sounds.size()-1).first;
        }
        playSound(it->second,1.0f,volume);
    }
    int Audio::makeBeep(float freq, float duration)
    {
        if(!m_engine) return -1;
        Sound *s = new Sound;
        s->isBeep = true;
        int rate = ma_engine_get_sample_rate(m_engine);   //ile probek na sekunde np 48000
        //bosz aż się zajęcia z cps przypominają
        //straszna katorga, osobiście nie polecam
        int n = (int)(duration * rate);
        s->samples.resize(n);

        for(int i=0; i<n; i++)
        {
            float phase = std::fmod(i*freq / rate,1.0f); //miejsce w fali 0-1
            float wave = phase < 0.5f ? 1.0f : -1.0f; //fala prostokatna
            float fade = 1.0f - (float)i/n; //wyciszenie od konca
            s->samples[i] = wave * fade * 0.2f; //glosnosc
        }
        ma_audio_buffer_config cfg= ma_audio_buffer_config_init(ma_format_f32,1,n,s->samples.data(),nullptr);
        ma_audio_buffer_init(&cfg, &s->buffer);
        ma_sound_init_from_data_source(m_engine,&s->buffer,0,nullptr,&s->sound);

        m_sounds.push_back(s);
        return (int)m_sounds.size() - 1;
    }

    void Audio::playBeep(int id, float pitch,float volume)
    {
        playSound(id,pitch,volume);
    }

    void Audio::stop(const std::string& path)
    {
        auto it = m_files.find(path);
        if(it!= m_files.end())
            stopBeep(it->second);
    }
    void Audio::stopBeep(int id)
    {
        if(!m_engine || id<0 || id>m_sounds.size()) return;
        ma_sound_stop(&m_sounds[id]->sound);
    }
    void Audio::stopAll()
    {
        if(!m_engine)   return;
        for(Sound* s : m_sounds)
            ma_sound_stop(&s->sound);
    }
}