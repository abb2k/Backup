#pragma once
#include <include.h>
#include <BackupsLayer.h>
#include <BackupCell.h>

class RenameBackupLayer :  public FLAlertLayer, public FLAlertLayerProtocol {
protected:
    virtual bool init(float _w, float _h,  BackupsLayer* _parentLayer, BackupCell* cell, const char* _spr = "GJ_square01.png");

public:
    static RenameBackupLayer* create(BackupsLayer* parentLayer, BackupCell* cell);

    virtual void show(CCNode* parent);

    void keyBackClicked();

    void backButtonCallback(CCObject* object);

    InputNode* NameInput;

    void DoneAndSave(CCObject* object);

    BackupsLayer* parentLayer;

    BackupCell* backupCell;

    FLAlertLayer* exportAlert;

    void OnExport(CCObject* object);

    void FLAlert_Clicked(FLAlertLayer* p0, bool p1);
};