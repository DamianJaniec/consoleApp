#pragma once

#include "include/terminal.h"
#include "include/canvas.h"
#include "include/renderer.h"
#include "include/input.h"
#include "include/resources.h"


//Szkielet programu
//przejmuje terminal, canvas, input, renderer


namespace engine
{
    class App
    {
        public:
        App();
        virtual ~App() = default;

        void run(Vec2i minSize, int fps = 60); //startuje petle
        void quit() {m_running = false;}

        protected:
        virtual void onStart() {} //raz przed pierwsza klatka
        virtual void onUpdate(float dt) {} //logika co klatke
        virtual void onDraw(Canvas& canvas) = 0; //rysowanie co klatke
        virtual void onExit() {} //po wyjsciu z petli

        Resources& resources() {return m_resources;}

        //dostep
        Input& input() {return m_input;}
        Terminal& terminal() {return m_terminal;}
        Vec2i screenSize() const {return m_canvas.getSize();}
        Vec2i screenPixels() const {return m_canvas.pixelSize();}

        bool showFps = true;
        char quitKey = 'q';


        private:
        void drawResizeScreen(Vec2i size);
        void drawFps(float dt);

        void drawConsole(float dt);
        bool m_console = false;

        Terminal m_terminal;
        Input m_input;
        Renderer m_renderer;
        Canvas m_canvas;
        Resources m_resources;

        Vec2i m_minSize;
        bool m_running = false;
        float m_fpsSmooth = 0.0f;


    };
}