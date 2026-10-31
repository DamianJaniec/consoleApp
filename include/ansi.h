#pragma once

#include <string>
#include "include/types.h"

//podstawoe funkcje, ktore bezposrednio zmieniaja kolor, przesuwaja kursor itp.

namespace engine::ansi
{
    void gotoxy(Vec2i coords); //zmien pozycje kursora
    void setFg(Color c); //OD TERAZ kazdy wypisany symbol jest Koloru c
    void setBg(Color c); //to samo tylko tlo
    void clearScreen(); //cls
    void resetColor(); //domyslny kolor
    void showCursor(bool show); //kursor cmd 

    //W momencie gdy chcemy wyslac cala klatke jako calosc
    void beginFrame(); //poczatek bufora
    void endFrame(); //koniec bufora

}