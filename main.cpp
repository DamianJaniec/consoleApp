#include <iostream>
#include "include/terminal.h"
#include "include/canvas.h"
#include "include/renderer.h"
//#include "include/graph.h"

#include <thread>
#include <chrono>

engine::Vec2i windowSize = {150,30};

int main()
{
    //graph::consoleInit();


    //graph::gotoxy(12,12);
    //graph::setFg(120,90,60);
    //graph::setBg(170,170,170);
    engine::Terminal term;
    engine::Canvas canva;

    term.requestSize(windowSize);
    canva.setSize(windowSize);
    //canva.setSize({120,30});
    


    engine::Renderer render(term);

    int r = 0, g = 0, b = 0;
    int dr = 1, dg = 2, db = 3;   // prędkość i kierunek każdego kanału
    bool running = true;
    



    while(running)
    {
        char key;
        while (term.readByte(key))
            if (key == 'q') running = false;

        
        
        engine::Color bg = {r,g,b};
        engine::Color fg = {0,0,0};
        engine::Cell c = {' ',fg,bg};
        canva.fillRect({0,0},term.size().x,term.size().y,c);

        r += dr; g += dg; b += db;

        if (r > 255 || r < 0) { dr = -dr; r += 2 * dr; }
        if (g > 255 || g < 0) { dg = -dg; g += 2 * dg; }
        if (b > 255 || b < 0) { db = -db; b += 2 * db; }

        render.present(canva);
        std::this_thread::sleep_for(std::chrono::milliseconds(33)); //tyczasowe (?)

        engine::Vec2i size = term.size();
        if (size != canva.getSize())//resize uzytkownika
            canva.setSize(size); 
        
    }
    return 0;
}