#include "../src/Imp.h"
using namespace Imp;

#define WINDOW_SIZE Vec2i(800, 420)

class App : public Imp::Main { 
public:
    App() : Imp::Main("Imp", WINDOW_SIZE) {}
    ~App() {}
    void render(Graphics* graph) override{
        graph->setColor(Color(255, 255, 255));
        graph->text("Hello World", Vec2i(20, 20));
        graph->text(std::to_string(graph->getTick()), Vec2i(20, 40));
        graph->sprite(Vec2i(100, 100), Vec2i(32, 32), Vec2i(0, 0), Vec2i(32, 32));
    }
};