#include "include/engine.h"


class TestApp : public engine::App
{
    float r = 0, dr = 60;

    engine::Animation slimeJump{&resources().texture("assets/slime.png"), {16, 16}, 4, 8.0f};
    engine::Sprite    spr{&slimeJump, {56, 48}};
    engine::Sprite    ghost{&slimeJump,{76, 46}};
    engine::Audio audio;
    std::string test;
    engine::Texture& cave = resources().texture("assets/cave.png");
    
    //int beep = audio.makeBeep(570.0f,0.2f);

    void onStart() override
    {
       ghost.opacity = 0.3f;
       
    }
    void onUpdate(float dt) override
    {
        r += dr * dt;
        if (r > 255) { r=255;   dr = -dr;}
        if (r < 0)   { r=0;     dr = -dr;}

        if (input().pressed(engine::Key::F5))     resources().reloadAll();
        if (input().pressed(engine::Key::Delete)) test.clear();
        
        slimeJump.update(dt);
        
        for(const auto& e : input().events())
        {
            if(e.key == engine::Key::Char)
                engine::utf8::encode(e.ch, test);

            if(e.key == engine::Key::Backspace && !test.empty())
            {
                test.pop_back();
                while (!test.empty() && ((unsigned char)test.back() & 0xC0) == 0x80)   // polska litera = 2 bajty
                    test.pop_back();
            }
        }
        
        
        
    }
    void onDraw(engine::Canvas& c) override
    {
        engine::Color bg = {20, 20, 28};
        c.clear({' ',{0,0,0},{(int)r,40,60}});
        c.drawTexturePx({0,0},cave);
        spr.draw(c);
        ghost.draw(c);
        //c.fillRectPx({15,30},{20,10},{200,100,67,100});
        //c.fillCirclePx({20,30},8,{255,200,0,128});
        //c.drawText({2,2},"Test",{255,255,255},{0,0,0,0});
        //c.drawText({2,2},"Test2",{255,255,255},{0,0,0,150});
        c.drawText({5,30},test,{120,120,120},bg);
        
    }
};

int main()
{
    TestApp app;
    app.run({120,40});
}