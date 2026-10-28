#include "include/ansi.h"
#include <cstdio>

namespace engine::ansi
{
    void gotoxy(Vec2i coords)
    {
        printf("\x1b[%d;%dH", coords.y + 1, coords.x + 1);
    }
    void clearScreen()
    {
        printf("\x1b[2J\x1b[H");
    }
    void showCursor(bool show)
    {
        if (show)
            printf("\x1b[?25h");
        else
            printf("\x1b[?25l");
    }
    void setFg(Color c)
    {
        printf("\x1b[38;2;%d;%d;%dm", c.r, c.g, c.b);
    }
    void setBg(Color c)
    {
        printf("\x1b[48;2;%d;%d;%dm", c.r, c.g, c.b);
    }
    void resetColor()
    {
        printf("\x1b[0m");
    }
    void beginFrame()
    {
        printf("\x1b[?2026h");
    }
    void endFrame()
    {
        printf("\x1b[?2026l");
    }
}