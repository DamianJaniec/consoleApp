#pragma once

#include "include/terminal.h"
#include "include/canvas.h"

namespace engine
{
    class Renderer
    {
        
    public:

        explicit Renderer(Terminal &_terminal);

        void present(const Canvas& frame);
        void invalidate();


    private:
       
        void drawCell(Vec2i coords, const Cell& cell);


        Terminal& m_terminal; 
        Canvas m_previous;
        bool m_forcedDraw = true; //flaga
        Color m_lastFg = {-1,-1,-1};
        Color m_lastBg = {-1,-1,-1}; //nie znane kolory
        Vec2i m_cursor = {-1, -1}; //-1 -1 nie wiadomo gdzie jest

    };
}