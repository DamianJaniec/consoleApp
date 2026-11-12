#include "include/engine.h"

class TestApp : public engine::App
{
    float r = 0, dr = 60;

    engine::Animation slimeJump{&resources().texture("assets/slime.png"), {16, 16}, 4, 8.0f};
    engine::Sprite    spr{&slimeJump, {10, 20}};
    engine::Sprite    ghost{&slimeJump,{20,30}};
    engine::Audio audio;
    int pip;

    void onStart() override
    {
        ghost.opacity = 0.3f;
        audio.play("assets/test.mp3");
        pip = audio.makeBeep(440.0f, 0.1f);
    }
    void onUpdate(float dt) override
    {
        r += dr * dt;
        if (r > 255) { r=255;   dr = -dr;}
        if (r < 0)   { r=0;     dr = -dr;}

        if (input().pressed('r'))      // R = przeladuj pliki
            resources().reloadAll();

        slimeJump.update(dt);


        if (input().pressed('1')) audio.playBeep(pip);         // normalnie
        if (input().pressed('2')) audio.playBeep(pip, 1.5f);   // wyzej
        if (input().pressed('3')) audio.playBeep(pip, 0.5f);   // oktawe nizej
    }
    void onDraw(engine::Canvas& c) override
    {
        c.clear({' ',{0,0,0},{(int)r,40,60}});
        spr.draw(c);
        ghost.draw(c);
    }
};

int main()
{
    TestApp app;
    app.run({120,40});
}