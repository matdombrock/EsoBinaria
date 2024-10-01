#include "../src/Imp.h"
using namespace Imp;

class App : public Imp::Main { 
public:
    Sound sound;
    App(const char *windowTitle = "Imp", Vec2i windowSize = Vec2i(800, 600)) 
        : Imp::Main(windowTitle, windowSize), sound("sound.wav") 
    {
        start();
    }
    ~App() {}
    void process() override{
        if (Input::key(SDLK_SPACE) && !sound.isPlaying()) {
            sound.play();
            DBG("Playing sound.wav");
        }
    }
    void render(Graphics* graph) override{
        graph->setColor(Color(255, 255, 255));
        graph->text("Press space to play sound", Vec2i(20, 20));
    }
};