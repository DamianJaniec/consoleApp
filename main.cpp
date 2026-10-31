#include <iostream>
#include "include/terminal.h"
#include "include/canvas.h"
#include "include/renderer.h"
#include "include/clock.h"
#include "include/input.h"
//#include "include/graph.h"

#include <string>

engine::Vec2i windowSize = {150,30};

int main()
{
    //graph::consoleInit();


    //graph::gotoxy(12,12);
    //graph::setFg(120,90,60);
    //graph::setBg(170,170,170);
    engine::Terminal term;
    engine::Canvas canva;
    engine::Clock timer(60);
    engine::Input inp(term);

    term.requestSize(windowSize);
    canva.setSize(windowSize);
    //canva.setSize({120,30});
    


    engine::Renderer render(term);

    float r = 0, g = 0, b = 0;
    float dr = 60, dg = 120, db = 180;   // prędkość i kierunek każdego kanału
    bool running = true;
    float dt = 0; // delta time



    while(running)
    {
        inp.update();
        if(inp.pressed('q'))
            running = false;

        engine::Vec2i size = term.size();
        if (size != canva.getSize())//resize uzytkownika
            canva.setSize(size); 
        
        r += dr * dt; g += dg * dt; b += db * dt;

        if (r > 255) { r = 255; dr = -dr; }  if (r < 0) { r = 0; dr = -dr; }
        if (g > 255) { g = 255; dg = -dg; }  if (g < 0) { g = 0; dg = -dg; }
        if (b > 255) { b = 255; db = -db; }  if (b < 0) { b = 0; db = -db; }



        engine::Color bg = {(int)r,(int)g,(int)b};
        engine::Color fg = {0,0,0};
        engine::Cell c = {' ',fg,bg};
        canva.clear(c);


        canva.drawText({0,0}, "FPS: " + std::to_string((int)(1.0f / (dt > 0 ? dt : 1))),{255,255,255},{0,0,0});

        //if (inp.pressed('w')) //test  hold
        //    canva.drawText({0, 1}, "W!", {255,255,255}, {0,0,0});

        if (inp.pressed('w')) //windows autorepeat
            canva.drawText({0, 1}, "W wcisniete", {255,255,255}, {0,0,0});
        if (inp.held('w')) //real hold
            canva.drawText({0, 2}, "W TRZYMANE", {0,255,0}, {0,0,0});

        canva.drawText({0, 3}, "Zażółć gęślą jaźń", {255,255,255}, {0,0,0});

        for (int x = 0; x < 20; x++)
        {
            engine::Cell d;
            d.c  = U'▀';
            d.fg = {255, 0, 0};   // górny "piksel"
            d.bg = {0, 0, 255};   // dolny "piksel"
            canva.setCell({x, 5}, d);
        }

        render.present(canva);

        //std::this_thread::sleep_for(std::chrono::milliseconds(33)); //tyczasowe (?)
        dt = timer.tick();

        
        
    }
    return 0;
}