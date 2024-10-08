#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#define PI 3.14159265358979323846f

namespace Imp {

//
// DEBUG
//
void DBG(const std::string& value) {
    std::cout << value << std::endl;
}
void DBG(const char* value) {
    std::cout << value << std::endl;
}
void DBG(int value) {
    std::cout << value << std::endl;
}
void DBG(float value) {
    std::cout << value << std::endl;
}
void DBG(bool value) {
    std::cout << (value ? "true" : "false") << std::endl;
}

//
// Vector2
//
template <typename T = int>
class Vec2 {
public:
    T x, y;
    Vec2() : x(0), y(0) {}
    Vec2(T x, T y) : x(x), y(y) {}
    Vec2 operator+(const Vec2& other) const {
        return Vec2<T>(x + other.x, y + other.y);
    }
    Vec2 operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Vec2 operator-(const Vec2& other) const {
        return Vec2<T>(x - other.x, y - other.y);
    }
    Vec2 operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Vec2 operator*(T scalar) const {
        return Vec2<T>(x * scalar, y * scalar);
    }
    Vec2 operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Vec2 operator/(T scalar) const {
        return Vec2<T>(x / scalar, y / scalar);
    }
    Vec2 operator/=(T scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Vec2& other) const {
        return x != other.x || y != other.y;
    }
    T length() const {
        return std::sqrt(x * x + y * y);
    }
    Vec2 normalize() const {
        T len = length();
        return Vec2<T>(x / len, y / len);
    }
    Vec2 floor() const {
        return Vec2<T>(std::floor(x), std::floor(y));
    }
    T size2d() const {
        return x * y;
    }
    Vec2 copy() const {
        return Vec2<T>(x, y);
    }
    std::string toString(bool floor = true) const {
        T xx = floor ? std::floor(x) : x;
        T yy = floor ? std::floor(y) : y;
        return "(" + std::to_string(xx) + ", " + std::to_string(yy) + ")";
    }
};
typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;

//
// Color
//
class Color {
public:
    uint8_t r, g, b, a;
    Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0xFF) : r(r), g(g), b(b), a(a) {}
};

//
// Input
//
class Input {
public:
    Input() {
        keyState = SDL_GetKeyboardState(NULL);
        mouseState = SDL_GetMouseState(&mouseX, &mouseY);
        poll();
    }
    ~Input() {}
    void poll() {
        for (int i = 0; i < 512; i++) {
            keyStatePrev[i] = keyState[i];
        }
        SDL_PumpEvents();

        mouseStatePrev = mouseState;
        mouseState = SDL_GetMouseState(&mouseX, &mouseY);
        // DBG((int)keyState[ SDL_SCANCODE_W ]);
    };
    Vec2i mousePos() {
        return Vec2i(mouseX, mouseY);
    }
    bool mouseKey(Uint32 key) {
        return mouseState == key;
    }
    bool mouseKeyDown(Uint32 key) {
        return mouseState == key && mouseStatePrev != key;
    }
    bool key(SDL_Keycode keyCode) {
        return keyState[SDL_GetScancodeFromKey(keyCode)];
    }
    bool keyDown(SDL_Keycode keyCode) {
        return keyState[SDL_GetScancodeFromKey(keyCode)] && !keyStatePrev[SDL_GetScancodeFromKey(keyCode)];
    }
    bool anyKey() {
        for (int i = 0; i < 512; i++) {
            if (keyState[i]) return true;
        }
        return false;
    }
    bool anyKeyDown() {
        for (int i = 0; i < 512; i++) {
            if (keyState[i] && !keyStatePrev[i]) return true;
        }
        return false;
    }
    Vec2i wasd() {
        Vec2i out;
        if (key(SDLK_w)) out.y -= 1;
        if (key(SDLK_s)) out.y += 1;
        if (key(SDLK_a)) out.x -= 1;
        if (key(SDLK_d)) out.x += 1;
        return out;
    }
private:
    const Uint8* keyState;
    Uint8 keyStatePrev[512];
    int mouseX;
    int mouseY;
    Uint32 mouseState;
    Uint32 mouseStatePrev;
};
Input _input;

//
// Graphics
//
class Graphics {
public:
    int fps = 60;
    Graphics(Vec2i windowSize) : renderer(nullptr){
        this->windowSize = windowSize;
    }
    ~Graphics() {}
    void setRenderer(SDL_Renderer* renderer) {
        this->renderer = renderer;
    }
    bool loadSpritesheet(const std::string& path) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
            DBG("Cant load image");
            return false;
        }
        spritesheet = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!spritesheet) {
            std::cerr << "SDL_CreateTextureFromSurface: " << SDL_GetError() << std::endl;
            DBG("Cant create texture");
            return false;
        }
        return true;
    }
    void sprite(Vec2i sheetPos, Vec2i pos, Vec2i spriteSize, Vec2i size, bool flipX = false, bool flipY = false) {
        SDL_Rect srcRect = { sheetPos.x, sheetPos.y, spriteSize.x, spriteSize.y };
        SDL_Rect dstRect = { pos.x, pos.y, size.x, size.y };
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (flipX) flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
        if (flipY) flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);
        SDL_RenderCopyEx(renderer, spritesheet, &srcRect, &dstRect, 0, nullptr, flip);
    }
    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF) {
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }
    void setColor(Color color) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }
    void setColor(Color color, uint8_t alpha) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);
    }
    void text(const std::string& text, Vec2i pos, int fontSize = 24, std::string fontName = "HomeVideo.ttf") {
        std::string basePath = SDL_GetBasePath();
        std::string fontPath = basePath + "assets/" + fontName;
        TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
        if (font == nullptr) {
            std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
            return;
        }
        SDL_Color color;
        SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a);
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (surface == nullptr) {
            std::cerr << "TTF_RenderText_Solid: " << TTF_GetError() << std::endl;
            return;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == nullptr) {
            std::cerr << "SDL_CreateTextureFromSurface: " << SDL_GetError() << std::endl;
            return;
        }
        SDL_Rect rect;
        rect.x = pos.x;
        rect.y = pos.y;
        rect.w = surface->w;
        rect.h = surface->h;
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        TTF_CloseFont(font);
    }
    void rect(Vec2i pos, Vec2i size, bool fill = true) {
        SDL_Rect rect = { pos.x, pos.y, size.x, size.y };
        if (fill){
            SDL_RenderFillRect(renderer, &rect);
        } else {
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
    void rectF(Vec2f pos, Vec2f size, bool fill = true) {
        SDL_FRect rect = { pos.x, pos.y, size.x, size.y };
        if (fill){
            SDL_RenderFillRectF(renderer, &rect);
        } else {
            SDL_RenderDrawRectF(renderer, &rect);
        }
    }
    void circle(Vec2i center, int radius, bool fill = true) {
        int offsetX, offsetY, d;
        int status;
        offsetX = 0;
        offsetY = radius;
        d = radius - 1;
        status = 0;
        while (offsetY >= offsetX) {
            if (fill) {
                status += SDL_RenderDrawLine(renderer, center.x - offsetY, center.y + offsetX, center.x + offsetY, center.y + offsetX);
                status += SDL_RenderDrawLine(renderer, center.x - offsetX, center.y + offsetY, center.x + offsetX, center.y + offsetY);
                status += SDL_RenderDrawLine(renderer, center.x - offsetX, center.y - offsetY, center.x + offsetX, center.y - offsetY);
                status += SDL_RenderDrawLine(renderer, center.x - offsetY, center.y - offsetX, center.x + offsetY, center.y - offsetX);
            } else {
                status += SDL_RenderDrawPoint(renderer, center.x + offsetX, center.y + offsetY);
                status += SDL_RenderDrawPoint(renderer, center.x + offsetY, center.y + offsetX);
                status += SDL_RenderDrawPoint(renderer, center.x - offsetX, center.y + offsetY);
                status += SDL_RenderDrawPoint(renderer, center.x - offsetY, center.y + offsetX);
                status += SDL_RenderDrawPoint(renderer, center.x + offsetX, center.y - offsetY);
                status += SDL_RenderDrawPoint(renderer, center.x + offsetY, center.y - offsetX);
                status += SDL_RenderDrawPoint(renderer, center.x - offsetX, center.y - offsetY);
                status += SDL_RenderDrawPoint(renderer, center.x - offsetY, center.y - offsetX);
            }
            if (status < 0) {
                status = -1;
                break;
            }
            if (d >= 2 * offsetX) {
                d -= 2 * offsetX + 1;
                offsetX += 1;
            } else if (d < 2 * (radius - offsetY)) {
                d += 2 * offsetY - 1;
                offsetY -= 1;
            } else {
                d += 2 * (offsetY - offsetX - 1);
                offsetY -= 1;
                offsetX += 1;
            }
        }
    }
    void line(Vec2i start, Vec2i end, int thickness = 1) {
        if (thickness <= 1) {
            SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
        } else {
            // Calculate the direction vector
            Vec2f direction = Vec2f(end.x - start.x, end.y - start.y).normalize();
            // Calculate the perpendicular vector
            Vec2f perpendicular(-direction.y, direction.x);
            // Scale the perpendicular vector by half the thickness
            perpendicular *= (thickness / 2.0f);

            // Define the four corners of the thick line
            Vec2f points[4] = {
                Vec2f(start.x, start.y) + perpendicular,
                Vec2f(start.x, start.y) - perpendicular,
                Vec2f(end.x, end.y) + perpendicular,
                Vec2f(end.x, end.y) - perpendicular
            };

            // Draw the thick line as a series of connected lines
            SDL_Point sdlPoints[5];
            for (int i = 0; i < 4; ++i) {
                sdlPoints[i].x = static_cast<int>(points[i].x);
                sdlPoints[i].y = static_cast<int>(points[i].y);
            }
            sdlPoints[4] = sdlPoints[0]; // Close the polygon

            SDL_RenderDrawLines(renderer, sdlPoints, 5);
        }
    }
    void point(Vec2i pos) {
        SDL_RenderDrawPoint(renderer, pos.x, pos.y);
    }
    void tri(Vec2i a, Vec2i b, Vec2i c, float rotation = 0.0f, bool fill = true) {
        auto rotatePoint = [&](Vec2i p, Vec2i center, float angle) {
            float s = std::sin(angle);
            float c = std::cos(angle);

            // Translate point back to origin
            p.x -= center.x;
            p.y -= center.y;

            // Rotate point
            int xNew = static_cast<int>(p.x * c - p.y * s);
            int yNew = static_cast<int>(p.x * s + p.y * c);

            // Translate point back
            p.x = xNew + center.x;
            p.y = yNew + center.y;
            return p;
        };

        // Calculate the center of the triangle
        Vec2i center = (a + b + c) / 3;

        // Rotate points around the center
        a = rotatePoint(a, center, rotation);
        b = rotatePoint(b, center, rotation);
        c = rotatePoint(c, center, rotation);

        if (fill) {
            // Sort the vertices by y-coordinate ascending (a.y <= b.y <= c.y)
            if (a.y > b.y) std::swap(a, b);
            if (a.y > c.y) std::swap(a, c);
            if (b.y > c.y) std::swap(b, c);

            auto drawScanline = [&](int y, int x1, int x2) {
                if (x1 > x2) std::swap(x1, x2);
                SDL_RenderDrawLine(renderer, x1, y, x2, y);
            };

            auto edgeInterpolate = [&](Vec2i v1, Vec2i v2, int y) {
                if (v1.y == v2.y) return v1.x;
                return v1.x + (v2.x - v1.x) * (y - v1.y) / (v2.y - v1.y);
            };

            for (int y = a.y; y <= c.y; ++y) {
                if (y < b.y) {
                    drawScanline(y, edgeInterpolate(a, b, y), edgeInterpolate(a, c, y));
                } else {
                    drawScanline(y, edgeInterpolate(b, c, y), edgeInterpolate(a, c, y));
                }
            }
        } else {
            SDL_Point points[4] = { {a.x, a.y}, {b.x, b.y}, {c.x, c.y}, {a.x, a.y} };
            SDL_RenderDrawLines(renderer, points, 4);
        }
    }
    void clear() {
        SDL_RenderClear(renderer);
         // Draw a rectangle
        SDL_Rect fillRect = { 200, 150, 240, 180 };
        SDL_RenderFillRect(renderer, &fillRect);
    }
    void render() {
        SDL_RenderPresent(renderer);
    }
    Vec2i getWindowSize() {
        return windowSize;
    }
    int textWidth(const std::string& text, int fontSize = 24, std::string fontName = "HomeVideo.ttf") {
        std::string basePath = SDL_GetBasePath();
        std::string fontPath = basePath + "assets/" + fontName;
        TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
        if (font == nullptr) {
            std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
            return 0;
        }
        int width;
        TTF_SizeText(font, text.c_str(), &width, nullptr);
        TTF_CloseFont(font);
        return width;
    }
    int textHeight(const std::string& text, int fontSize = 24, std::string fontName = "HomeVideo.ttf") {
        std::string basePath = SDL_GetBasePath();
        std::string fontPath = basePath + "assets/" + fontName;
        TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
        if (font == nullptr) {
            std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
            return 0;
        }
        int height;
        TTF_SizeText(font, text.c_str(), nullptr, &height);
        TTF_CloseFont(font);
        return height;
    }
    void tickUp() {
        tick++;
    }
    int getTick() {
        return tick;
    }
private:
    int tick = 0;
    Vec2i windowSize;
    SDL_Renderer* renderer;
    SDL_Texture* spritesheet;
};

class Sound {
public:
    Mix_Chunk* sound;
    int channel;
    int volume;
    int pan[2];
    Sound(const std::string& path = "") {
        pan[0] = 255;
        pan[1] = 255;
        volume = MIX_MAX_VOLUME / 2;
        if (path != "") set(path);
    }
    ~Sound() {
        Mix_FreeChunk(sound);
        Mix_CloseAudio();
    }
    void set(const std::string& path) {
        channel = -1;
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
            DBG("Cant initialize SDL_mixer");
            return;
        }
        std::string basePath = SDL_GetBasePath();
        std::string fullPath = basePath + "assets/" + path;
        sound = Mix_LoadWAV(fullPath.c_str());
        if (sound == nullptr) {
            std::cerr << "Mix_LoadWAV: " << Mix_GetError() << std::endl;
            DBG("Cant load sound: " + fullPath);
        }
    }
    void play(bool ifNotPlaying = false) {
        if (ifNotPlaying && isPlaying()) return;
        Mix_VolumeChunk(sound, volume);
        channel = Mix_PlayChannel(-1, sound, 0);
        Mix_SetPanning(channel, pan[0], pan[1]);
    }
    void stop() {
        if (channel == -1) return;
        Mix_HaltChannel(channel);
    }
    void mute() {
        Mix_Volume(channel, 0);
    }
    void unmute() {
        Mix_Volume(channel, volume);
    }
    void volDown(int amount = 1, int min = 0){
        volume -= amount;
        if (volume < min) volume = min;
        Mix_VolumeChunk(sound, volume);
    }
    void volUp(int amount = 1, int max = MIX_MAX_VOLUME){
        volume += amount;
        if (volume > max) volume = max;
        Mix_VolumeChunk(sound, volume);
    }
    bool isPlaying() {
        return Mix_Playing(channel);
    }
    void setPan(int left, int right) {
        pan[0] = left;
        pan[1] = right;
    }
};

class Sprite {
public:
    Vec2i size;
    Vec2i sheetPos;
    Vec2i spriteSize;
    int fps;
    Sprite(Vec2i sheetPos = Vec2i(0,0), Vec2i spriteSize = Vec2i(16,16), Vec2i size = Vec2i(64,64)) 
        : size(size), sheetPos(sheetPos), spriteSize(spriteSize) 
    {
        fps = 0;
        isAnimated = false;
    }
    ~Sprite() {}
    void set(Vec2i sheetPos, Vec2i spriteSize, Vec2i size) {
        this->sheetPos = sheetPos;
        this->spriteSize = spriteSize;
        this->size = size;
    }
    void setSheetPos(Vec2i sheetPos) {
        this->sheetPos = sheetPos;
    }
    void setAnimation(std::vector<Vec2i> frames, int fps = 1, bool pingPong = false) {
        if (pingPong) {
            std::vector<Vec2i> framesCopy = frames;
            for (int i = frames.size() - 2; i >= 1; --i) {
                frames.push_back(framesCopy[i]);
            }
        }
        this->frames = frames;
        this->fps = fps;
        isAnimated = true;
    }
    void stopAnimation() {
        isAnimated = false;
    }
    void render(Graphics* graph, Vec2i pos, bool flipX = false, bool flipY = false) {
        Vec2i sheetPosTemp = sheetPos;
        if (isAnimated) {
            int tickNorm = (graph->getTick() / (graph->fps / fps));
            int frameIndex =  tickNorm % frames.size();
            sheetPosTemp = frames[frameIndex];
        }
        graph->sprite(sheetPosTemp, pos, spriteSize, size, flipX, flipY);
    }
private:
    std::vector<Vec2i> frames;
    bool isAnimated;
};

//
// Collision
//
class BoxCollider {
public:
    Vec2i pos;
    Vec2i size;
    Vec2i positionOffset;
    BoxCollider(Vec2i pos, Vec2i size, Vec2i positionOffset = Vec2i(0, 0)) 
        : pos(pos), size(size), positionOffset(positionOffset) {}
    ~BoxCollider() {}
    bool isColliding(BoxCollider* other) {
        Vec2i thisPos = pos + positionOffset;
        Vec2i otherPos = other->pos + other->positionOffset;
        return thisPos.x < otherPos.x + other->size.x &&
               thisPos.x + size.x > otherPos.x &&
               thisPos.y < otherPos.y + other->size.y &&
               thisPos.y + size.y > otherPos.y;
    }
    bool isInside(Vec2i point) {
        Vec2i thisPos = pos + positionOffset;
        return point.x >= thisPos.x && point.x <= thisPos.x + size.x &&
               point.y >= thisPos.y && point.y <= thisPos.y + size.y;
    }
};

//
// Entities
//
class Entity {
public:
    std::string tag;
    Vec2i pos;
    BoxCollider* collider;
    int zindex = 0;
    bool colliderEnabled = false;
    Entity(){
        tag = "none";
        collider = new BoxCollider(pos, Vec2i(0, 0));
    }
    ~Entity(){}
    // XXX - Pointless?
    void init(int id){
        onStart();
    }
    int getId() {
        return id;
    }
    void setPos(Vec2i pos) {
        this->pos = pos;
        updateCollider();
    }
    void setCollider(Vec2i size, Vec2i offset = Vec2i(0, 0)) {
        colliderEnabled = true;
        collider->size = size;
        collider->positionOffset = offset;
    }
    void updateCollider() {
        collider->pos = pos;
    }
    void drawCollider(Graphics* graph) {
        graph->setColor(0, 255, 0);
        graph->rect(collider->pos + collider->positionOffset, collider->size, false);
    }
    // Called after init
    virtual void onStart() {}
    // Called every frame
    virtual void process() {}
    // Called every frame
    virtual void render(Graphics* graph) {}
    // Called when colliding with another entity
    virtual void onCollision(Entity* other) {}
    // Called every frame
    virtual void onMouse(bool over) {}
    // Called every frame when mouse is over
    virtual void onMouseOver() {}
private:
    int id;
};

typedef std::vector<std::pair<int, int>> CollisionPair;
class EntityManager {
public:
    std::vector<Entity*> entities;
    EntityManager() {}
    ~EntityManager() {}
    void init() {}
    int addEntity(Entity* entity) {
        entities.push_back(entity);
        size_t index = entities.size() - 1;
        entity->init(index);
        DBG("Added entity: " + entity->tag);
        return index;
    }
    void removeEntity(Entity* entity) {
        entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
    }
    void process() {
        for (Entity* entity : entities) {
            entity->process();
            entity->updateCollider();
        }
    }
    void render(Graphics* graph) {
        // Sort entities by zindex
        std::sort(entities.begin(), entities.end(), [](Entity* a, Entity* b) {
            return a->zindex < b->zindex;
        });

        for (Entity* entity : entities) {
            entity->render(graph);
        }
    }
    CollisionPair checkCollisions() {
        CollisionPair collisions;
        for (size_t i = 0; i < entities.size(); ++i) {
            for (size_t j = i + 1; j < entities.size(); ++j) {
                if (entities[i]->colliderEnabled && entities[j]->colliderEnabled) {
                    if (entities[i]->collider->isColliding(entities[j]->collider)) {
                        collisions.emplace_back(i, j);
                        entities[i]->onCollision(entities[j]);
                        entities[j]->onCollision(entities[i]);
                    }
                }
            }
        }
        return collisions;
    }
    void checkMouse() {
        Vec2i mousePos = _input.mousePos();
        for (Entity* entity : entities) {
            if (entity->colliderEnabled) {
                bool over = entity->collider->isInside(mousePos);
                entity->onMouse(over);
                if (over) entity->onMouseOver();
            }
        }
    }
    std::vector<Entity*> getEntities() {
        return entities;
    }
};

//
// Buttons
//
class Btn : public Entity {
public:
    Vec2i size;
    Uint8 state;
    bool center;
    bool available;
    std::function<void()> onClick;
    std::function<void()> onHover;
    Btn() : Entity() {
        tag = "btn";
        state = 0;
        center = false;
        available = true;
        size = Vec2i(80, 24);
        setCollider(size);
    }
    ~Btn() {}
    bool isHovered() {
        return state > 0;
    }
    bool isClicked() {
        return state == 2;
    }
    void process() override {}
    virtual void render(Graphics* graph) override {}
    void onMouse(bool over) override {
        if (!available) return;
        if (_input.mouseKeyDown(SDL_BUTTON_LEFT) && over) {
            state = 2;
            if (onClick != nullptr) onClick();
        }
        else if (over) {
            state = 1;
            if (onHover != nullptr) onHover();
        }
        else state = 0;
    }
};

class BtnText : public Btn {
public:
    std::string text;
    Uint8 fontSize;
    BtnText() : Btn() {
        text = "Button";
        fontSize = 24;
    }
    ~BtnText() {}
    void render(Graphics* graph) override {
        if (!available) return;
        Color c = Color(0, 0, 0);
        switch (state) {
            case 0: c = Color(200,200,200); break;
            case 1: c = Color(255,200,200); break;
            case 2: c = Color(255,255,200); break;
        }
        graph->setColor(c);
        if (center) {
            int textWidth = graph->textWidth(text, fontSize);
            Vec2i textPos = pos + Vec2i((size.x - textWidth) / 2, (size.y - fontSize) / 2);
            graph->text(text, textPos, fontSize);
        }
        else {
            graph->text(text, pos, fontSize);
        }
    }
};

//
// Util
//
class Util {
public:
    static void hideMouse(bool hide = true) {
        SDL_ShowCursor(hide ? SDL_DISABLE : SDL_ENABLE);
    }
    static void relativeMouse(bool relative = true) {
        SDL_SetRelativeMouseMode(relative ? SDL_TRUE : SDL_FALSE);
    }
    static std::vector<std::string> splitString(const std::string& str, const std::string& delimiter) {
        std::vector<std::string> strings;
        size_t pos = 0;
        size_t prev = 0;
        while ((pos = str.find(delimiter, prev)) != std::string::npos) {
            strings.push_back(str.substr(prev, pos - prev));
            prev = pos + delimiter.length();
        }
        strings.push_back(str.substr(prev));
        return strings;
    }
    static std::string toLowercase(const std::string& str) {
        std::string out = str;
        std::transform(out.begin(), out.end(), out.begin(), ::tolower);
        return out;
    }
    static std::string strReplace(const std::string& str, const std::string fine, const std::string& replace) {
        std::string out = str;
        size_t pos = 0;
        while ((pos = out.find(fine, pos)) != std::string::npos) {
            out.replace(pos, fine.length(), replace);
            pos += replace.length();
        }
        return out;
    }
    static bool stringContains(const std::string& str, const std::string& find) {
        return str.find(find) != std::string::npos;
    }
    static std::vector<bool> intToBits(int n, int bits) {
        std::vector<bool> out;
        for (int i = 0; i < bits; i++) {
            out.push_back(n & 1);
            n >>= 1;
        }
        return out;
    }
    static int maxUnsignedInt(int bits) {
        return (1 << bits);
    }
};


//
// Main class
//
class Main {
public:
    bool shouldQuit;
    Main(const char *windowTitle = "Imp", 
        Vec2i windowSize = Vec2i(800, 600),
        int fps = 60,
        std::string spriteSheet = "")
    {
        DBG("Imp constructed");
        // Set random seed
        srand(static_cast<unsigned int>(time(0)));
        pauseRenderer = false;
        this->fps = fps;
        this->windowTitle = windowTitle;
        this->windowSize = windowSize;
        graph = new Graphics(windowSize);
        init(spriteSheet);
    }
    ~Main() {
        SDL_Quit();
        TTF_Quit();
    }
    void loop()
    {
        frameStart = SDL_GetTicks();
        entityMan.checkMouse();
        entityMan.checkCollisions();
        entityMan.process();
        process();
        if (!pauseRenderer) {
            graph->setColor(clearColor);
            graph->clear();
            entityMan.render(graph);
            render(graph);
            SDL_RenderPresent(SDL_renderer);
            graph->tickUp();   
        }
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                shouldQuit = true;
                quit();
                return;
            }
        }
        // Handle focus
        if ((SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS) == 0) {
            if (focused) {
                DBG("Focus Lost");
                focused = false;
                onLostFocus();
            }
        }
        else {
            if (!focused) {
                DBG("Focus Gained");
                focused = true;
                onGainedFocus();
            }
        }
        // Constant FPS
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
protected:
    EntityManager entityMan;
    const char* windowTitle;
    Vec2i windowSize;
    int fps;
    Color clearColor;
    SDL_Event event;
    bool pauseRenderer;
    void init(std::string spriteSheetFile = "") {
        DBG("Imp starting");
        window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize.x, windowSize.y, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            exit(1);
        }
        SDL_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (SDL_renderer == nullptr) {
            std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(1);
        }
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            exit(1);
        }
        // Initialize SDL_ttf
        if (TTF_Init() == -1) {
            // Handle error
            SDL_Quit();
            exit(1);
        }
        graph->setRenderer(SDL_renderer);

        if (spriteSheetFile != "") {
            graph->loadSpritesheet("assets/" + spriteSheetFile);
            DBG("Loaded spritesheet: " + spriteSheetFile);
        }
        shouldQuit = false;
        frameDelay = 1000 / fps; // 60 FPS
        entityMan.init();
        DBG("Imp started");
    }
    bool isFocused() {
        return focused;
    }
    void quit() {
        // Clean up and quit SDL
        DBG("Imp quitting");
        SDL_DestroyRenderer(SDL_renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

private:
    SDL_Window* window;
    SDL_Renderer* SDL_renderer;
    Graphics* graph;
    int frameDelay;
    int frameStart;
    int frameTime;
    bool focused;
    virtual void render(Graphics* graph) {}
    virtual void process() {}
    virtual void onLostFocus() {}
    virtual void onGainedFocus() {}
};

} // namespace Imp