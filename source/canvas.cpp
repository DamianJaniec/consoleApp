#include "include/canvas.h"
#include "include/utf8.h"

#include <cstdlib>   // std::abs

namespace engine
{
    static constexpr char32_t HALF_TOP = U'▀';
    
    
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
    void Canvas::drawText(Vec2i coords, const std::string &text, Color fg, Color bg)
    {
        std::u32string chars = utf8::decode(text);
        for(int i=0; (int)i<chars.size(); i++)
            setCell({coords.x + i, coords.y},{chars[i],fg,bg});
            //setCell({coords.x+i,coords.y},{text[i],fg,bg});
    }
    void Canvas::plot(Vec2i coords, const Brush &b)
    {
        if(b.pixel)
            setPixel(coords,b.color);
        else
            setCell(coords,b.cell);
    }
    void Canvas::line(Vec2i coords, Vec2i target, const Brush& b) // Bresenham
    {
        int dx = std::abs(target.x - coords.x);
        int dy = -std::abs(target.y - coords.y);
        int sx = coords.x < target.x ? 1 : -1;
        int sy = coords.y < target.y ? 1 : -1;
        int err = dx + dy;

        while(true)
        {
            plot(coords, b);
            if(coords == target) 
                break;
            int e2 = 2 * err;
            if (e2 >= dy) {err += dy; coords.x += sx;}
            if (e2 <= dx) {err += dx; coords.y += sy;}
        }
    }
    void Canvas::rect(Vec2i coords, Vec2i _size, const Brush &b)
    {
        if(_size.x <= 0 || _size.y <= 0) return;
        int x2 = coords.x + _size.x - 1;
        int y2 = coords.y + _size.y - 1;
        line({coords.x, coords.y},  {x2, coords.y},b);
        line({coords.x, y2},        {x2,y2},b);
        line({coords.x, coords.y},  {coords.x, y2},b);
        line({x2, coords.y},        {x2, y2},b);
    }
    void Canvas::rectFill(Vec2i coords, Vec2i _size, const Brush &b)
    {
        for(int y = coords.y; y< coords.y + _size.y; y++)
            for(int x = coords.x; x<coords.x + _size.x; x++)
                plot({x,y},b);
    }
    void Canvas::circle(Vec2i coords, int radius, const Brush &b)
    {
        if(radius < 0) return;
        int x = radius, y = 0, err = 1 - radius;
        while(x>=y)
        {
            plot({coords.x + x, coords.y + y}, b); plot({coords.x + y, coords. y + x},b);
            plot({coords.x - y, coords.y + x}, b); plot({coords.x - x, coords. y + y},b);
            plot({coords.x - x, coords.y - y}, b); plot({coords.x - y, coords. y - x},b);
            plot({coords.x + y, coords.y - x}, b); plot({coords.x + x, coords. y - y},b);
            y++;
            if(err<0) err += 2 * y + 1;
            else 
            {
                x--;
                err += 2* (y-x) + 1;
            }
        }
    }
    void Canvas::circleFill(Vec2i coords, int radius, const Brush &b)
    {
        if(radius < 0) return;
        for(int dy = -radius; dy <= radius; dy++)
            for(int dx = -radius; dx <= radius; dx++)
                if(dx*dx + dy*dy <= radius*radius + radius)
                    plot({coords.x + dx, coords.y + dy}, b);
    }

    //wywolania - znaki
    void Canvas::drawLine(Vec2i coords, Vec2i target, Cell c)   {line(coords,target,{false, c, {}});}
    void Canvas::drawRect(Vec2i coords, Vec2i _size, Cell c)     {rect(coords,_size,{false, c, {}});}
    void Canvas::fillRect(Vec2i coords, Vec2i _size, Cell c)     {rectFill(coords,_size,{false, c, {}});}
    void Canvas::drawCircle(Vec2i coords, int radius, Cell c)   {circle(coords,radius,{false, c, {}});}
    void Canvas::fillCircle(Vec2i coords, int radius, Cell c)   {circleFill(coords,radius,{false, c, {}});}

    //wywolania - pixele
    void Canvas::drawLinePx(Vec2i coords, Vec2i target, Color c)   {line(coords,target,{true, {}, c});}
    void Canvas::drawRectPx(Vec2i coords, Vec2i _size, Color c)     {rect(coords,_size,{true, {}, c});}
    void Canvas::fillRectPx(Vec2i coords, Vec2i _size, Color c)     {rectFill(coords,_size,{true, {}, c});}
    void Canvas::drawCirclePx(Vec2i coords, int radius, Color c)   {circle(coords,radius,{true, {}, c});}
    void Canvas::fillCirclePx(Vec2i coords, int radius, Color c)   {circleFill(coords,radius,{true, {}, c});}


    void Canvas::drawTexturePx(Vec2i coords, const Texture& tex)
    {
        Vec2i ts = tex.size();
        for (int y=0; y<ts.y; y++)
            for(int x=0; x<ts.x; x++)
            {
                if(!tex.visible({x,y}))
                    continue;
                setPixel({coords.x + x, coords.y + y},tex.colorAt({x,y}));
            }
    }
    void Canvas::drawAscii(Vec2i coords, const Ascii& tex,Color fg)
    {
        Vec2i ts = tex.size();
        for (int y=0; y<ts.y; y++)
            for(int x=0; x<ts.x; x++)
            {
                char32_t ch = tex.charAt({x,y});
                if (ch == U' ')
                    continue;
                
                Vec2i target = {coords.x + x, coords.y + y};
                Cell under = getCell(target);
                setCell(target, {ch, fg, under.bg});
            }
    }



    Vec2i Canvas::pixelSize() const
    {
        return {size.x, size.y * 2};
    }
    bool Canvas::pixelInBounds(Vec2i coords) const
    {
        return coords.x >= 0 && coords.y >= 0 && coords.x < size.x && coords.y < size.y * 2;
    }
    void Canvas::setPixel(Vec2i coords, Color color)
    {
        if(!pixelInBounds(coords))
            return;
        
        Cell &cell = cells[(coords.y / 2) * size.x + coords.x];

        if (cell.c != HALF_TOP)     // komórka nie jest jeszcze pikselowa
        {
            Color base = cell.bg;   // obie połówki dostają dotychczasowe tło
            cell.c  = HALF_TOP;
            cell.fg = base;
            cell.bg = base;
        }

        if (coords.y % 2 == 0)  cell.fg = color;   // górna połówka
        else                    cell.bg = color;   // dolna połówka
    }
    Color Canvas::getPixel(Vec2i coords) const
    {
        if (!pixelInBounds(coords))
            return {0, 0, 0};

        const Cell& cell = cells[(coords.y / 2) * size.x + coords.x];

        if (cell.c != HALF_TOP)     // zwykła komórka - cała ma kolor tła
            return cell.bg;

        return (coords.y % 2 == 0) ? cell.fg : cell.bg;
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