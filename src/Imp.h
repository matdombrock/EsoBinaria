#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <vector> 
#include <cmath>
#include <map>
#include <fstream>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif
#if defined(__APPLE__)
    #include <SDL.h>
    #include <SDL_ttf.h>
    #include <SDL_image.h>
    #include <SDL_mixer.h>
#else
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_ttf.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_mixer.h>
#endif
#define PI 3.14159265358979323846f

namespace Imp {

const std::string basePath = SDL_GetBasePath();

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
// String Tools
//
class StringTools {
public:
    static std::string toLowercase(const std::string& str) {
        std::string out = str;
        std::transform(out.begin(), out.end(), out.begin(), ::tolower);
        return out;
    }
    static std::string toUppercase(const std::string& str) {
        std::string out = str;
        std::transform(out.begin(), out.end(), out.begin(), ::toupper);
        return out;
    }
    static std::string replace(const std::string& str, const std::string& find, const std::string& replace) {
        std::string out = str;
        size_t pos = 0;
        while ((pos = out.find(find, pos)) != std::string::npos) {
            out.replace(pos, find.length(), replace);
            pos += replace.length();
        }
        return out;
    }
    static bool contains(const std::string& str, const std::string& find) {
        return str.find(find) != std::string::npos;
    }
    static int containsCount(const std::string& str, const std::string& find) {
        int count = 0;
        size_t pos = 0;
        while ((pos = str.find(find, pos)) != std::string::npos) {
            count++;
            pos += find.length();
        }
        return count;
    }
    // TODO: This code is sketchy
    static std::vector<std::string> splitSketch(const std::string& str, const std::string& delimiter, bool cleanDelimiter = true) {
        std::vector<std::string> tokens;
        size_t start = 0;
        size_t end = str.find(delimiter);
        bool firstToken = true;
        while (end != std::string::npos) {
            std::string token = str.substr(start, end - start);
            if (!token.empty() || !cleanDelimiter) {
                if (!cleanDelimiter && !token.empty() && !firstToken) {
                    token = delimiter + token;
                }
                tokens.push_back(token);
            }
            firstToken = false;
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }
        std::string token = str.substr(start);
        if (!token.empty() || !cleanDelimiter) {
            if (!cleanDelimiter && !token.empty() && !firstToken) {
                token = delimiter + token;
            }
            tokens.push_back(token);
        }
        return tokens;
    }
    static std::vector<std::string> split(const std::string& str, std::string delimiter) {
        std::vector<std::string> tokens;
        size_t start = 0;
        size_t end = str.find(delimiter);
        while (end != std::string::npos) {
            tokens.push_back(str.substr(start, end - start));
            start = end + 1;
            end = str.find(delimiter, start);
        }
        tokens.push_back(str.substr(start));
        return tokens;
    }
};

//
// Storage
//
#ifndef __EMSCRIPTEN__
class Store {
public:
    Store() {
        read();
    }
    ~Store() {}
    void setString(std::string key, std::string value) {
        data[key] = value;
        write();
    }
    void setInt(std::string key, int value) {
        data[key] = std::to_string(value);
        write();
    }
    void setFloat(std::string key, float value) {
        data[key] = std::to_string(value);
        write();
    }
    void setBool(std::string key, bool value) {
        data[key] = value ? "true" : "false";
        write();
    }
    std::string getString(std::string key) {
        if (data.find(key) == data.end()) return "";
        return data[key];
    }
    int getInt(std::string key) {
        if (!hasKey(key)) return 0;
        if (data.find(key) == data.end()) return 0;
        return std::stoi(data[key]);
    }
    float getFloat(std::string key) {
        if (!hasKey(key)) return 0.0f;
        return std::stof(data[key]);
    }
    bool getBool(std::string key) {
        if (!hasKey(key)) return false;
        return data[key] == "true";
    }
    bool hasKey(std::string key) {
        return data.find(key) != data.end();
    }
    void clear() {
        data.clear();
        write();
    }
private:
    std::map<std::string, std::string> data;
    void write() {
        std::ofstream file(basePath + "store.txt");
        for (auto const& [key, val] : data) {
            file << key << "=" << val << std::endl;
        }
        file.close();
    }
    void read() {
        std::ifstream file(basePath + "store.txt");
        if (!file.is_open()) {
            std::cerr << "Store file not found:" << std::endl;
            std::cerr << basePath + "store.txt" << std::endl;
            return;
        }
        std::string line;
        while (std::getline(file, line)) {
            std::vector<std::string> parts = StringTools::split(line, "=");
            if (parts.size() == 2) {
                data[parts[0]] = parts[1];
            }
        }
        file.close();
    }
};
#else
class Store {
public:
    Store() {
        read();
    }
    ~Store() {}
    void setString(std::string key, std::string value) {
        EM_ASM({
            localStorage.setItem(UTF8ToString($0), UTF8ToString($1));
        }, key.c_str(), value.c_str());
    }
    void setInt(std::string key, int value) {
        setString(key, std::to_string(value));
    }
    void setFloat(std::string key, float value) {
        setString(key, std::to_string(value));
    }
    void setBool(std::string key, bool value) {
        setString(key, value ? "true" : "false");
    }
    std::string getString(std::string key) {
        if (!hasKey(key)) return "";
        char* value = (char*)EM_ASM_INT({
            var value = localStorage.getItem(UTF8ToString($0));
            if (value === null) return 0;
            var lengthBytes = lengthBytesUTF8(value) + 1;
            var stringOnWasmHeap = _malloc(lengthBytes);
            stringToUTF8(value, stringOnWasmHeap, lengthBytes);
            return stringOnWasmHeap;
        }, key.c_str());
        std::string result = value ? std::string(value) : "";
        free(value);
        return result;
    }
    int getInt(std::string key) {
        if (!hasKey(key)) return 0;
        return std::stoi(getString(key));
    }
    float getFloat(std::string key) {
        if (!hasKey(key)) return 0.0f;
        return std::stof(getString(key));
    }
    bool getBool(std::string key) {
        if (!hasKey(key)) return false;
        return getString(key) == "true";
    }
    bool hasKey(std::string key) {
        return static_cast<bool>(EM_ASM_INT({
            return localStorage.getItem(UTF8ToString($0)) !== null;
        }, key.c_str()));
    }
    void clear() {
        EM_ASM(
            localStorage.clear();
        );
    }
private:
    void read() {
        // No need to read from file, data is already in localStorage
    }
};
#endif

//
// Input
//
class Input {
public:
    Input() {
        poll();
    }
    ~Input() {}
    void poll() {
        for (int i = 0; i < 512; i++) {
            if (keyStatePrev[i] != keyState[i]) DBG("Key changed " + std::to_string(i) + " " + std::to_string(keyState[i]));
            keyStatePrev[i] = keyState[i];
        }
        // SDL_PumpEvents();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            SDL_Scancode scancode = SDL_GetScancodeFromKey(event.key.keysym.sym);
            if (event.type == SDL_KEYDOWN)    keyState[scancode] = 1;
            else if (event.type == SDL_KEYUP) keyState[scancode] = 0;
            if (event.type == SDL_QUIT) {
                DBG("SDL Quit event");
                exit(0);
                return;
            }
        }
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
    bool mouseKeyOnce(Uint32 key) {
        return mouseState == key && mouseStatePrev != key;
    }
    bool key(SDL_Keycode keyCode) {
        return keyState[SDL_GetScancodeFromKey(keyCode)];
    }
    bool keyOnce(SDL_Keycode keyCode) {
        SDL_Scancode scancode = SDL_GetScancodeFromKey(keyCode);
        return keyState[scancode] && !keyStatePrev[scancode];
    }
    bool anyKey() {
        for (int i = 0; i < 512; i++) {
            if (keyState[i]) return true;
        }
        if (mouseState) return true;
        return false;
    }
    bool anyKeyOnce() {
        for (int i = 0; i < 512; i++) {
            if (keyState[i] && !keyStatePrev[i]) return true;
        }
        if (mouseState && (mouseStatePrev != mouseState)) return true;
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
    Uint8 keyState[512] = {0};
    Uint8 keyStatePrev[512] = {0};
    int mouseX = 0;
    int mouseY = 0;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    Uint32 mouseStatePrev = 0;
};
// Declare global input
Input _input;

//
// Graphics
//

class Font {
public:
    TTF_Font* data;
    int size;
    Font(const std::string& fontName = "", int fontSize = 24) {
        if (fontName.empty()) return;
        std::string fontPath = basePath + "assets/" + fontName;
        data = TTF_OpenFont(fontPath.c_str(), fontSize);
        this->size = fontSize;
        if (data == nullptr) {
            std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
            exit(1);
            return;
        }
    }
};

class ScreenFXProps {   
public:
    int tick = 0;
    int x = 0;
    int y = 0;
    float p1 = 0.0f;
    float p2 = 0.0f;
    float p3 = 0.0f;
    Uint32* pixels;
};

class ScreenFX {
public:
    ScreenFX(){}
    ~ScreenFX() {}
    void render(SDL_Renderer* renderer, int tick, float mix, float p1, float p2, float p3) {
        SDL_GetRendererOutputSize(renderer, &width, &height);
        surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
        SDL_RenderReadPixels(renderer, nullptr, surface->format->format, surface->pixels, surface->pitch);
        Uint32* pixels = (Uint32*)surface->pixels;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Uint32 pixel= pixels[y * width + x];
                Uint8 r, g, b, a;
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
                ScreenFXProps props;
                props.tick = tick;
                props.x = x;
                props.y = y;
                props.p1 = p1;
                props.p2 = p2;
                props.p3 = p3;
                props.pixels = pixels;
                Uint8 ri = r;
                Uint8 gi = g;
                Uint8 bi = b;
                renderPixel(&r, &g, &b, &a, props);
                r = (r * mix) + (ri * (1.0f - mix));
                g = (g * mix) + (gi * (1.0f - mix));
                b = (b * mix) + (bi * (1.0f - mix));
                pixels[y * width + x] = SDL_MapRGBA(surface->format, r, g, b, a);
            }
        }
        SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        // Render the new texture
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderCopy(renderer, newTexture, nullptr, nullptr);
        SDL_DestroyTexture(newTexture);
    }
protected:
    virtual void renderPixel(Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a, ScreenFXProps props) {}
    int width, height;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
};

class FXInvert : public ScreenFX {
protected:
    void renderPixel(Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a, ScreenFXProps props) override {
        *r = 255 - *r;
        *g = 255 - *g;
        *b = 255 - *b;
    }
};

class FXScanlines : public ScreenFX {
protected:
    void renderPixel(Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a, ScreenFXProps props) override {
        float f = 0.9f;
        int mod = props.y + (props.tick / 16);
        if (mod % 32 >= 24) {
            *r = *r * f;
            *g = *g * f;
            *b = *b * f;
        }
    }
};

class FXScanlines2 : public ScreenFX {
protected:
    void renderPixel(Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a, ScreenFXProps props) override {
        float f = 0.9f;
        int scanSpeed = 4;
        float scanTime = 1.0f;
        
        int mod = props.y + (props.tick * scanSpeed);
        int scanSize = height * scanTime;
        int subScanSize = scanSize / 128;
        if (mod % scanSize >= scanSize - subScanSize) {
            Uint32 pixel = props.pixels[props.y * width + props.x + (mod % subScanSize)];
            Uint8 r1, g1, b1, a1;
            SDL_GetRGBA(pixel, surface->format, &r1, &g1, &b1, &a1);
            *r = b1 * f;
            *g = r1 * f;
            *b = g1 * f;
        }
        f=0.8f;
        mod = props.y + (props.tick * (scanSpeed/4));
        if (mod % (subScanSize * 8) >= (subScanSize * 7.5f)) {
            Uint8 rt = *r;
            Uint8 gt = *g;
            Uint8 bt = *b;
            *r = bt;
            *g = rt;
            *b = gt;
        }
        if (mod % (subScanSize * 2) >= subScanSize) {
            Uint8 rt = *r;
            Uint8 gt = *g;
            Uint8 bt = *b;
            *r = rt * f;
            *g = gt * f;
            *b = bt * f;
        }
        if (std::rand() % 5000 < 1) {
            *r = std::max(200 - *r, 0);
            *g = std::max(256 - *g, 0);
            *b = std::max(200 - *b, 0);
        }
        *r = std::min((int)(*r * 1.2f), 255);
        *b = std::min((int)(*b * 1.1f), 255);
    }
};

enum FXName {
    FX_INVERT,
    FX_SCANLINES,
    FX_SCANLINES2,
};

class Graphics {
public:
    int fps = 60;
    FXInvert fxInvert;
    FXScanlines fxScanlines;
    FXScanlines2 fxScanlines2;
    Graphics(Vec2i windowSize) : renderer(nullptr), windowSize(windowSize) {}
    ~Graphics() {}
    void setRenderer(SDL_Renderer* renderer) {
        this->renderer = renderer;
    }
    bool loadSpritesheet(const std::string& path) {
        std::string spritePath = basePath + "assets/" + path;
        SDL_Surface* surface = IMG_Load(spritePath.c_str());
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
    void text(const std::string& text, Vec2i pos, Font* font) {
        if (text.length() == 0) {
            // DBG("Attempt to render Empty text");
            return;
        }
        if (font == nullptr) {
            std::cerr << "Font not set" << std::endl;
            exit(1);
        }
        SDL_Color color;
        SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a);
        SDL_Surface* surface = TTF_RenderText_Solid(font->data, text.c_str(), color);
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
        // TTF_CloseFont(font);
    }
    void textFmt(const std::string& text, Vec2i pos, Font* font, int maxWidth = 1024) {
        // Split by new lines
        std::vector<std::string> lines = StringTools::split(text, "\n");
        // Split by overflows
        std::vector<std::string> linesOverflow = {};
        int colorWidth = textWidth("<$RRGGBB$>", font);
        for (std::string line : lines) {
            std::string lineOverflow = "";
            for (std::string word : StringTools::split(line, " ")) {
                int colorCount = StringTools::containsCount(lineOverflow + word, "<$");
                int  colorWidthTotal = colorCount * colorWidth;
                int lineW = textWidth(lineOverflow + word, font);
                if (lineW - colorWidthTotal > maxWidth) {
                    linesOverflow.push_back(lineOverflow);
                    lineOverflow = word + " ";
                }
                else {
                    lineOverflow += word + " ";
                }
            }
            linesOverflow.push_back(lineOverflow);
        }

        // Split each line into color chunks
        std::vector<std::vector<std::string>> chunks = {};
        for (std::string line : linesOverflow) {
            std::vector<std::string> subChunks = StringTools::splitSketch(line, "<$", false);
            chunks.push_back(subChunks);
        }

        // Render the text
        int yOff = 0;
        int xOff = 0;
        for (std::vector<std::string> chunk : chunks) {
            for (std::string subChunk : chunk) {
                if (subChunk[0] == '<' && subChunk[1] == '$') {
                    if (subChunk.length() >= 11) {
                        // Color code
                        std::string colorCode = subChunk.substr(2, 9);
                        Color color = Color(
                            std::stoi(colorCode.substr(0, 2), nullptr, 16),
                            std::stoi(colorCode.substr(2, 2), nullptr, 16),
                            std::stoi(colorCode.substr(4, 2), nullptr, 16)
                        );
                        setColor(color);
                        subChunk = subChunk.substr(10);
                    }
                    else continue;
                }
                this->text(subChunk, pos + Vec2i(xOff, yOff), font);
                xOff += textWidth(subChunk, font);
            }
            xOff = 0;
            yOff += font->size;
        }
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
    int textWidth(const std::string& text, Font* font) {
        if (font == nullptr) {
            std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
            exit(1);
        }
        int width;
        TTF_SizeText(font->data, text.c_str(), &width, nullptr);
        return width;
    }
    int textHeight(const std::string& text, Font* font) {
        if (font == nullptr) {
            std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
            exit(1);
        }
        int height;
        TTF_SizeText(font->data, text.c_str(), nullptr, &height);
        return height;
    }
    void tickUp() {
        tick++;
    }
    int getTick() {
        return tick;
    }
    void fxApply(FXName name, int tick, float mix = 1.0f, float p1 = 0.0f, float p2 = 0.0f, float p3 = 0.0f) {
        switch(name) {
            case FX_INVERT:
                fxInvert.render(renderer, tick, mix, p1, p2, p3);
                break;
            case FX_SCANLINES:
                fxScanlines.render(renderer, tick, mix, p1, p2, p3);
                break;
            case FX_SCANLINES2:
                fxScanlines2.render(renderer, tick, mix, p1, p2, p3);
                break;
        }
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
    int channel = -1;
    // Max volume is 128
    int volume = MIX_MAX_VOLUME / 2;
    int pan[2] = { 255, 255 };
    std::string tag;
    Sound(std::string path = "") {
        if (path != "") set(path);
    }
    ~Sound() {
        if (sound != nullptr) {
            // WARN: Causes errors if sound is redefined
            // WARN: AS happens in _sounds.h
            // Mix_FreeChunk(sound);
        }
    }
    void set(std::string path) {
        channel = -1;

        //  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        //     std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        //     DBG("Cant initialize SDL_mixer");
        //     exit(1);
        // }
        std::string fullPath = basePath + "assets/" + path;
        sound = Mix_LoadWAV(fullPath.c_str());
        if (sound == nullptr) {
            std::cerr << "Mix_LoadWAV: " << Mix_GetError() << std::endl;
            DBG("Cant load sound: " + fullPath);
            exit(1);
        }
        else {
            DBG("Sound loaded: " + fullPath);
        }
    }
    void play(int channel = 4, bool ifNotPlaying = false) {
        // return;
        this->channel = channel;
        if (sound == nullptr) {
            DBG("Sound not set");
            return;
        }
        if (ifNotPlaying && isPlaying()) return;
        Mix_VolumeChunk(sound, volume);
        this->channel = Mix_PlayChannel(channel, sound, 0);
        if (this->channel == -1) {
            std::cerr << "Mix_PlayChannel: " << Mix_GetError() << std::endl;
            DBG("Cant play sound");
            exit(1);
            return;
        }
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
    int fps = 0;
    Sprite(Vec2i sheetPos = Vec2i(0,0), Vec2i spriteSize = Vec2i(16,16), Vec2i size = Vec2i(64,64)) 
        : size(size), sheetPos(sheetPos), spriteSize(spriteSize) {}
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
    std::vector<Vec2i> frames = {};
    bool isAnimated = false;
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
    std::string tag = "none";
    Vec2i pos;
    BoxCollider* collider;
    int zindex = 0;
    bool colliderEnabled = false;
    Entity(){
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
    int id = -1;
};

typedef std::vector<std::pair<int, int>> CollisionPair;
class EntityManager {
public:
    std::vector<Entity*> entities = {};
    EntityManager() {}
    ~EntityManager() {}
    void init() {}
    int addEntity(Entity* entity) {
        entities.push_back(entity);
        size_t index = entities.size() - 1;
        entity->init(index);
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
    Vec2i size = Vec2i(80, 24);
    Uint8 state = 0;
    bool center = false;
    bool available = true;
    std::function<void()> onClick;
    std::function<void()> onHover;
    Btn() : Entity() {
        tag = "btn";
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
        if (_input.mouseKeyOnce(SDL_BUTTON_LEFT) && over) {
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
    std::string text = "Button";
    Font font = Font("HomeVideo.ttf", 24);
    BtnText() : Btn() {}
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
            int textWidth = graph->textWidth(text, &font);
            Vec2i textPos = pos + Vec2i((size.x - textWidth) / 2, (size.y - font.size) / 2);
            graph->text(text, textPos, &font);
        }
        else {
            graph->text(text, pos, &font);
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
    bool shouldQuit = false;
    Main(const char *windowTitle = "Imp", 
        Vec2i windowSize = {800, 600},
        int fps = 60,
        std::string spriteSheet = ""
    ) 
        : fps(fps), windowTitle(windowTitle), windowSize(windowSize), graph(new Graphics(windowSize))
    {
        DBG("Imp constructed");
        // Set random seed
        srand(static_cast<unsigned int>(time(0)));
        init(spriteSheet);
    }
    ~Main() {
        SDL_Quit();
        TTF_Quit();
    }
    void loop()
    {
        frameStart = SDL_GetTicks();
        _input.poll();
        // while (SDL_PollEvent(&event) != 0) {
        //     if (event.type == SDL_QUIT) {
        //         shouldQuit = true;
        //         quit();
        //         return;
        //     }
        // }
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
        int delay = frameDelay - frameTime;
        // delay = 0;
        if (delay > 0) {
            SDL_Delay(delay);
        }
        // Monitor Frame time
        frameTimeAdder += frameTime + delay;
        frameTimeAdderCount++;
        if (frameTimeAdderCount >= 16) {
            frameTimeAvg = frameTimeAdder / frameTimeAdderCount;
            frameTimeAdder = 0;
            frameTimeAdderCount = 0;
        }
    }
    int getRealFPS() {
        return 1000 / frameTimeAvg;
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
        if (basePath.empty()) {
            DBG("Base path not set");
            exit(1);
        }
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
        SDL_Init(SDL_INIT_AUDIO);
        if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 8, 2048) < 0) {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
            DBG("Cant initialize SDL_mixer");
            exit(1);
        }
        graph->setRenderer(SDL_renderer);

        if (spriteSheetFile != "") {
            graph->loadSpritesheet(spriteSheetFile);
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
    int frameTimeAdder = 0;
    int frameTimeAdderCount = 0;
    int frameTimeAvg = 0;
    bool focused;
    virtual void render(Graphics* graph) {}
    virtual void process() {}
    virtual void onLostFocus() {}
    virtual void onGainedFocus() {}
};

} // namespace Imp
