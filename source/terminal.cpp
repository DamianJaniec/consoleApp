#include "include/terminal.h" //windows.h termios.h
#include <cstdio>
#include <atomic>

#ifdef _WIN32
    #include <conio.h>  //kbhit, _getch_
    #include <timeapi.h>
    #pragma comment(lib, "winmm.lib")
#else
    #include <unistd.h> //read STDIN_FILENO
    #include <sys/ioctl.h> //ioctl
    #include <csignal>
#endif

namespace engine
{
    std::atomic<bool> g_quit{false};
    static_assert(std::atomic<bool>::is_always_lock_free,"atomic<bool> musi byc lock-free aby byl bezpieczny w handlerze");

    #ifdef _WIN32
        BOOL WINAPI ctrlHandler(DWORD type)
        {
            switch(type)
            {
                case CTRL_C_EVENT:
                case CTRL_BREAK_EVENT:
                    g_quit = true;
                    return true;
                default:
                    return false;
            }
        }
    #else
        void signalHandler(int) {g_quit = true;}
    #endif

    bool Terminal::quitRequested() {return g_quit;}

    Terminal::Terminal()
    {
        setvbuf(stdout,nullptr,_IOFBF,1 << 20);
        //ustawienie bufora, stdout - standarodwe wyjscie
        //nullptr - bez sztywnej alokacji bufora
        //_IOFBF - full buffering, wysylaj sam gdy sie zapelni lub fflush
        //1^20 ~ 1M bjatów czyli 1MB

        #ifdef _WIN32
        timeBeginPeriod(1); //ustawienie dokladzniejszego timera
        SetConsoleCtrlHandler(ctrlHandler, TRUE);

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
            struct sigaction sa{};
            sa.sa_handler = signalHandler;
            sigemptyset(&sa.sa_mask);
            sigaction(SIGINT,  &sa, nullptr);  // Ctrl+C
            sigaction(SIGTERM, &sa, nullptr);  // kill
            sigaction(SIGHUP,  &sa, nullptr);  // zamkniecie terminala
        
        
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
        timeEndPeriod(1);
        SetConsoleCtrlHandler(ctrlHandler, FALSE);

        SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), m_oldOutMode);
        SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),  m_oldInMode);
        SetConsoleOutputCP(m_oldCodePage);
    #else
        tcsetattr(STDIN_FILENO, TCSANOW, &m_oldSettings);
        signal(SIGINT,  SIG_DFL);
        signal(SIGTERM, SIG_DFL);
        signal(SIGHUP,  SIG_DFL);
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

    bool Terminal::readByte(char32_t& out)
    {
    #ifdef _WIN32
        if (m_pending)                        //najpierw oddaj odlozony kod
        {
            out = m_pending;
            m_pending = 0;
            return true;
        }
        if (!_kbhit())
            return false;

        out = static_cast<char32_t>(_getwch());
        if (out == 0 || out == 0xE0)          //klawisz specjalny - kod zawsze idzie zaraz za prefiksem
            m_pending = static_cast<char32_t>(_getwch());   //czekamy na niego od razu, bez _kbhit
        return true;
    #else
        unsigned char b;
        if(read(STDIN_FILENO, &b,1)!=1) return false;

        std::string s(1,(char)b);
        int len = b < 0x80 ? 1 : b< 0xE0 ? 2 : b < 0xF0 ? 3 : 4;
        for(int k = 1; k< len; k++)
        {
            char c;
            if(read(STDIN_FILENO, &c, 1)==1) s+=c;
        }
        out = utf8::decode(s)[0];
        return true;
    #endif
    }

    void Terminal::flush()
    {
        fflush(stdout);
    }
    void Terminal::requestSize(Vec2i s)
    {
    #ifdef _WIN32
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD max = GetLargestConsoleWindowSize(out);   // ile się zmieści na ekranie
        if (s.x > max.X) s.x = max.X;
        if (s.y > max.Y) s.y = max.Y;

        SMALL_RECT tiny = {0, 0, 1, 1};                  // najpierw zmniejsz okno,
        SetConsoleWindowInfo(out, TRUE, &tiny);          // bo bufor nie może być mniejszy od okna
        COORD buf = {(SHORT)s.x, (SHORT)s.y};
        SetConsoleScreenBufferSize(out, buf);
        SMALL_RECT win = {0, 0, (SHORT)(s.x - 1), (SHORT)(s.y - 1)};
        SetConsoleWindowInfo(out, TRUE, &win);
    #else
        printf("\x1b[8;%d;%dt", s.y, s.x);               // xterm: "ustaw rozmiar okna"
        flush();
    #endif
    }
}
