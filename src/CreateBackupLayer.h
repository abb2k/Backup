#pragma once
#include <include.h>
#include <BackupsLayer.h>

class CreateBackupLayer :  public FLAlertLayer {
protected:
    virtual bool init(float _w, float _h,  BackupsLayer* _parentLayer, const char* _spr = "GJ_square01.png");

public:
    static CreateBackupLayer* create(BackupsLayer* parentLayer);

    virtual void show(CCNode* parent);

    void keyBackClicked();

    void backButtonCallback(CCObject* object);

    InputNode* NameInput;

    void DoneAndSave(CCObject* object);

    BackupsLayer* parentLayer;
};