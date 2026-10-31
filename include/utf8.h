#pragma once
#include <string>

namespace engine::utf8
{
    inline void encode(char32_t cp, std::string & out)
    {
        if (cp < 128) //1 bajt
            out += static_cast<char>(cp);
        else if(cp < 2048) //2 bajty
        {
            out += static_cast<char>(192 | (cp >> 6)); //przesuniete
            out += static_cast<char>(128 | (cp & 63)); //ostatnie bity
        }
        else if(cp < 65536) //3 bajty
        {
            out += static_cast<char>(224 | (cp>>12));
            out += static_cast<char>(128 | ((cp>>6) & 63));
            out += static_cast<char>(128 | (cp & 63));
        }
        else    //4 bajty
        {
            out += static_cast<char>(240 | (cp>>18));
            out += static_cast<char>(128 | ((cp>>12) & 63));
            out += static_cast<char>(128 | ((cp>> 6) & 63));
            out += static_cast<char>(128 | (cp & 63));
        }
    }

    inline std::u32string decode(const std::string& s)
    {
        std::u32string out;
        size_t i = 0, n = s.size();

        while(i<n)
        {
            unsigned char b = static_cast<unsigned char>(s[i]);
            char32_t cp;
            int len;

            if          (b < 128)        {cp = b;         len = 1;}
            else if     ((b>>5)==6)      {cp = b & 31;    len = 2;}
            else if     ((b>>4)==14)     {cp = b & 15;    len = 3;}
            else if     ((b>>3)==30)     {cp = b & 7;     len = 4;}
            else {out += U"?"; i++; continue;}

            if(i+len > n) {out +=U"?"; break;}

            bool ok = true;
            for(int k = 1; k < len; k++)
            {
                unsigned char c = static_cast<unsigned char>(s[i + k]);
                if((c&192)!=128) {ok=false; break;}
                cp = (cp<<6) | (c & 63);
            }

            if (ok) {out += cp; i+=len;}
            else    {out +=U"?"; i++;}
        }
        return out;
    }

}