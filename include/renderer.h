#pragma once

#include "include/terminal.h"
#include "include/canvas.h"

//przerzucenie plotna na ekran

namespace engine
{
    class Renderer
    {
        
    public:

        explicit Renderer(Terminal &_terminal); //terminal na ktorym dzialamy

        void present(const Canvas& frame); //rzutowanie bufora na ekran terminala
        void invalidate(); //wymuszenie Rysowanie calej klatki
        //algorytm sprawdza z poprzednim ekranem gdzie zmienily sie znaki i tam rysujemy
        //gdy wywolamy invalidate - wymusimy rysowanie na calym obszarze bez tego sprawdzania



    private:
       
        void drawCell(Vec2i coords, const Cell& cell);


        Terminal& m_terminal; 
        Canvas m_previous; //poprzedni "ekran"
        bool m_forcedDraw = true; //flaga
        Color m_lastFg = {-1,-1,-1};
        Color m_lastBg = {-1,-1,-1}; //nie znane kolory
        Vec2i m_cursor = {-1, -1}; //-1 -1 nie wiadomo gdzie jest

    };
}