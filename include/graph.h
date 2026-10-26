//Basic graphic functions and libs

#pragma once

namespace graph
{

    void consoleInit();
    void consoleRestore();


    void gotoxy(int x, int y);
    void clearScreen();
    void showCursor(bool show);

    void setFg(int r, int g, int b);
    void setBg(int r, int g, int b);
    void resetColor();

    void getConsoleSize(int &cols, int &rows);
}