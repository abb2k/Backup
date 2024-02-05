#pragma once
#include <include.h>
#include <BackupsLayer.h>

class BackupCell : public CCNode, public FLAlertLayerProtocol {
protected:
    virtual bool init(ghc::filesystem::path folderPath, BackupsLayer* mainLayer, bool autos);

public:
    static BackupCell* create(ghc::filesystem::path folderPath, BackupsLayer* mainLayer, bool autos);

    ghc::filesystem::path _folderPath;

    std::string Name;

    std::string DateOfCreation;

    void DeleteMe(CCObject* object);
    void LoadBackup(CCObject* object);
    void EditBackup(CCObject* object);

    bool selected;
    BackupsLayer* mainLayer;

    CCMenu* buttonsM;

    void OnSelectedStateChanged(CCObject* object);

    void FLAlert_Clicked(FLAlertLayer* p0, bool p1);

    FLAlertLayer* deleteWarning;
    FLAlertLayer* loadWarning;

    bool autos;
};