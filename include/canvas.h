#pragma once

#include "include/types.h"
#include <iostream>
#include <vector>

namespace engine
{
    class Canvas
    {
        engine::Vec2i size;
        std::vector<Cell> cells;

        public:
            Canvas();

            void setSize(Vec2i _size);
            Vec2i getSize() const;

            void clear(Cell c);
            void setCell(Vec2i coords, Cell c);
            Cell getCell(Vec2i coords) const;

            bool inBounds(Vec2i coords) const;

            void fillRect(Vec2i coords, int w, int h, Cell c);
            void drawText(Vec2i coords, const std::string &text, Color fg, Color bg);
            void drawLine(Vec2i coords, Vec2i target, Cell c);


    };
}