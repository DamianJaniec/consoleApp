#include "include/log.h"

#include <fstream>
#include <chrono>
#include <cstdio>

namespace engine::log
{
    using clock = std::chrono::steady_clock;

    static const clock::time_point s_start = clock::now();
    static std::deque<Entry> s_recent;
    static const size_t MAX_PERCENT = 100;

    static std::ofstream& file()
    {
        static std::ofstream f("log.txt",std::ios::trunc);
        return f;
    }

    static const char* name(Level l)
    {
        switch(l)
        {
            case Level::Info:  return "INFO ";
            case Level::Warn:  return "WARN ";
            case Level::Error: return "Error";
        }
        return "?";
    }

    void write(Level level, const std::string& text)
    {
        float t = std::chrono::duration<float>(clock::now() - s_start).count();

        char stamp[32];
        std::snprintf(stamp, sizeof(stamp), "[%8.3f] ", t);
        file() << stamp << name(level) << ' '<<text<<std::endl;

        s_recent.push_back({level, t, text});
        if(s_recent.size() > MAX_PERCENT)
            s_recent.pop_front();
    }

    const std::deque<Entry>& recent()
    {
        return s_recent;
    }
}