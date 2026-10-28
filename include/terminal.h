#pragma once
#include "include/types.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <termios.h>
#endif


namespace engine
{
    class Terminal
    {
        public:
        Terminal(); //Init
        ~Terminal(); //Przywraca domyślne ustawienia terminala

        Terminal(const Terminal&) = delete; //Maksmyalnie tylko 1 terminal
        Terminal& operator = (const Terminal&) = delete;

        Vec2i size() const; //rozmiar okna
        bool readByte(char &out); //funkcja analogiczna do kbhit, ale uniwersalna dla win oraz linux
        void flush(); //wypachanie bufora na ekran

        private:
        #ifdef _WIN32 //zmienna na stare ustawienia terminala
            DWORD m_oldOutMode = 0;
            DWORD m_oldInMode = 0;
            UINT  m_oldCodePage = 0;
        #else
            termios m_oldSettings{};
        #endif
    };
}