#include "include/resources.h"
#include "include/log.h"

namespace engine
{
    static void loadTexture(Texture& tex, const std::string& path)
    {
        if(tex.load(path))
            log::info("Wczytano",path," (",tex.size().x,"x",tex.size().y,")");
        else
        {
            log::error("Nie udalo sie wczytac textury: ",path);
            tex.makeMissing();
        }
    }
    Texture& Resources::texture(const std::string& path)
    {
        auto it = m_textures.find(path);
        if(it != m_textures.end())
            return *it->second; //texturra juz sie u nas znajduje

        //wpp
        auto tex = std::make_unique<Texture>();
        loadTexture(*tex, path);
        Texture& ref = *tex;
        m_textures[path] = std::move(tex);
        return ref;

    }
    Ascii& Resources::ascii(const std::string& path)
    {
        auto it = m_ascii.find(path);
        if(it != m_ascii.end())
            return *it->second;

        auto a = std::make_unique<Ascii>();
        if(a->load(path)) log::info("Wczytano ", path);
        else              log::error("Nie udalo sie wczytac ascii: ", path);

        Ascii& ref = *a;
        m_ascii[path] = std::move(a);
        return ref;
    }
    void Resources::reloadAll()
    {
        for(auto& [path, tex] : m_textures)   //C++17: rozpakowanie pary na dwie zmienne
            loadTexture(*tex, path);
        for(auto& [path, a] : m_ascii)
            a->load(path);
        log::info("Przeladowano zasoby (", m_textures.size() + m_ascii.size(), ")");
    }
}