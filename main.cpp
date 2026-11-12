#include "include/engine.h"

class TestApp : public engine::App
{
    float r = 0, dr = 60;

    engine::Animation slimeJump{&resources().texture("assets/slime.png"), {16, 16}, 4, 8.0f};
    engine::Sprite    spr{&slimeJump, {10, 20}};

    void onStart() override
    {
        
    }
    void onUpdate(float dt) override
    {
        r += dr * dt;
        if (r > 255) { r=255;   dr = -dr;}
        if (r < 0)   { r=0;     dr = -dr;}

        if (input().pressed('r'))      // R = przeladuj pliki
            resources().reloadAll();

        slimeJump.update(dt);
    }
    void onDraw(engine::Canvas& c) override
    {
        c.clear({' ',{0,0,0},{(int)r,40,60}});
        spr.draw(c);
    }
};

int main()
{
    TestApp app;
    app.run({120,40});
}