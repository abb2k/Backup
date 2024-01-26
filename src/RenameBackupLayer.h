#pragma once
#include <include.h>
#include <BackupsLayer.h>

class RenameBackupLayer :  public FLAlertLayer {
protected:
    virtual bool init(float _w, float _h,  BackupsLayer* _parentLayer, std::string cellBackupDir, const char* _spr = "GJ_square01.png");

public:
    static RenameBackupLayer* create(BackupsLayer* parentLayer, std::string cellBackupDir);

    virtual void show(CCNode* parent);

    void keyBackClicked();

    void backButtonCallback(CCObject* object);

    InputNode* NameInput;

    void DoneAndSave(CCObject* object);

    BackupsLayer* parentLayer;

    std::string cellBackupDir;

    std::fstream& GotoLine(std::fstream& file, unsigned int num);
};