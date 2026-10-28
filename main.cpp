#include <iostream>
#include "include/terminal.h"
#include "include/canvas.h"
#include "include/renderer.h"
//#include "include/graph.h"

#include <thread>
#include <chrono>

int main()
{
    //graph::consoleInit();


    //graph::gotoxy(12,12);
    //graph::setFg(120,90,60);
    //graph::setBg(170,170,170);
    engine::Terminal term;
    engine::Canvas canva;

    canva.setSize(term.size());

    engine::Renderer render(term);

    int r = 0; 
    int g = 0;
    int b = 0;
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

        r+=1; g+=2; b+=3;

        if(r>255) r = 0;
        if(g>255) g = 0;
        if(b>255) b = 0;

        render.present(canva);
        std::this_thread::sleep_for(std::chrono::milliseconds(33)); //tyczasowe (?)
    }
    return 0;
}