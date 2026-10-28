#include "include/renderer.h"
#include "include/ansi.h"

namespace engine
{
    Renderer::Renderer(Terminal& _terminal) : m_terminal(_terminal)
    {
    }

    void Renderer::present(const Canvas& frame)
    {
        if(m_previous.getSize() != frame.getSize())
        {
            m_previous.setSize(frame.getSize());
            m_forcedDraw = true;
        }

        if(m_forcedDraw)
        {
            ansi::resetColor();
            ansi::clearScreen();
            m_cursor = {-1, -1};
            m_lastFg = {-1,-1,-1};
            m_lastBg = {-1,-1,-1};
        }
        ansi::beginFrame();

        for(int y=0; y<frame.getSize().y; y++)
        {
            for(int x=0; x<frame.getSize().x; x++)
            {
                Cell cell = frame.getCell({x,y});

                if(!m_forcedDraw && cell == m_previous.getCell({x,y}))
                    continue;
                
                drawCell({x,y},cell);
            }
        }

        ansi::endFrame();
        m_terminal.flush();
        m_previous = frame;
        m_forcedDraw = false;
    }
    void Renderer::drawCell(Vec2i coords, const Cell& cell)
    {
        if (m_cursor != coords)
        {
            ansi::gotoxy(coords);
            m_cursor = coords;
        }
        if (m_lastBg != cell.bg)
        {
            m_lastBg = cell.bg;
            ansi::setBg(cell.bg);
        }
        if (m_lastFg != cell.fg)
        {
            m_lastFg = cell.fg;
            ansi::setFg(cell.fg);
        }
        putchar(cell.c);
        
        if(coords.x == m_previous.getSize().x - 1)
            m_cursor = {-1, -1};
        else
            m_cursor.x++;
    }
    void Renderer::invalidate()
    {
        m_forcedDraw = true;
    }
}


/*
lass Renderer
    {
        
    public:

        explicit Renderer(Terminal &_terminal);

        void present(const Canvas& frame);
        void invalidate();


    private:
       
        void moveCursor(Vec2i pos);
        void setColors(Color fg, Color bg);
        void drawCell(Vec2i coords, const Cell& cell);
        bool isSameSize(const Canvas& frame);
        bool isSameCell(const Cell c, const Cell t);


        Terminal& m_terminal; 
        Canvas m_previous;
        bool m_forcedDraw = true; //flaga
        Color m_lastFg = {-1,-1,-1};
        Color m_lastBg = {-1,-1,-1}; //nie znane kolory
        Vec2i m_cursor = {-1, -1}; //-1 -1 nie wiadomo gdzie jest

    };
*/