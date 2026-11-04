#include "include/engine.h"

class TestApp : public engine::App
{
    engine::Texture testrgb;
    float r = 0, dr = 60;

    engine::Texture slime;
    engine::Animation slimeJump{&slime,{16,16},4,8.0f,true};
    engine::Sprite spr{&slimeJump, {10,20}};

    void onStart() override
    {
        testrgb.load("assets/testrgb.txt");

        slime.load("assets/slime.png");
        
    }
    void onUpdate(float dt) override
    {
        r += dr * dt;
        if (r > 255) { r=255;   dr = -dr;}
        if (r < 0)   { r=0;     dr = -dr;}

        if(input().held('w'))
            r = 255;

        slimeJump.update(dt);
    }
    void onDraw(engine::Canvas& c) override
    {
        c.clear({' ',{0,0,0},{(int)r,40,60}});
        c.drawText({0,2},"Zażółć gęślą jaźń",{255,255,255},{0,0,0});
        c.drawTexturePx({10,20},{33,27},testrgb);
        spr.draw(c);
    }
};

int main()
{
    TestApp app;
    app.run({120,40});
}