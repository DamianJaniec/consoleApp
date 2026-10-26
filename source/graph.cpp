#include "graph.h"
#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
    static termios g_orig;
    static bool g_raw = false;
#endif


void graph::consoleInit()
{
    #ifdef _WIN32
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        GetConsoleMode(h, &mode);
        SetConsoleMode(h,mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    #else
        tcgetattr(STDIN_FILENO, &g_orig);
        termios raw = g_orig;
        raw.c_lflag &= ~(ICANON | ECHO);
        raw.c_cc[VMIN] = 0; //read() nie czeka na znak
        raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
        g_raw = true;
    #endif
    atexit(consoleRestore);
    showCursor(false);
}

void graph::consoleRestore()
{
    resetColor();
    showCursor(true);
    #ifndef _WIN32
        if(g_raw)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &g_orig);
            g_raw = false;
        }
    #endif
    fflush(stdout);
}
void graph::gotoxy(int x, int y)
{
    printf("\x1b[%d;%dH",y+1,x+1);
}
void graph::clearScreen()
{
    printf("\x1b[2J\x1b[H");
}
void graph::showCursor(bool show)
{
    if(show)
        printf("\x1b[?25h");
    else
        printf("\x1b[?25l");
}
void graph::setFg(int r, int g, int b)
{
    printf("\x1b[38;2;%d;%d;%dm",r,g,b);
}
void graph::setBg(int r,int g, int b)
{
    printf("\x1b[48;2;%d;%d;%dm",r,g,b);
}
void graph::resetColor()
{
    printf("\x1b[0m");
}

void graph::getConsoleSize(int& cols, int& rows)
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    cols = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
#else
    winsize w{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    cols = w.ws_col;
    rows = w.ws_row;
#endif
}