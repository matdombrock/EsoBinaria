#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "_colors.h"
#include "_gameMaster.h"

class Modal : public Entity {
public:
    std::string title;
    std::function<void()> onOk;
    std::function<void()> onCancel;
    bool showCancel;
    BtnText btnOk;
    BtnText btnCancel;
    EntityManager em;
    bool show;
    Modal() : Entity() {
        tag = "modal";
        pos = WINDOW_SIZE / 4;
        title = "Modal";
        showCancel = true;

        btnOk.text = "OK";
        btnOk.pos = pos + Vec2i(_g.vu(0.5f), _g.vu(2));
        btnOk.onClick = [this]() {
            if (onOk != nullptr) onOk();
            reset();
        };
        
        btnCancel.text = "CANCEL";
        btnCancel.pos = pos + Vec2i(_g.vu(2.5f), _g.vu(2));
        btnCancel.onClick = [this]() {
            if (onCancel != nullptr) onCancel();
            reset();
        };
        em.addEntity(&btnOk);
        em.addEntity(&btnCancel);
    }
    ~Modal() {}
    void reset() {
        show = false;
        showCancel = true;
        title = "Modal";
    }
    void process() override {
        // if (_g.getShowModal()) {
        //     btnOk.available = true;
        //     btnCancel.available = true;
        // }
        if (!showCancel) {
            btnCancel.available = false;
        }
        else {
            btnCancel.available = true;
        }
        em.checkMouse();
        em.process();
    }
    void render(Graphics* graph) override {
        if (!show) return;
        graph->setColor(_colors["BG3"]);
        graph->rect(pos, Vec2i(WINDOW_SIZE.x / 2, WINDOW_SIZE.y / 4), true);
        graph->setColor(_colors["WHITE"]);
        graph->text(title, pos + Vec2i(_g.vu(0.5f), _g.vu(0.5f)));
        em.render(graph);
    }
};