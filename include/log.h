#pragma once

#include <string>
#include <deque>
#include <sstream>

namespace engine::log
{
    enum class Level {Info, Warn, Error};
    struct Entry
    {
        Level level;
        float time;
        std::string text;
    };

    void write(Level level, const std::string& text);
    const std::deque<Entry>& recent();

    template<class... Args>
    std::string concat(const Args&... args)
    {
        std::ostringstream ss;
        (ss << ... << args);
        return ss.str();
    }

    template<class... Args> void info (const Args&... a){write(Level::Info, concat(a...));}
    template<class... Args> void warn (const Args&... a){write(Level::Warn, concat(a...));}
    template<class... Args> void error (const Args&... a){write(Level::Error, concat(a...));}
}