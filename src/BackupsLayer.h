#pragma once
#include <include.h>

class BackupsLayer : public FLAlertLayer, public FLAlertLayerProtocol {
protected:
    virtual bool init(float _w, float _h, const char* _spr = "GJ_square01.png");

public:
    static BackupsLayer* create();

    virtual void show(CCNode* parent);

    void keyBackClicked();

    void backButtonCallback(CCObject* object);

    void addButtonClicked(CCObject* object);
    void deleteButtonClicked(CCObject* object);

    void openSettings(CCObject* object);

    void OpenFolder(CCObject* object);

    CCNode* parent;

    ghc::filesystem::path BackupsDir;

    GJListLayer* list = nullptr;

    void RefreshBackupsList();

    void FLAlert_Clicked(FLAlertLayer* p0, bool p1);

    FLAlertLayer* deleteWarning;

    bool displayingAutos;

    CCSprite* normalsIcon;
    CCSprite* autosIcon;

    CCMenuItemSpriteExtra* addButton;

    CCScale9Sprite* bg;

    void changeViewMode(CCObject* object);

    void importBackup(CCObject* object);

    float GetResFixedScale(float scale, bool width);
    CCSize GetResFixedScale(CCSize scale, float multiplier = 1, bool devide = false, CCSize originSize = {569, 320});

    float DefaultScaleMultiplier;

    float GetFixedScale(float scale);
};