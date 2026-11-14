#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace engine
{
    class Texture
    {
        public:
        Texture() = default;    
        explicit Texture(const std::string& path);

        bool load(const std::string &path);
        bool loadTxt(const std::string &path);
        bool loadImage(const std::string &path);

        Vec2i size() const {return m_size;}
        bool empty() const {return m_size.x == 0 || m_size.y==0;}

        bool visible(Vec2i coords) const;
        Color colorAt(Vec2i coords) const;
        Color sample(float u, float v) const;

        void makeMissing(); //szachownica magenta/czarna

        private:
        bool inBounds(Vec2i coords) const;

        Vec2i m_size;
        std::vector<Color> m_pixels;
        //std::vector<bool> m_visible;
    };
}

/*
Textury beda wygladac na dwa sposoby
tutaj zaprojektowano sposób drugi

1) ASCII, czyli obrazek za pomoca liter, jednokolorowy
- spacja robi za przexroczysty znak
reszta znakow jest wyswietlana w jednym kolorze
2) Pixelowy czyli za pomoca kolorow, jednoznakowy
- na poczatku pliku wypisujemy palete kolorow, a potem obrazek

np.

. none
R 255 0 0
G 0 255 0
B 0 0 255
---
R.....R
.G...G.
...B...
.G...G.
R.....R



*/