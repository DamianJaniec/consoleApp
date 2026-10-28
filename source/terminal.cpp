#include "include/terminal.h" //windows.h termios.h
#include <cstdio>

#ifdef _WIN32
    #include <conio.h>  //kbhit, _getch_
#else
    #include <unistd.h> //read STDIN_FILENO
    #include <sys/ioctl.h> //ioctl
#endif

namespace engine
{
    Terminal::Terminal()
    {
        setvbuf(stdout,nullptr,_IOFBF,1 << 20);
        //ustawienie bufora, stdout - standarodwe wyjscie
        //nullptr - bez sztywnej alokacji bufora
        //_IOFBF - full buffering, wysylaj sam gdy sie zapelni lub fflush
        //1^20 ~ 1M bjatów czyli 1MB

        #ifdef _WIN32
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        HANDLE in  = GetStdHandle(STD_INPUT_HANDLE);

        GetConsoleMode(out, &m_oldOutMode);
        GetConsoleMode(in,  &m_oldInMode);
        m_oldCodePage = GetConsoleOutputCP();
        //obecne ustawienie terminala

        SetConsoleMode(out, m_oldOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        //Wymagane aby korzystac z pelnej palety rgb (domyslnie tylko 16)
        SetConsoleMode(in,  m_oldInMode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
        SetConsoleOutputCP(CP_UTF8);                 // pod znaki typu ▀
        //WAŻNE PRZETESOTWAĆ DOKŁADNIE // ostatnio jak robiłem coś podobnego to nie mogłem uruchomić


        #else
            tcgetattr(STDIN_FILENO, &m_oldSettings); //obecne ustawienia ternminala

            termios raw = m_oldSettings;
            raw.c_lflag &= ~(ICANON | ECHO);             // bez Entera i bez echa
            raw.c_cc[VMIN]  = 0;                         // read() nie czeka
            raw.c_cc[VTIME] = 0;                         // nie czekaj
            tcsetattr(STDIN_FILENO, TCSANOW, &raw); //wczytaj ustawienia
        #endif

        printf("\x1b[?1049h");     // alternatywny ekran
        printf("\x1b[?25l");       // ukryj kursor
        printf("\x1b[2J\x1b[H");   // wyczyść, kursor w róg
        flush();

    }
    Terminal::~Terminal()
    {
        // najpierw kody ANSI, póki terminal jeszcze je rozumie
        printf("\x1b[0m");         // reset kolorów
        printf("\x1b[?25h");       // pokaż kursor
        printf("\x1b[?1049l");     // wróć do normalnego ekranu
        flush();

        // potem przywracamy ustawienia sprzed startu
    #ifdef _WIN32
        SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), m_oldOutMode);
        SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),  m_oldInMode);
        SetConsoleOutputCP(m_oldCodePage);
    #else
        tcsetattr(STDIN_FILENO, TCSANOW, &m_oldSettings);
    #endif
    }
    Vec2i Terminal::size() const
    {
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
            return {120,30};                         // awaryjnie, jak się nie uda
        return { csbi.srWindow.Right  - csbi.srWindow.Left + 1,
                 csbi.srWindow.Bottom - csbi.srWindow.Top  + 1 };
    #else
        winsize w{};
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != 0 || w.ws_col == 0)
            return {80, 24};
        return { w.ws_col, w.ws_row };
    #endif
    }

    bool Terminal::readByte(char& out)
    {
    #ifdef _WIN32
        if (!_kbhit())
            return false;
        out = static_cast<char>(_getch());
        return true;
    #else
        return read(STDIN_FILENO, &out, 1) == 1;
    #endif
    }

    void Terminal::flush()
    {
        fflush(stdout);
    }
}
