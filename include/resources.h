#pragma once

#include "include/texture.h"
#include "include/ascii.h"

#include <string>
#include <memory>
#include <unordered_map>

//wspolny magazyn plikow
namespace engine
{
    class Resources
    {
        public:
        Texture& texture(const std::string& path);
        Ascii& ascii(const std::string& path);

        void reloadAll(); 

        private:
        std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
        std::unordered_map<std::string, std::unique_ptr<Ascii>> m_ascii;
    };
}