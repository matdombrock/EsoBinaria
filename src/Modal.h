/*
* Draws a modal dialog with OK and CANCEL buttons
*/
#pragma once
#include "Imp.h"
using namespace Imp;
#include "_colors.h"
#include "_fonts.h"
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
  // Font font = Font("HomeVideo.ttf", _g.fontSize);
  Modal() : Entity() {
    tag = "modal";
    pos = WINDOW_SIZE / 4;
    title = "Modal";
    showCancel = true;

    btnOk.text = "OK";
    btnOk.pos = pos + Vec2i(g_gm.vu(0.5f), g_gm.vu(2));
    btnOk.onClick = [this]() {
      if (onOk != nullptr)
        onOk();
      reset();
    };

    btnCancel.text = "CANCEL";
    btnCancel.pos = pos + Vec2i(g_gm.vu(2.5f), g_gm.vu(2));
    btnCancel.onClick = [this]() {
      if (onCancel != nullptr)
        onCancel();
      reset();
    };
    em.addEntity(&btnOk);
    em.addEntity(&btnCancel);
  }
  ~Modal() {}
  void setOkText(std::string text) { btnOk.text = text; }
  void setCancelText(std::string text) { btnCancel.text = text; }
  void reset() {
    show = false;
    showCancel = true;
    title = "Modal";
    g_gm.setModalActive(false);
  }
  void process() override {
    if (!show) {
      return;
    } 
    g_gm.setModalActive(true);
    if (!showCancel) {
      btnCancel.available = false;
    } else {
      btnCancel.available = true;
    }
    em.checkMouse();
    em.process();
  }
  void render(Graphics *graph) override {
    if (!show)
      return;
    graph->setColor(g_colors["BG3"]);
    graph->rect(pos, Vec2i(WINDOW_SIZE.x / 2, WINDOW_SIZE.y / 4), true);
    graph->setColor(g_colors["WHITE"]);
    graph->text(title, pos + Vec2i(g_gm.vu(0.5f), g_gm.vu(0.5f)), &Fonts::medium);
    em.render(graph);
  }
};
