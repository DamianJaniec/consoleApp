#pragma once
#include <chrono>


namespace engine
{
    class Clock
    {
        using clock = std::chrono::steady_clock;

        public:
            explicit Clock(int fps) : m_frame(std::chrono::nanoseconds(1'000'000'000)/fps) {}

            float tick();

        private:

            clock::duration m_frame;
            clock::time_point m_next = clock::now();
            clock::time_point m_last = clock::now();

    };
}