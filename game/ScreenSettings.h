#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "Modal.h"
#include "_colors.h"
#include "_gameMaster.h"
#include "_fonts.h"

class BtnSettings : public Btn {
public:
    std::string text = "button";
    bool isBool = false;
    Sprite sprTrue;
    Sprite sprFalse;
    std::function<bool()> isEnabled;
    BtnSettings() : Btn() {
        tag = "settings";
        size = Vec2i(_g.vu(6), _g.vu(0.5f));
        setCollider(size);

        sprTrue = Sprite(Vec2i(0, 80), Vec2i(16, 16), Vec2i(_g.cellSize/2, _g.cellSize/2));
        std::vector<Vec2i> trueFrames = {
            Vec2i(0, 80), 
            Vec2i(16, 80), 
            Vec2i(32, 80), 
            Vec2i(48, 80), 
            Vec2i(64, 80), 
        };
        sprTrue.setAnimation(trueFrames, 4, true);
        sprFalse = Sprite(Vec2i(0, 96), Vec2i(16, 16), Vec2i(_g.cellSize/2, _g.cellSize/2));
        std::vector<Vec2i> falseFrames = {
            Vec2i(0, 96), 
            Vec2i(16, 96), 
            Vec2i(32, 96), 
            Vec2i(48, 96), 
            Vec2i(64, 96), 
        };
        sprFalse.setAnimation(falseFrames, 4, true);
    }
    ~BtnSettings() {}
    void render(Graphics* graph) override {
        if (!available) return;
        graph->setColor(state ? _colors["YELLOW"] : _colors["GRAY"]);
        graph->rect(pos, size, true);
        graph->setColor(_colors["BG"]);
        graph->text(text, pos, &Fonts::medium);
        //
        if (isBool) {
            if (isEnabled()) sprTrue.render(graph, pos + Vec2i(size.x - _g.vu(0.5f), 0));
            else sprFalse.render(graph, pos + Vec2i(size.x - _g.vu(0.5f), 0));
        }
    }
};

class ScreenSettings : public Entity {
public:
    EntityManager em;
    BtnSettings btnFPS;
    BtnSettings btnAudio;
    BtnSettings btnMusic;
    BtnSettings btnClearData;
    BtnText btnBack;
    Modal modal;
    Sprite sprBg;
    ScreenSettings() : Entity() {
        tag = "ScreenSettings";

        btnFPS.pos = Vec2i(_g.vu(2), _g.vu(2.5f));
        btnFPS.isBool = true;
        btnFPS.text = "ENABLE FPS METER";
        btnFPS.onClick = [](){
            _g.store.setBool("settings_enable_fps", !_g.store.getBool("settings_enable_fps"));
        };
        btnFPS.isEnabled = [](){
            return _g.store.getBool("settings_enable_fps");
        };
        em.addEntity(&btnFPS);

        btnAudio.pos = Vec2i(_g.vu(2), _g.vu(2.5f) + _g.vu(1));
        btnAudio.isBool = true;
        btnAudio.text = "ENABLE AUDIO";
        btnAudio.onClick = [](){
            _g.store.setBool("settings_enable_audio", !_g.store.getBool("settings_enable_audio"));
        };
        btnAudio.isEnabled = [](){
            return _g.store.getBool("settings_enable_audio");
        };
        em.addEntity(&btnAudio);

        btnMusic.pos = Vec2i(_g.vu(2), _g.vu(2.5f) + _g.vu(2));
        btnMusic.isBool = true;
        btnMusic.text = "ENABLE MUSIC";
        btnMusic.onClick = [](){
            _g.store.setBool("settings_enable_music", !_g.store.getBool("settings_enable_music"));
        };
        btnMusic.isEnabled = [](){
            return _g.store.getBool("settings_enable_music");
        };
        em.addEntity(&btnMusic);

        btnClearData.pos = Vec2i(_g.vu(2), _g.vu(2.5f) + _g.vu(7));
        btnClearData.text = "CLEAR DATA";
        btnClearData.onClick = [this](){
            modal.onOk = [&]() {
                _g.store.clear();
                _g.sendMessage("init_default_settings");
                modal.show = false;
            };
            modal.showCancel = true;
            modal.title = "Clear Data?";
            modal.show = true;
        };
        em.addEntity(&btnClearData);

        btnBack.pos = Vec2i(_g.vu(0.25f), _g.vu(0.25f));
        btnBack.text = "<< BACK";
        btnBack.onClick = [](){
            _g.setScreen(SCN_MAIN_MENU);
        };
        em.addEntity(&btnBack);

        sprBg = Sprite(Vec2i(0, 128), Vec2i(32, 32), Vec2i(_g.vu(7), _g.vu(7)));

        em.addEntity(&modal);
    }
    ~ScreenSettings() {}
    void process() override {
        if (_g.getScreen() != SCN_SETTINGS) return;
        em.process();
        em.checkMouse();
    }
    void render(Graphics* graph) override {
        if (_g.getScreen() != SCN_SETTINGS) return;
        graph->setColor(_colors["BG"]);
        graph->rect(Vec2i(0, 0), WINDOW_SIZE, true);
        graph->setColor(_colors["BG3"]);
        graph->rect(Vec2i(_g.vu(0), _g.vu(0)), Vec2i(WINDOW_SIZE.x, _g.vu(2)), true);
        graph->setColor(_colors["WHITE"]);
        graph->text("   Settings", Vec2i(_g.vu(0), _g.vu(0.1f)), &Fonts::large);
        sprBg.render(graph, Vec2i(_g.vu(9), _g.vu(4.5f)));
        em.render(graph);
    }
};