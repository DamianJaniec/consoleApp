#include "include/animation.h"

namespace engine
{
    Animation::Animation(const Texture *sheet, Vec2i frameSize, int count, float fps, bool loop): m_sheet(sheet), m_frameSize(frameSize),
    m_count(count), m_fps(fps), m_loop(loop) {}

    void Animation::update(float dt)
    {
        if(m_count <= 0 || m_fps <= 0.0f)
        return;

        float frameTime = 1.0f / m_fps;
        m_time += dt;

        while(m_time >= frameTime)
        {
            m_time -= frameTime;

            if(m_index + 1 < m_count)
                m_index++;              // nastepna klatka
            else if(m_loop)
                m_index = 0;            // od poczatku
            else
            {
                m_time = 0.0f;          // stoi na ostatniej
                break;
            }
        }
    }
    void Animation::reset()
    {
        m_index = 0;
        m_time = 0.0f;
    }
    Recti Animation::frame() const 
    {
        return {m_frameSize.x * m_index, 0, m_frameSize.x, m_frameSize.y};
    }
    bool Animation::finished() const
    {
        return !m_loop && m_index == m_count - 1;
    }
    const Texture* Animation::texture() const
    {
        return m_sheet;
    }
}