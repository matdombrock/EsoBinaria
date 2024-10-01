#include "../src/Imp.h"

#define WINDOW_SIZE Vec2i(800, 420)

using namespace Imp;

class Player : public Entity {
public:
    Sprite sprite;
    Player() : Entity(), sprite(Vec2i(0,0), Vec2i(16,16), Vec2i(64,64) ) {
        std::vector<Vec2i> frames = {
            Vec2i(0,0),
            Vec2i(16,0)
        };
        sprite.setAnimation(frames, 2);
        pos = Vec2i(400, 200);
    }
    ~Player() {}
    void onStart() override {
        setCollider(Vec2i(64, 64));
    }
    void process() override {
        if (collided) {
            pos -= lastMove;
            collided = false;
        }
        Vec2i move = Input::wasd() * 2;
        if (move.x != 0 || move.y != 0) {
            sprite.fps = 10;
        }
        else {
            sprite.fps = 1;
        }
        if (move.x != 0 || move.y != 0) {
            lastMove = move.copy();   
        }
        pos += move;
    }
    void render(Graphics* graph) override {
        sprite.render(graph, pos);
    }
    void onCollision(Entity* other) override {
        DBG("Collision");
        DBG(pos.toString());
        DBG(lastMove.toString());
        Vec2i direction = pos - other->pos;
        direction = direction.normalize();
        collided = true;
    }
private:
    bool collided;
    Vec2i lastMove;
};

class App : public Main {
public:
    Vec2<int> mousePos;
    Player player;
    App() : Main("App", WINDOW_SIZE, "spritesheet.png") {
        clearColor = Color(33, 33, 33);
        entityMan.addEntity(&player);
    }
    ~App() {}
    void process() override {}
    void render(Graphics* graph) override {
        graph->setColor(255, 255, 255);
        graph->text("WASD to move", Vec2<int>(10, 10));
    }
};