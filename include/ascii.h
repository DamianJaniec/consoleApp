#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace engine
{
    class Ascii
    {
        public:
        Ascii() = default;
        explicit Ascii(const std::string &path);

        bool load(const std::string &path);

        Vec2i size() const {return m_size;}
        bool empty() const {return m_size.x == 0 || m_size.y == 0;}
        char32_t charAt(Vec2i coords) const;

        private:
        Vec2i m_size;
        std::vector<char32_t> m_chars;
    };
}