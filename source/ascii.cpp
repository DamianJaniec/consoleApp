#include "include/ascii.h"
#include "include/utf8.h"

#include <fstream>

namespace engine
{
    Ascii::Ascii(const std::string& path)
    {
        load(path);
    }
    bool Ascii::load(const std::string& path)
    {
        m_size = {0,0};
        m_chars.clear();

        std::ifstream file(path);
        if(!file)
            return false;

        std::vector<std::u32string> lines;
        std::string line;

        while(std::getline(file,line))
        {
            if(!line.empty() && line.back() == '\r')
                line.pop_back();
            lines.push_back(utf8::decode(line));
        }

        while(!lines.empty() && lines.back().empty())
            lines.pop_back();

        if(lines.empty())
            return false;
        
        int w = 0;
        for(const std::u32string& row : lines)
            if((int)row.size() > w) w = (int)row.size();
        int h = (int)lines.size();

        m_size = {w, h};
        m_chars.assign(w * h,U' ');

        for(int y = 0; y < h; y++)
            for(int x = 0; x < (int)lines[y].size(); x++)
                m_chars[y*w+x] = lines[y][x];
        
        return true;
    }
    char32_t Ascii::charAt(Vec2i coords) const
    {
        if(coords.x < 0 || coords.y < 0 || coords.x >= m_size.x || coords.y >= m_size.y)
            return U' ';
        return m_chars[coords.y * m_size.x + coords.x];
    }

}