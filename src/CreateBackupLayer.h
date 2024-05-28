#pragma once
#include "Geode/ui/TextInput.hpp"
#include <BackupsLayer.h>
#include <include.h>
#include <string>

class CreateBackupLayer : public FLAlertLayer {
protected:
  virtual bool init(float _w, float _h, BackupsLayer* _parentLayer, const char* _spr = "GJ_square01.png");

public:
  static CreateBackupLayer* create(BackupsLayer* parentLayer);

  virtual void show(CCNode* parent);

  void keyBackClicked();

  void backButtonCallback(CCObject* object);

  TextInput* NameInput;

  void DoneAndSave(CCObject* object);

  BackupsLayer* parentLayer;
};
