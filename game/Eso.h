// EsoBinaria - A simple tile-based visual programming game
#include <map>

#include "../lib/s7/s7.h"

#include "../src/Imp.h"
using namespace Imp;
#include "Grid.h"
#include "TestArea.h"
#include "Cursor.h"
#include "TopMenu.h"
#include "BottomBar.h"
#include "MainMenu.h"
#include "SetupScreen.h"
#include "HelpScreen.h"
#include "_gameMaster.h"
#include "_sounds.h"
#include "_fonts.h"


class App : public Imp::Main { 
public:
    Grid grid;
    TestArea testArea;
    Cursor cursor;
    TopMenu TopMenu;
    BottomBar bottomBar;
    MainMenu mainMenu;
    SetupScreen setupScreen;
    HelpScreen helpScreen;
    ShaderProgram shader;
    App() : Imp::Main("EsoBinaria (v0.1-alpha)", WINDOW_SIZE, 60, "tiles.png"){ 
        clearColor = Color(_colors["BG"]);
        entityMan.addEntity(&grid);
        entityMan.addEntity(&bottomBar);
        entityMan.addEntity(&testArea);
        entityMan.addEntity(&TopMenu);
        entityMan.addEntity(&setupScreen);
        entityMan.addEntity(&helpScreen);
        entityMan.addEntity(&mainMenu);
        entityMan.addEntity(&cursor);

        _g.setScreen("helpScreen");

        Sounds::init();
        Fonts::init(_g.fontSize);
        
        shader.load("assets/shaders/vertex_shader.glsl", "assets/shaders/fragment_shader.glsl");
        shader.use();

    }
    ~App() {}
    void render(Graphics* graph) override {
        graph->setColor(_colors["RED"]);
        graph->text("EsoBinaria", Vec2i(20, 20), &Fonts::large);
        // Create a VBO
        GLuint vbo;
        glGenBuffers(1, &vbo);

        // Create a VAO
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Specify the layout of the vertex data
        GLint posAttrib = glGetAttribLocation(shader.ID, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        SDL_GL_SwapWindow(window);
    }
    void process() override {
        _input.poll();
        // Pause
        if (_input.keyOnce(SDLK_ESCAPE)) {
            DBG("Hit ESC");
            _g.setScreen("mainMenu");
        }

        if (_g.getReset()) {
            _g.setReset(false);
            grid.reset();
            testArea.reset();
        }
        if (_g.getQuit()) {
            exit(0);
        }

        _g.incTick();
    }
    void onLostFocus() override {
        pauseRenderer = true;
    }
    void onGainedFocus() override {
        pauseRenderer = false;
    }
};