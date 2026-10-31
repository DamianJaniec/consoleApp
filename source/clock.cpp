#include "include/clock.h"
#include <thread>

namespace engine
{
    float Clock::tick()
    {
        m_next += m_frame;
        auto now = clock::now();

        if(m_next < now)
            m_next = now;
        else
            std::this_thread::sleep_until(m_next);

        auto end = clock::now(); //faktyczny koniec klatki
        float dt = std::chrono::duration<float>(end - m_last).count();
        m_last = end;

        return dt;
    }
}