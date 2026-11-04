#include "include/app.h"
#include "include/clock.h"
#include "include/log.h"

#include <algorithm>
#include <string>

namespace engine
{
    App::App() : m_input(m_terminal), m_renderer(m_terminal) {}

    void App::run(Vec2i minSize, int fps)
    {
        m_minSize = minSize;
        m_terminal.requestSize(minSize);
        m_canvas.setSize(m_terminal.size());

        Clock clock(fps);
        float dt = 0.0f;
        m_running = true;

        onStart();

        log::info("Start, minimalne okno",minSize.x,"x",minSize.y);

        while(m_running && !Terminal::quitRequested())
        {
            m_input.update();
            if(quitKey && m_input.pressed(quitKey))
            {
                log::info("Wcisnieto q");
                m_running = false;
            }
            if(m_input.pressed('`'))
                m_console = !m_console;
            
            Vec2i size = m_terminal.size();
            if(size != m_canvas.getSize())
                m_canvas.setSize(size);
            
            if(size.x < m_minSize.x || size.y < m_minSize.y)
            {
                drawResizeScreen(size);
                m_renderer.present(m_canvas);
                dt = clock.tick();
                continue;
            }

            onUpdate(dt);
            onDraw(m_canvas);
            
            if(m_console)
                drawConsole(dt);
            else if(showFps)
                drawFps(dt);

            m_renderer.present(m_canvas);
            dt = clock.tick();
        }

        log::info("Koniec");
        onExit();
    }
    void App::drawFps(float dt)
    {
        if(dt > 0.0f)
            m_fpsSmooth = m_fpsSmooth * 0.9f + (1.0f / dt) * 0.1f;
        m_canvas.drawText({0,0},"FPS: " + std::to_string((int)m_fpsSmooth),{255,255,255},{0,0,0});
    }
    void App::drawResizeScreen(Vec2i size)
    {
        Color bg   = {24, 26, 32};
        Color text = {220, 220, 220};
        Color bad  = {255, 90, 80};
        Color good = {110, 220, 120};

        m_canvas.clear({' ', text, bg});

        std::string l1 = "Powieksz okno terminala";
        std::string l2 = "szerokosc: " + std::to_string(size.x) + " / " + std::to_string(m_minSize.x);
        std::string l3 = "wysokosc:  " + std::to_string(size.y) + " / " + std::to_string(m_minSize.y);
        std::string l4 = "Q - wyjscie";

        auto cx = [&](const std::string& s) {return std::max(0, (size.x - (int)s.size()) / 2);};
        int cy = std::max(0, size.y / 2 - 2);

        m_canvas.drawText({cx(l1), cy},     l1, text, bg);
        m_canvas.drawText({cx(l2), cy + 2}, l2, size.x >= m_minSize.x ? good : bad, bg);
        m_canvas.drawText({cx(l3), cy + 3}, l3, size.y >= m_minSize.y ? good : bad, bg);
        m_canvas.drawText({cx(l4), cy + 5}, l4, {130, 130, 140}, bg);
    }
    void App::drawConsole(float dt)
    {
        const int lines = 8;
        Vec2i s = m_canvas.getSize();
        Color bg = {12, 12, 16};

        for(int y = 0; y < lines + 2; y++)            // tlo paska
            for(int x = 0; x < s.x; x++)
                m_canvas.setCell({x, y}, {' ', {255,255,255}, bg});

        if(dt > 0.0f)
            m_fpsSmooth = m_fpsSmooth * 0.9f + (1.0f / dt) * 0.1f;

        Vec2i px = m_canvas.pixelSize();
        std::string stats = "FPS " + std::to_string((int)m_fpsSmooth) +
                            "  |  " + std::to_string((int)(dt * 1000.0f)) + " ms" +
                            "  |  ekran " + std::to_string(s.x) + "x" + std::to_string(s.y) +
                            "  (" + std::to_string(px.x) + "x" + std::to_string(px.y) + " px)";
        m_canvas.drawText({1, 0}, stats, {240, 162, 59}, bg);

        const auto& logs = log::recent();
        int first = std::max(0, (int)logs.size() - lines);
        for(int i = first; i < (int)logs.size(); i++)
        {
            const log::Entry& e = logs[i];
            Color c = e.level == log::Level::Error ? Color{255, 90, 80}
                    : e.level == log::Level::Warn  ? Color{240, 200, 80}
                    :                                Color{170, 170, 180};
            m_canvas.drawText({1, 1 + (i - first)}, e.text, c, bg);
        }
    }
}