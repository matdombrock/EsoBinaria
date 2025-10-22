/*
* The ScreenSettings class provides a settings menu with various toggleable options.
*/
#pragma once
#include "Imp.h"
using namespace Imp;
#include "Modal.h"
#include "g_colors.h"
#include "g_gameMaster.h"
#include "g_fonts.h"

class BtnSettings : public Btn {
public:
    std::string text = "button";
    bool isBool = false;
    Sprite sprTrue;
    Sprite sprFalse;
    std::function<bool()> isEnabled;
    BtnSettings() : Btn() {
        tag = "settings";
        size = Vec2i(g_gm.vu(6), g_gm.vu(0.5f));
        setCollider(size);

        sprTrue = Sprite(Vec2i(0, 80), Vec2i(16, 16), Vec2i(g_gm.cellSize/2, g_gm.cellSize/2));
        std::vector<Vec2i> trueFrames = {
            Vec2i(0, 80), 
            Vec2i(16, 80), 
            Vec2i(32, 80), 
            Vec2i(48, 80), 
            Vec2i(64, 80), 
        };
        sprTrue.setAnimation(trueFrames, 4, true);
        sprFalse = Sprite(Vec2i(0, 96), Vec2i(16, 16), Vec2i(g_gm.cellSize/2, g_gm.cellSize/2));
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
        graph->setColor(state ? g_colors["YELLOW"] : g_colors["GRAY"]);
        graph->rect(pos, size, true);
        graph->setColor(g_colors["BG"]);
        graph->text(text, pos, &Fonts::medium);
        //
        if (isBool) {
            if (isEnabled()) sprTrue.render(graph, pos + Vec2i(size.x - g_gm.vu(0.5f), 0));
            else sprFalse.render(graph, pos + Vec2i(size.x - g_gm.vu(0.5f), 0));
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
    BtnSettings btnAutoSave;
    BtnSettings btnScreenFX;
    BtnSettings btnRecolor;
    BtnSettings btnResize;
    BtnText btnBack;
    Modal modal;
    Sprite sprBg;
    ScreenSettings() : Entity() {
        tag = "ScreenSettings";

        btnFPS.pos = Vec2i(g_gm.vu(2), g_gm.vu(2.5f));
        btnFPS.isBool = true;
        btnFPS.text = "FPS METER";
        btnFPS.onClick = [](){
            g_gm.store.setBool("settings_enable_fps", !g_gm.store.getBool("settings_enable_fps"));
        };
        btnFPS.isEnabled = [](){
            return g_gm.store.getBool("settings_enable_fps");
        };
        em.addEntity(&btnFPS);

        btnAudio.pos = Vec2i(g_gm.vu(2), g_gm.vu(2.5f) + g_gm.vu(1));
        btnAudio.isBool = true;
        btnAudio.text = "ALL AUDIO";
        btnAudio.onClick = [](){
            g_gm.store.setBool("settings_enable_audio", !g_gm.store.getBool("settings_enable_audio"));
        };
        btnAudio.isEnabled = [](){
            return g_gm.store.getBool("settings_enable_audio");
        };
        em.addEntity(&btnAudio);

        btnMusic.pos = Vec2i(g_gm.vu(2), g_gm.vu(2.5f) + g_gm.vu(2));
        btnMusic.isBool = true;
        btnMusic.text = "MUSIC";
        btnMusic.onClick = [](){
            g_gm.store.setBool("settings_enable_music", !g_gm.store.getBool("settings_enable_music"));
        };
        btnMusic.isEnabled = [](){
            return g_gm.store.getBool("settings_enable_music");
        };
        em.addEntity(&btnMusic);

        btnAutoSave.pos = Vec2i(g_gm.vu(2), g_gm.vu(2.5f) + g_gm.vu(3));
        btnAutoSave.isBool = true;
        btnAutoSave.text = "AUTO SAVE/LOAD";
        btnAutoSave.onClick = [](){
            g_gm.store.setBool("settings_enable_auto_save", !g_gm.store.getBool("settings_enable_auto_save"));
        };
        btnAutoSave.isEnabled = [](){
            return g_gm.store.getBool("settings_enable_auto_save");
        };
        em.addEntity(&btnAutoSave);

        btnScreenFX.pos = Vec2i(g_gm.vu(2), g_gm.vu(2.5f) + g_gm.vu(4));
        btnScreenFX.isBool = true;
        btnScreenFX.text = "SCREEN FX";
        btnScreenFX.onClick = [](){
            g_gm.store.setBool("settings_enable_screen_fx", !g_gm.store.getBool("settings_enable_screen_fx"));
        };
        btnScreenFX.isEnabled = [](){
            return g_gm.store.getBool("settings_enable_screen_fx");
        };
        em.addEntity(&btnScreenFX);

        btnRecolor.pos = Vec2i(g_gm.vu(2), g_gm.vu(2.5f) + g_gm.vu(5));
        btnRecolor.text = "RECOLORIZE";
        btnRecolor.onClick = [](){
            g_gm.store.setInt("settings_color_overlay_num", (g_gm.store.getInt("settings_color_overlay_num") + 1) % 8);
            g_gm.sendMessage("recolor");
        };
        em.addEntity(&btnRecolor);

        btnResize.pos = Vec2i(g_gm.vu(2), g_gm.vu(2.5f) + g_gm.vu(6));
        btnResize.text = "RESIZE";
        btnResize.onClick = [](){
            g_gm.sendMessage("resize");
        };
        em.addEntity(&btnResize);


        btnClearData.pos = Vec2i(g_gm.vu(2), g_gm.vu(2.5f) + g_gm.vu(8));
        btnClearData.text = "CLEAR DATA";
        btnClearData.onClick = [this](){
            modal.onOk = [&]() {
                g_gm.store.clear();
                g_gm.sendMessage("init_default_settings");
                modal.show = false;
            };
            modal.showCancel = true;
            modal.title = "Clear Data?";
            modal.show = true;
        };
        em.addEntity(&btnClearData);

        btnBack.pos = Vec2i(g_gm.vu(0.25f), g_gm.vu(0.25f));
        btnBack.text = "<< BACK";
        btnBack.onClick = [](){
            g_gm.setScreen(SCN_MAIN_MENU);
        };
        em.addEntity(&btnBack);

        sprBg = Sprite(Vec2i(0, 128), Vec2i(32, 32), Vec2i(g_gm.vu(7), g_gm.vu(7)));

        em.addEntity(&modal);
    }
    ~ScreenSettings() {}
    void process() override {
        if (g_gm.getScreen() != SCN_SETTINGS) return;
        em.process();
        em.checkMouse();
    }
    void render(Graphics* graph) override {
        if (g_gm.getScreen() != SCN_SETTINGS) return;
        graph->setColor(g_colors["BG"]);
        graph->rect(Vec2i(0, 0), WINDOW_SIZE, true);
        graph->setColor(g_colors["BG3"]);
        graph->rect(Vec2i(g_gm.vu(0), g_gm.vu(0)), Vec2i(WINDOW_SIZE.x, g_gm.vu(2)), true);
        graph->setColor(g_colors["WHITE"]);
        graph->text("   Settings", Vec2i(g_gm.vu(0), g_gm.vu(0.1f)), &Fonts::large);
        sprBg.render(graph, Vec2i(g_gm.vu(9), g_gm.vu(4.5f)));
        em.render(graph);
    }
};
