#include "include/texture.h"

#include <fstream>
#include <sstream>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION 
#include "include/stb_image.h"

namespace engine
{
    Texture::Texture(const std::string& path)
    {
        load(path);
    }
    bool Texture::load(const std::string& path)
    {
        if(path.size() >= 4 && path.substr(path.size() - 4) == ".txt")
            return loadTxt(path);
        else
            return loadImage(path);
    }
    bool Texture::loadImage(const std::string& path)
    {
        m_size = {0,0};
        m_pixels.clear();
        m_visible.clear();

        int w, h, n;
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &n, 4);//rgba
        if(!data) 
            return false;
        m_size = {w,h};
        m_pixels.resize(w*h);
        m_visible.resize(w*h);

        for(int i=0; i<w*h; i++)
        {
            m_pixels[i] = {data[i*4],data[i*4+1],data[i*4+2]};
            m_visible[i] = data[i*4 +3] >=128; //pol przexroczyste to juz u nas przexroczyste
        }
        stbi_image_free(data);
        return true;

    }
    bool Texture::loadTxt(const std::string& path)
    {
        m_size={0,0};
        m_pixels.clear();
        m_visible.clear();

        std::ifstream file(path);
        if(!file)
            return false;
        
        struct Entry {
            Color color;
            bool visible;
        };
        std::unordered_map<char, Entry> palette;
        std::vector<std::string> image;

        bool readingImage = false;
        std::string line;

        while(std::getline(file, line))
        {
            if(!line.empty() && line.back() == '\r')
                line.pop_back();
            
            if(!readingImage)
            {
                if (line=="---") {readingImage=true; continue;} //separator palety od obrazku
                if (line.empty() || line[0] == '#') continue;

                char key = line[0];
                std::istringstream ss(line.substr(1));
                std::string first;
                ss >> first;

                if(first == "none") palette[key] = {{0,0,0},false}; //none -> przeroczysty
                else
                {
                    std::istringstream nums(line.substr(1));
                    int r = std::stoi(first), g = 0, b = 0; //r juz zczytalismy
                    nums>>r>>g>>b;
                    palette[key] = {{r,g,b},true};
                }
                
            }
            else
            {
                image.push_back(line); 
            }
        }

        while(!image.empty() && image.back().empty()) //usun puste linijki
            image.pop_back();
        
        if(image.empty())
            return false;
        
        int h = (int) image.size();
        int w = 0;
        for(const std::string& row : image)
            if((int) row.size() > w) w = (int)row.size();
        
        m_size = {w,h};
        m_pixels.assign(w*h,Color{});
        m_visible.assign(w*h,false);

        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                if(x>=(int)image[y].size()) //krotsza linia
                    continue;
                
                char ch = image[y][x];
                int i = y * w + x;

                auto it = palette.find(ch);
                if(it !=palette.end())
                {
                    m_pixels[i] = it->second.color;
                    m_visible[i] = it->second.visible;
                }
                else
                {
                    m_pixels[i] = {255,0,255}; //znak z poza palety
                    m_visible[i] = true;
                }
            }
        }
        return 1;
    }
    bool Texture::inBounds(Vec2i coords) const
    {
        return coords.x >= 0 && coords.y >= 0 && coords.x < m_size.x && coords.y <m_size.y;
    }
    bool Texture::visible(Vec2i coords) const
    {
        if(!inBounds(coords)) return false;
        return m_visible[coords.y * m_size.x + coords.x];
    }
    Color Texture::colorAt(Vec2i coords) const
    {
        if(!inBounds(coords)) return {0,0,0};
        return m_pixels[coords.y * m_size.x + coords.x];
    }
    Color Texture::sample(float u, float v) const
    {
        if(empty()) return {0,0,0};

        int x = (int)(u * m_size.x);
        int y = (int)(v * m_size.y);

        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x>=m_size.x) x= m_size.x - 1;
        if (y>=m_size.y) y= m_size.y - 1;

        return m_pixels[y * m_size.x + x];
    }
}