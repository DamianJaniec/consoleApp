#pragma once

#include <string>
#include "include/types.h"

namespace engine::ansi
{
    void gotoxy(Vec2i coords);
    void setFg(Color c);
    void setBg(Color c);
    void clearScreen();
    void resetColor();
    void showCursor(bool show);

    void beginFrame();
    void endFrame();

}