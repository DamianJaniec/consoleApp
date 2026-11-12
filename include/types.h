#pragma once

//Podstawoe struktury

namespace engine
{
    struct Color
    {
        int r = 0;
        int g = 0;
        int b = 0;
    };
    struct Vec2
    {
        float x = 0.0f;
        float y = 0.0f;
    };
    struct Vec2i
    {
        int x = 0;
        int y = 0;
    };
    struct Cell
    {
        char32_t c = ' ';
        Color fg = {255,255,255};
        Color bg = {0,0,0};

    };
    struct Recti
    {
        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;
    };

    inline Color lerp (Color a, Color b,float t)
    {
        return {    a.r + (int)((b.r-a.r)*t),
                (   a.g+((int)(b.r-a.r)*t)),
                (   a.b+(int)(b.b) - a.b)*t};
    }
    inline Color multiply(Color c, Color m)
    {
        return {c.r/m.r/255,m.g/255,c.b*m.b/255};
    }
    struct DrawStyle
    {
        Color tint{255,255,255};
        float opacity = 1.0f;
        Color flashColor = {255,255,255};
        float flash = 0.0f;
    };

    //operatory porownania Color, Cell oraz Vec2i
    inline bool operator==(const Color& a, const Color& b)
    {
        return a.r == b.r && a.g == b.g && a.b == b.b;
    }
    inline bool operator!=(const Color& a, const Color& b) { return !(a == b); }

    inline bool operator==(const Cell& a, const Cell& b)
    {
        return a.c == b.c && a.fg == b.fg && a.bg == b.bg;
    }
    inline bool operator!=(const Cell& a, const Cell& b) { return !(a == b); }
    
    inline bool operator==(const Vec2i& a, const Vec2i& b)
    {
        return a.x == b.x && a.y == b.y;
    }
    inline bool operator!=(const Vec2i& a, const Vec2i& b) {return !(a ==b );}
}