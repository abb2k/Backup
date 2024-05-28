#pragma once
#include <Geode/Geode.hpp>
#include "Geode/ui/TextInput.hpp"
#include <BackupsLayer.h>

using namespace geode::prelude;

class CreateBackupLayer : public Popup<BackupsLayer* const&> {
protected:
  bool setup(BackupsLayer* const& Level);
  //bool init(float _w, float _h, , const char* _spr = "GJ_square01.png");

public:
  static CreateBackupLayer* create(BackupsLayer* const& parentLayer);

  void show(CCNode* parent);

  void keyBackClicked();

  void onClose(cocos2d::CCObject*);

  TextInput* NameInput;

  void DoneAndSave(CCObject* object);

  BackupsLayer* parentLayer;
};
