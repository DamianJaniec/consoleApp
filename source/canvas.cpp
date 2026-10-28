#include "include/canvas.h"

namespace engine
{
    Canvas::Canvas()
    {

    }
    void Canvas::setSize(Vec2i _size)
    {
        size = _size;
        cells.assign(size.x * size.y, Cell{});
    }
    Vec2i Canvas::getSize() const
    {
        return size;
    }

    void Canvas::clear(Cell c)
    {
        for (Cell& cell : cells)
            cell = c;
    }
    bool Canvas::inBounds(Vec2i coords) const
    {
        if (coords.x < 0 || coords.y < 0 || coords.x >= size.x || coords.y >= size.y)
            return false;
        return true;
    }
    void Canvas::setCell(Vec2i coords, Cell c)
    {
        if(!inBounds(coords))
            return;
        cells[coords.y * size.x + coords.x] = c;
    }
    Cell Canvas::getCell(Vec2i coords) const
    {
        if(!inBounds(coords))
            return Cell{};
        return cells[coords.y * size.x + coords.x];
    }
    void Canvas::fillRect(Vec2i coords, int w, int h, Cell c)
    {
        for(int y=coords.y; y<(coords.y+h); y++)
            for (int x=coords.x; x<=(coords.x+w); x++)
               setCell({x,y},c); 
    }
    void Canvas::drawText(Vec2i coords, const std::string &text, Color fg, Color bg)
    {
        for(int i=0; i<text.length(); i++)
            setCell({coords.x+i,coords.y},{text[i],fg,bg});
    }
    void Canvas::drawLine(Vec2i coords, Vec2i target, Cell c)
    {
        //pozniej wypelnic
    }
}

/*
void setSize(Vec2i _size);
            Vec2i getSize() const;

            void clear(Cell c);
            void setCell(Vec2i coords, Cell c);
            Cell getCell(Vec2i coords) const;

            void fillRect(Vec2i coords, int w, int h, Cell c);
            void drawText(Vec2i coords, Cell c);
            void drawText(Vec2i coords, std::string &text, Color fg, Color bg);
            void drawLine(Vec2i coords, Vec2i target, Cell c);
*/