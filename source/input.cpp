#include "include/input.h"


#ifdef _WIN32
    #pragma comment(lib, "user32.lib")
#endif

namespace engine
{
    #ifdef _WIN32
    static bool windowsKey(char32_t code, Key& k)
    {
        switch(code)
        {
            case 72: k = Key::Up;       return true;
            case 80: k = Key::Down;     return true;
            case 75: k = Key::Left;     return true;
            case 77: k = Key::Right;    return true;
            case 71: k = Key::Home;     return true;
            case 79: k = Key::End;      return true;
            case 73: k = Key::PageUp;   return true;
            case 81: k = Key::PageDown; return true;
            case 82: k = Key::Insert;   return true;
            case 83: k = Key::Delete;   return true;
            case 133: k = Key::F11;     return true;
            case 134: k = Key::F12;     return true;
        }
        if(code >= 59 && code <= 68)    //F1..F10 maja kody 59..68 po kolei
        {
            k = static_cast<Key>(static_cast<int>(Key::F1) + (code - 59));
            return true;
        }
        return false;
    }
    #else
    static bool linuxKey(char32_t last, int num, Key& k)
    {
        switch(last)
        {
            case 'A': k = Key::Up;    return true;
            case 'B': k = Key::Down;  return true;
            case 'C': k = Key::Right; return true;
            case 'D': k = Key::Left;  return true;
            case 'H': k = Key::Home;  return true;
            case 'F': k = Key::End;   return true;
            case 'P': k = Key::F1;    return true;
            case 'Q': k = Key::F2;    return true;
            case 'R': k = Key::F3;    return true;
            case 'S': k = Key::F4;    return true;
            case '~': break;                         //ESC [ liczba ~
            default:  return false;
        }
        switch(num)
        {
            case 1: case 7: k = Key::Home;     return true;
            case 2:         k = Key::Insert;   return true;
            case 3:         k = Key::Delete;   return true;
            case 4: case 8: k = Key::End;      return true;
            case 5:         k = Key::PageUp;   return true;
            case 6:         k = Key::PageDown; return true;
            case 11: k = Key::F1;  return true;  case 12: k = Key::F2;  return true;
            case 13: k = Key::F3;  return true;  case 14: k = Key::F4;  return true;
            case 15: k = Key::F5;  return true;  case 17: k = Key::F6;  return true;
            case 18: k = Key::F7;  return true;  case 19: k = Key::F8;  return true;
            case 20: k = Key::F9;  return true;  case 21: k = Key::F10; return true;
            case 23: k = Key::F11; return true;  case 24: k = Key::F12; return true;
        }
        return false;
    }
    #endif



    static char32_t lower(char32_t c) { return (c >= 'A' && c <= 'Z') ? c + 32 : c; }
    
    Input::Input(Terminal& terminal) : m_terminal(terminal) {}

    void Input::update()
    {
        m_events.clear();

        std::vector<char32_t> bytes;
        char32_t b;
        while(m_terminal.readByte(b))
            bytes.push_back((b));
        
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

    void Input::parse(const std::vector<char32_t>& bytes)
    {
        size_t n = bytes.size();

        for(size_t i = 0; i< n; i++)
        {
            char32_t b = bytes[i];

            #ifdef _WIN32
            if((b == 0 || b == 224) && i + 1 < n)   //Windows: prefiks 0 albo 224, potem kod klawisza
            {
                Key k;
                if(windowsKey(bytes[++i], k))
                    m_events.push_back({k});
                continue;
            }
            #else
            if(b == 27 && i + 1 < n && (bytes[i + 1] == '[' || bytes[i + 1] == 'O'))   //Linux: ESC [ ... albo ESC O ...
            {
                size_t j = i + 2;
                int num = 0;                                            //pierwsza liczba, np. 15 w ESC[15~
                bool firstNum = true;
                while(j < n && ((bytes[j] >= '0' && bytes[j] <= '9') || bytes[j] == ';'))
                {
                    if(bytes[j] == ';') firstNum = false;               //po ; sa modyfikatory (Ctrl/Shift) - ignorujemy
                    else if(firstNum) num = num * 10 + (bytes[j] - '0');
                    j++;
                }
                if(j < n)
                {
                    Key k;
                    if(linuxKey(bytes[j], num, k))
                        m_events.push_back({k});
                    i = j;
                    continue;
                }
            }
            #endif
            
            switch(b)
            {
                case 27:                m_events.push_back({Key::Escape}); break;
                case '\r': case '\n':   m_events.push_back({Key::Enter}); break;
                case 8: case 127:       m_events.push_back({Key::Backspace}); break;
                case '\t':              m_events.push_back({Key::Tab}); break;
                default:  
                    if(b >= 32 && b != 127)
                        m_events.push_back({Key::Char, b});
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

    bool Input::pressed(char32_t c) const
    {
        for(const KeyEvent& e : m_events)
            if(e.key == Key::Char && lower(e.ch) == lower(c))
                return true;
        return false;
    }

    int Input::keyId(Key k, char32_t c)
    {
        if (k == Key::Char)
            return lower(c);   
        return -1 - static_cast<int>(k);                        // klawisze specjalne
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
            case Key::Home:      vk = VK_HOME;   break;
            case Key::End:       vk = VK_END;    break;
            case Key::PageUp:    vk = VK_PRIOR;  break;
            case Key::PageDown:  vk = VK_NEXT;   break;
            case Key::Insert:    vk = VK_INSERT; break;
            case Key::Delete:    vk = VK_DELETE; break;
            default:
                if(k >= Key::F1 && k <= Key::F12)   //F1..F12 po kolei, tak jak VK_F1..VK_F12
                    vk = VK_F1 + (static_cast<int>(k) - static_cast<int>(Key::F1));
                else
                    return false;
        }
        return windowFocused() && (GetAsyncKeyState(vk) & 0x8000);
    }

    bool Input::held(char32_t c) const
    {
        SHORT vk = VkKeyScanW(static_cast<wchar_t>(c));                 // znak -> kod klawisza
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
    bool Input::held(char32_t c) const { return heldById(keyId(Key::Char, c)); }
#endif
}