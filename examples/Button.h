#include "../src/Imp.h"
using namespace Imp;

#define WINDOW_SIZE Vec2i(800, 420)

class Button : public Entity {
public:
    Sprite sprite;
    Uint32 lastPress;
    bool hovered;
    Button() : Entity(), sprite(Vec2i(0, 0), Vec2i(16, 16), Vec2i(64, 64)) {
        tag = "button";
        lastPress = 0;
        hovered = false;
    }
    ~Button() {}
    void onStart() override {
        setCollider(Vec2i(64, 64));
    }
    void process() override {
    }
    void render(Graphics* graph) override {
        Vec2i sheetPos = lastPress > 0 ? Vec2i(16, 0) : Vec2i(0, 0);
        sprite.setSheetPos(sheetPos);
        sprite.render(graph, pos, false, !hovered);
        hovered = false;
    }
    void onCollision(Entity* other) override {
        if (other->tag == "player") {
            DBG("button hit player");
        }
    }
    void onMouse(Uint32 btn) override {
        DBG((int)btn);
        lastPress = btn;
        hovered = true;
    }
};

class App : public Imp::Main { 
public:
    Button button;
    App() : Imp::Main("IMP", WINDOW_SIZE, "spritesheet.png") {
        button.pos = Vec2i(100, 100);
        entityMan.addEntity(&button);
    }
    ~App() {}
    void render(Graphics* graph) override{
        graph->setColor(Color(255, 255, 255));
        graph->text("Last Press: " + std::to_string(button.lastPress), Vec2i(20, 20));
    }
};