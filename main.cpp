#include "include/engine.h"
#include <cstdlib>

class TestApp : public engine::App
{
    float r = 0, dr = 60;

    engine::Animation slimeJump{&resources().texture("assets/slime.png"), {16, 16}, 4, 8.0f};
    engine::Sprite    spr{&slimeJump, {10, 20}};
    engine::Sprite    ghost{&slimeJump,{20,30}};
    engine::Audio audio;
    int beep;
    
    engine::Talk talk;
    engine::Voice glut, robot, postac;
    int line = 0;


    void onStart() override
    {
        audio.setVolume(0.8f);
        
        
        ghost.opacity = 0.3f;
        //audio.play("assets/test.mp3",0.4f);
        beep = audio.makeBeep(440.0f, 0.1f);

        int talkBeep = audio.makeBeep(570.0f, 0.06f);
        int robotBeep = audio.makeBeep(180.0f, 0.02f);
        
        glut = {talkBeep,1.3f,0.1f,25.0f};
        robot = {robotBeep,0.4f,0.0f,12.0f};
        postac = {talkBeep,1.6f,0.15f,9.0f};

        talk.say("Glut test... coś tam se gada",glut);

       
    }
    void onUpdate(float dt) override
    {
        r += dr * dt;
        if (r > 255) { r=255;   dr = -dr;}
        if (r < 0)   { r=0;     dr = -dr;}

        if (input().pressed('r'))      // R = przeladuj pliki
            resources().reloadAll();

        slimeJump.update(dt);
        talk.update(dt, audio);

        if (input().pressed('1')) audio.playBeep(beep);         // normalnie
        if (input().pressed('2')) audio.playBeep(beep, 1.5f);   // wyzej
        if (input().pressed('3')) audio.playBeep(beep, 0.5f);   // oktawe nizej

        if(input().pressed('m')) audio.stop("assets/test.mp3");
        if(input().pressed('x')) audio.stopAll();

         if (input().pressed('z'))
        {
            if (!talk.done())
                talk.skip();                     // Z w trakcie = pokaz cale
            else
            {
                line = (line + 1) % 3;           // 0, 1, 2, 0, 1, 2...
                if (line == 0) talk.say("glut gadanie testowe duzo tekstu", glut);
                if (line == 1) talk.say("ROBOT DUZO GADANIE TEST GADANIE", robot);
                if (line == 2) talk.say("Postac cos tam se gada, i se gada cos tam gadanie", postac);
            }
        }
        
    }
    void onDraw(engine::Canvas& c) override
    {
        engine::Color bg = {20, 20, 28};
        c.clear({' ',{0,0,0},{(int)r,40,60}});
        spr.draw(c);
        ghost.draw(c);
        talk.draw(c, {4, 32}, {255,255,255}, bg);
        if (talk.done())
            c.drawText({76, 33}, "[Z]", {120,120,140}, bg);
    }
};

int main()
{
    TestApp app;
    app.run({120,40});
}