#pragma once

#include "include/types.h"
#include "include/texture.h"
#include "include/ascii.h"

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

        //void fillRect(Vec2i coords, int w, int h, Cell c); //funkcje do latwiejszego rysowania
        void drawText(Vec2i coords, const std::string &text, Color fg, Color bg);
        
        //puste wywolania
        //rysowanie znakami
        void drawLine(Vec2i coords, Vec2i target, Cell c);
        void drawRect(Vec2i coords, Vec2i _size, Cell c);
        void fillRect(Vec2i coords, Vec2i _size, Cell c);
        void drawCircle(Vec2i coords, int radius, Cell c);
        void fillCircle(Vec2i coords, int radius, Cell c);
        //rysowanie pixelami
        void drawLinePx(Vec2i coords, Vec2i target,Color c);
        void drawRectPx(Vec2i coords, Vec2i _size, Color c);
        void fillRectPx(Vec2i coords, Vec2i _size, Color c);
        void drawCirclePx(Vec2i coords, int radius, Color c);
        void fillCirclePx(Vec2i coords, int radius, Color c);

        void drawAscii(Vec2i coords, const Ascii& tex, Color fg);
        
        void drawTexturePx(Vec2i coords, const Texture& tex); //skala 1:1
        void drawTexturePx(Vec2i coords, Vec2i size, const Texture& tex); //skalowanie do rozmiaryu

        private: 
        struct Brush
        {
            bool pixel;
            Cell cell;
            Color color;
        };
        //prawdziwe funkcje
        void plot   (Vec2i coords, const Brush& b); //jeden punkt
        void line   (Vec2i coords, Vec2i target, const Brush& b);
        void rect   (Vec2i coords, Vec2i _size, const Brush& b);
        void rectFill (Vec2i coords, Vec2i _size, const Brush& b);
        void circle (Vec2i coords, int radius, const Brush& b);
        void circleFill (Vec2i coords, int radius, const Brush &b);
        

        //dodac wiecej
        //kolo, okrag, rect ale kontur
        //wygooglowac algorytm na rysownie linii prostych z dowolnych punktow (pod dowolnym katem)


        //tryb pixelowy
        //za pomoca znaku ▀ możemy mieć pixele
        //kwadrat zajmuje połowe znaku (dzięki czemu jest kwadratem duh...)
        //kolor znaku to poprostu color foreground
        //a kolor pixela poniżej kwadratu to zwyczajnie kolor jego tła
        //więc nie jako mamy 2 pixele zapisane w jednym znaku
        //przy korzystaniu trzeba o tym pamiętać, ponieważ wtedy inaczej
        //przeliczamy kordynaty w porównaniu do zwykłych znaków
        public:
        Vec2i pixelSize() const;    //ile pixeli wejdzie na ekranie
        void setPixel(Vec2i coords, Color color);
        bool pixelInBounds(Vec2i coords) const;
        Color getPixel(Vec2i coords) const;

    };
}