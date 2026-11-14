#pragma once

#include <vector>
#include <chrono>
#include <unordered_map>
#include "include/terminal.h"

namespace engine
{
    enum class Key
    {
        Char,   //zwykly znak jakich wiele w swiecie
        Up, Down, Left, Right, //strzalki
        Enter, Escape, Backspace, Tab, //wybrane klawisze specjalne
        Home, End, PageUp, PageDown, Insert, Delete,
        F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12
    };

    struct KeyEvent
    {
        Key key;
        char32_t ch = 0;
    };

    class Input
    {
        public:
        explicit Input(Terminal& terminal);

        void update();

        bool pressed(Key k) const;  //Key::UP
        bool pressed(char32_t c) const; //np. char 'w'
        bool held(Key k) const;
        bool held(char32_t c) const;
        const std::vector<KeyEvent>& events() const {return m_events;}
        
        private:
        void parse(const std::vector<char32_t>& bytes);

        using clock = std::chrono::steady_clock;
        struct HoldInfo { clock::time_point first; clock::time_point last; };
        std::unordered_map<int, HoldInfo> m_hold;     // tylko Linux

        static int keyId(Key k, char32_t c = 0);
        bool heldById(int id) const;

        Terminal& m_terminal;
        std::vector<KeyEvent> m_events;

    };
}