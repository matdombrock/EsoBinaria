#include "../src/Imp.h"
using namespace Imp;

class App : public Imp::Main { 
public:
    Sprite sprite;
    App(const char *windowTitle = "Imp", Vec2i windowSize = Vec2i(800, 600)) 
        : Imp::Main(windowTitle, windowSize),
        sprite(Vec2i(0, 0), Vec2i(16, 16), Vec2i(64, 64))
    {
        spriteSheetFile = "spritesheet.png";
        sprite.setAnimation({Vec2i(0, 0), Vec2i(16, 0)}, 4);
        start();
    }
    ~App() {}
    void render(Graphics* graph) override{
        sprite.render(graph, Vec2i(100, 100));
    }
};