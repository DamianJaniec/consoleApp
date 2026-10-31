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
        char c = ' ';
        Color fg = {255,255,255};
        Color bg = {0,0,0};

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