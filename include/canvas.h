#pragma once

#include "include/types.h"
#include <iostream>
#include <vector>

//Bufor w ktorych zapisujemy nasza klatke

namespace engine
{
    class Canvas
    {
        engine::Vec2i size; //rozmiar plotna, a nie buforu
        std::vector<Cell> cells; //bufor z naszymi komorkami (dostep {y*size.x + x})

        public:
            void setSize(Vec2i _size);
            Vec2i getSize() const; 

            void clear(Cell c); // "wyzerowanie" bufora
            void setCell(Vec2i coords, Cell c); //ustawianie pojedynczych komorek
            Cell getCell(Vec2i coords) const;

            bool inBounds(Vec2i coords) const; //pomocnicze

            void fillRect(Vec2i coords, int w, int h, Cell c); //funkcje do latwiejszego rysowania
            void drawText(Vec2i coords, const std::string &text, Color fg, Color bg);
            void drawLine(Vec2i coords, Vec2i target, Cell c);

            //dodac wiecej
            //kolo, okrag, rect ale kontur
            //wygooglowac algorytm na rysownie linii prostych z dowolnych punktow (pod dowolnym katem)


    };
}