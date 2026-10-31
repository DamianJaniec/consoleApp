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
        Enter, Escape, Backspace, Tab //wybrane klawisze specjalne
    };

    struct KeyEvent
    {
        Key key;
        char ch = 0;
    };

    class Input
    {
        public:
        explicit Input(Terminal& terminal);

        void update();

        bool pressed(Key k) const;  //Key::UP
        bool pressed(char c) const; //np. char 'w'
        bool held(Key k) const;
        bool held(char c) const;
        const std::vector<KeyEvent>& events() const {return m_events;}
        
        private:
        void parse(const std::vector<unsigned char>& bytes);

        using clock = std::chrono::steady_clock;
        struct HoldInfo { clock::time_point first; clock::time_point last; };
        std::unordered_map<int, HoldInfo> m_hold;     // tylko Linux

        static int keyId(Key k, char c = 0);
        bool heldById(int id) const;

        Terminal& m_terminal;
        std::vector<KeyEvent> m_events;

    };
}