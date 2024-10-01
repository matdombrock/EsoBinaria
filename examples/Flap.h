#include "../src/Imp.h"

#define WINDOW_SIZE Vec2i(800, 420)

using namespace Imp;

class Player : public Entity {
public:
    Sprite sprite;
    float speed = 8;
    float gravity = 2;
    int flyTime = 8;
    int flyTimer = 0;
    bool isDead = false;
    Player() : Entity(), sprite(Vec2i(0,0), Vec2i(16,16), Vec2i(64,64) ) {
        tag = "player";
        pos = Vec2i(400, 200);
    }
    ~Player() {}
    void onStart() override {
        setCollider(Vec2i(64, 64));
    }
    void process() override {
        bool pressed = Input::anyKeyDown();
        if (pressed) {
            flyTimer = flyTime;
        }
        else {
            flyTimer--;
        }
        if (flyTimer > 0) {
            std::vector<Vec2i> frames = {
                Vec2i(0,0),
                Vec2i(16,0)
            };
            sprite.setAnimation(frames, 8);
            pos.y -= speed;
        }
        else{
            sprite.setAnimation({Vec2i(16,0)}, 1);   
        }
        pos.y += gravity;
    }
    void render(Graphics* graph) override {
        sprite.render(graph, pos);
    }
    void onCollision(Entity* other) override {
        if (other->tag == "block") {
            DBG("player hit block");
            isDead = true;
        }
    }
    void onClick(Uint32 btn) override {
        DBG("player clicked");
    }
private:
    Vec2i lastMove;
};


class Ring : public Entity {
public:
    Sprite sprite;
    bool show = true;
    Ring() : Entity(), sprite(Vec2i(0,16), Vec2i(16,16), Vec2i(64,64) ) {
        tag = "ring";
    }
    ~Ring() {}
    void onStart() override {
        setCollider(Vec2i(64, 64));
    }
    void process() override {
        pos.x -= 2;
        if (pos.x < -64) {
            pos.x = WINDOW_SIZE.x;
        }
    }
    void render(Graphics* graph) override {
        if (show) sprite.render(graph, pos);
    }
    void onCollision(Entity* other) override {
        if (other->tag == "block" || other->tag == "ring") return;
        DBG("hit ring " + other->tag);
        show = false;
        colliderEnabled = false;
    }
};

class BlockPart : public Entity {
public:
    Sprite spriteTop;
    Sprite spriteMid;
    Sprite spriteBot;
    bool isTop = false;
    BlockPart(): 
        Entity(), 
        spriteTop(Vec2i(0,32), Vec2i(16,16), Vec2i(64,64)),
        spriteMid(Vec2i(0,48), Vec2i(16,16), Vec2i(64,64)), 
        spriteBot(Vec2i(0,64), Vec2i(16,16), Vec2i(64,64))
    {
        tag = "block";
    }
    ~BlockPart() {}
    void onStart() override {
        if (isTop) {
            setCollider(Vec2i(64, 196), Vec2i(0, -128 - 16));
        }
        else {
            setCollider(Vec2i(64, 196 ), Vec2i(0, 16));
        }
    }
    void process() override {}
    void render(Graphics* graph) override {
        if (isTop) {
            spriteTop.render(graph, pos, false, true);
            spriteMid.render(graph, pos - Vec2i(0, 64), false, true);
            spriteBot.render(graph, pos - Vec2i(0, 128), false, true);
        }
        else {
            spriteTop.render(graph, pos);
            spriteMid.render(graph, pos + Vec2i(0, 64));
            spriteBot.render(graph, pos + Vec2i(0, 128));
        }
        drawCollider(graph);
        
    }
};

class Block : public Entity {
public:
    Ring ring;
    BlockPart top;
    BlockPart bot;
    int height = 64;
    int gap = 128;
    float speed = 1;
    Block() : Entity(){
        tag = "block";
    }
    ~Block() {}
    void init(EntityManager* entityMan) {
        top.isTop = true;
        entityMan->addEntity(&top);
        entityMan->addEntity(&bot);
        entityMan->addEntity(&ring);
        reset();
    }
    void reset() {
        height = 64 + (rand() % 100);
        gap = 196 + (rand() % 128);
        pos.x = WINDOW_SIZE.x + (rand() % 128);
        ring.show = true;
        ring.colliderEnabled = true;
    }
    void process() override {
        pos.x -= speed;
        ring.pos = pos;
        top.pos = pos + Vec2i(0, height - gap);
        bot.pos = pos + Vec2i(0, height); 
        if (pos.x < -64) {
            pos.x = WINDOW_SIZE.x;
            reset();
        }
    }
    void render(Graphics* graph) override {}
};

class App : public Main {
public:
    Vec2<int> mousePos;
    Player player;
    std::vector<Block*> blocks;
    int tick = 0;
    float speed = 1;
    App() : Main("App", WINDOW_SIZE, "spritesheet.png") {
        clearColor = Color(33, 38, 44);
        entityMan.addEntity(&player);

        Block* block = new Block();
        block->init(&entityMan);
        blocks.push_back(block);
        entityMan.addEntity(block);

        Block* block2 = new Block();
        block2->init(&entityMan);
        blocks.push_back(block2);
        entityMan.addEntity(block2);

        Block* block3 = new Block();
        block3->init(&entityMan);
        blocks.push_back(block3);
        entityMan.addEntity(block3);

        restart();
    }
    ~App() {
        for (Block* block : blocks) {
            delete block;
        }
    }
    void restart() {
        player.pos.y = WINDOW_SIZE.y / 2;
        player.isDead = false;
        speed = 1;
        int off = 0;
        for (Block* block : blocks) {
            block->reset();
            block->pos.x += off;
            block->pos.y = WINDOW_SIZE.y / 2;
            off += 256;
        }
    }
    void process() override {
        if (player.isDead) {
            restart();
            return;
        }
        speed += 0.001;
        for (Block* block : blocks) {
            block->speed = speed;
        }
    }
    void render(Graphics* graph) override {
        graph->setColor(255, 255, 255);
        graph->text(std::to_string(speed), Vec2<int>(10, 10));
    }
};