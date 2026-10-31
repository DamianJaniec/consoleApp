#include "include/input.h"
#include <cctype>

#ifdef _WIN32
    #pragma comment(lib, "user32.lib")
#endif

namespace engine
{
    Input::Input(Terminal& terminal) : m_terminal(terminal) {}

    void Input::update()
    {
        m_events.clear();

        std::vector<unsigned char> bytes;
        char b;
        while(m_terminal.readByte(b))
            bytes.push_back(static_cast<unsigned char>(b));
        
        parse(bytes);
        #ifndef _WIN32
            auto now = clock::now();
            for (const KeyEvent& e : m_events)
            {
                int id = keyId(e.key, e.ch);
                auto it = m_hold.find(id);

                if (it == m_hold.end() || !heldById(id))   // nowe wciśnięcie
                    m_hold[id] = {now, now};
                else                                        // kolejne powtórzenie
                    it->second.last = now;
            }
        #endif
    }

    void Input::parse(const std::vector<unsigned char>& bytes)
    {
        size_t n = bytes.size();

        for(size_t i = 0; i< n; i++)
        {
            unsigned char b = bytes[i];

            #ifdef _WIN32
            if((b==0 || b==224) && i + 1 < n)
            {
                switch(bytes[++i])
                {
                    case 72: m_events.push_back({Key::Up});    break;
                    case 80: m_events.push_back({Key::Down});  break;
                    case 75: m_events.push_back({Key::Left});  break;
                    case 77: m_events.push_back({Key::Right}); break;
                    default: break;
                }
                continue;
            }

            #else
                // Linux: sekwencje ESC [ X  albo  ESC O X
                if (b == 27 && i + 2 < n && (bytes[i + 1] == '[' || bytes[i + 1] == 'O'))
                {
                    switch (bytes[i + 2])
                    {
                        case 'A': m_events.push_back({Key::Up});    break;
                        case 'B': m_events.push_back({Key::Down});  break;
                        case 'C': m_events.push_back({Key::Right}); break;
                        case 'D': m_events.push_back({Key::Left});  break;
                        default: break;
                    }
                    i += 2;
                    continue;
                }
            #endif
            
            switch(b)
            {
                case 27:                m_events.push_back({Key::Escape}); break;
                case '\r': case '\n':   m_events.push_back({Key::Enter}); break;
                case 8: case 127:       m_events.push_back({Key::Backspace}); break;
                case '\t':              m_events.push_back({Key::Tab}); break;
                default:
                    if(b>= 32 && b < 127)
                        m_events.push_back({Key::Char, static_cast<char>(b)});
                    break;
            }

        }

        
    }

    bool Input::pressed(Key k) const
    {
        for (const KeyEvent& e: m_events)
            if(e.key == k) return true;
        return false;
    }

    bool Input::pressed(char c) const
    {
        for(const KeyEvent& e : m_events)
            if(e.key == Key::Char && std::tolower(e.ch) == std::tolower(c))
                return true;
            return false;
    }

    int Input::keyId(Key k, char c)
    {
        if (k == Key::Char)
            return std::tolower(static_cast<unsigned char>(c));   // 0..255
        return 1000 + static_cast<int>(k);                        // klawisze specjalne
    }

    #ifdef _WIN32
    static bool windowFocused()
    {
        HWND fg  = GetForegroundWindow();
        HWND con = GetConsoleWindow();
        // conhost: okno konsoli; Windows Terminal: okno-właściciel pseudokonsoli
        return fg == con || fg == GetAncestor(con, GA_ROOTOWNER);
    }

    bool Input::held(Key k) const
    {
        int vk = 0;
        switch (k)
        {
            case Key::Up:        vk = VK_UP;     break;
            case Key::Down:      vk = VK_DOWN;   break;
            case Key::Left:      vk = VK_LEFT;   break;
            case Key::Right:     vk = VK_RIGHT;  break;
            case Key::Enter:     vk = VK_RETURN; break;
            case Key::Escape:    vk = VK_ESCAPE; break;
            case Key::Backspace: vk = VK_BACK;   break;
            case Key::Tab:       vk = VK_TAB;    break;
            default: return false;
        }
        return windowFocused() && (GetAsyncKeyState(vk) & 0x8000);
    }

    bool Input::held(char c) const
    {
        SHORT vk = VkKeyScanA(c);                 // znak -> kod klawisza
        if (vk == -1) return false;
        return windowFocused() && (GetAsyncKeyState(vk & 0xFF) & 0x8000);
    }

    bool Input::heldById(int) const { return false; }   // nieużywane na Windowsie

#else
    bool Input::heldById(int id) const
    {
        auto it = m_hold.find(id);
        if (it == m_hold.end()) return false;

        auto now = clock::now();
        const HoldInfo& h = it->second;

        bool repeating = (h.last - h.first) > std::chrono::milliseconds(50);
        auto timeout   = repeating ? std::chrono::milliseconds(100)    // autorepeat leci - szybko wykryj puszczenie
                                   : std::chrono::milliseconds(600);   // przeczekaj początkową przerwę
        return now - h.last < timeout;
    }

    bool Input::held(Key k)  const { return heldById(keyId(k)); }
    bool Input::held(char c) const { return heldById(keyId(Key::Char, c)); }
#endif
}