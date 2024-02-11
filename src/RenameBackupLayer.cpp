
#include "RenameBackupLayer.h"
#include <BackupCell.h>
#include <gdbackup.h>
RenameBackupLayer* RenameBackupLayer::create(BackupsLayer* _parentLayer, BackupCell* cell) {
    auto ret = new RenameBackupLayer();
    if (ret && ret->init(280, 97, _parentLayer, cell, "GJ_square01.png")) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool RenameBackupLayer::init(float _w, float _h, BackupsLayer* _parentLayer, BackupCell* cell, const char* _spr){
    //create window
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    parentLayer = _parentLayer;
    backupCell = cell;

    if (!this->initWithColor({0, 0, 0, 105})) return false;
    m_mainLayer = CCLayer::create();
    this->addChild(m_mainLayer);

    CCScale9Sprite* bg = CCScale9Sprite::create(_spr, {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(parentLayer->GetResFixedScale({_w, _h}));
    bg->setPosition(winSize.width / 2, winSize.height / 2);
    m_mainLayer->addChild(bg);

    m_buttonMenu = CCMenu::create();
    m_mainLayer->addChild(m_buttonMenu);

    //create close window button
    auto CloseS = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    CloseS->setScale(parentLayer->GetFixedScale(1));

    auto CloseButton = CCMenuItemSpriteExtra::create(
        CloseS,
        nullptr,
        this,
        menu_selector(RenameBackupLayer::backButtonCallback)
    );
    CloseButton->setUserData(reinterpret_cast<void*>(this));
    m_buttonMenu->addChild(CloseButton);
    CloseButton->setPosition(parentLayer->GetResFixedScale({-_w, _h}, 2.1f, true));
    CloseButton->setZOrder(1);

    //add export button
    auto exportBackupSprite = CCSprite::createWithSpriteFrameName("accountBtn_myLevels_001.png");
    exportBackupSprite->setScale(parentLayer->GetFixedScale(1));
    auto exportBackupButton = CCMenuItemSpriteExtra::create(
        exportBackupSprite,
        nullptr,
        this,
        menu_selector(RenameBackupLayer::OnExport)
    );
    exportBackupButton->setPosition(parentLayer->GetResFixedScale({_w, -_h}, 2.1f, true));
    m_buttonMenu->addChild(exportBackupButton);
    CloseButton->setZOrder(1);

    CCLabelBMFont* Label = CCLabelBMFont::create("Backup Settings", "bigFont.fnt");
    Label->setPosition(parentLayer->GetResFixedScale({285, 193}));
    Label->setScale(0.75f);
    m_mainLayer->addChild(Label);

    NameInput = InputNode::create(220, "Backup Name", "bigFont.fnt", "", 12);
    NameInput->setPositionY(3);
    NameInput->setScale(0.8f);
    m_buttonMenu->addChild(NameInput);

    auto DoneS = CCScale9Sprite::create("GJ_button_01.png", {0, 0, 40, 40});
    DoneS->setContentSize({72, 25});
    DoneS->setScale(1);

    CCLabelBMFont* DoneSLabel = CCLabelBMFont::create("Apply", "bigFont.fnt");
    DoneSLabel->setScale(0.575f);
    DoneSLabel->setPosition({36, 13.5f});
    DoneS->addChild(DoneSLabel);

    auto DoneButton = CCMenuItemSpriteExtra::create(
        DoneS,
        nullptr,
        this,
        menu_selector(RenameBackupLayer::DoneAndSave)
    );

    DoneButton->setPosition({0, -29});

    m_buttonMenu->addChild(DoneButton);

    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(parentLayer->m_buttonMenu);
    CCObject* obj;
    CCARRAY_FOREACH(parentLayer->list->m_listView->m_entries, obj){
        auto cell = static_cast<BackupCell*>(obj);
        if (cell){
            CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(cell->buttonsM);
        }
    }
    return true;
}

void RenameBackupLayer::show(CCNode* parent) {
    this->setZOrder(100);

    m_mainLayer->runAction(CCEaseElasticOut::create(CCScaleTo::create(0.6f, 1.1f), 0.5f));
    this->runAction(CCFadeTo::create(0.14f, 100));
        
    parent->addChild(this); 
}

void RenameBackupLayer::OnExport(CCObject* object){
    exportAlert = FLAlertLayer::create(
        this,
        "Export Backup",
        "Do you want to export\n \"" + backupCell->Name + "\"?",
        "No",
        "Yes"
    );
    exportAlert->show();
}

void RenameBackupLayer::keyBackClicked() {
    parentLayer->RefreshBackupsList();
    this->setKeyboardEnabled(false);
    this->removeFromParentAndCleanup(true);
}

void RenameBackupLayer::backButtonCallback(CCObject* object) {
    keyBackClicked();
}

void RenameBackupLayer::DoneAndSave(CCObject* object){
    if (NameInput->getString() != ""){
        std::fstream file;

        file.open(backupCell->_folderPath / "Backup.dat");

        if (!file.fail()){
            std::vector<std::string> lines;
            std::string currLine;

            while (std::getline(file, currLine)){
                lines.push_back(currLine);
            }

            file.close();
            if (0 <= lines.size()){
                std::ofstream fileW;

                fileW.open(backupCell->_folderPath / "Backup.dat");

                if (!fileW.fail()){
                    for (int i = 0; i < lines.size(); i++)
                    {
                        if (i != 0){
                            fileW << lines[i] << std::endl;
                        }
                        else{
                            fileW << NameInput->getString() << std::endl;
                        }
                    }

                    fileW.close();
                }
            }
            
            std::string succMessage = "This backup is now named\n";
            succMessage += NameInput->getString();
            succMessage += "!";
            keyBackClicked();
            FLAlertLayer::create(
                "Success!",
                succMessage,
                "OK"
            )->show();
        }
        else{
            FLAlertLayer::create(
                "Fail",
                "Backup Not Found.",
                "Back"
            )->show();
        }
    }
    else{
        FLAlertLayer::create(
            "No Name For Backup",
            "Please add a name to your backup.",
            "Back"
        )->show();
    }
}

void RenameBackupLayer::FLAlert_Clicked(FLAlertLayer* p0, bool p1){
    if (exportAlert == p0 && p1){
        Result<std::vector<ghc::filesystem::path>> readBackups;
        readBackups = file::readDirectory(backupCell->_folderPath);

        gdbackupFile backup;
        for (int i = 0; i < readBackups.value().size(); i++)
        {
            if (i == 0)
                backup.backupName = file::readString(readBackups.value()[i]).value();
            else if (i == 1){
                backup.CCGameManager = file::readString(readBackups.value()[i]).value();
                backup.CCGameManager = backup.CCGameManager.substr(0, backup.CCGameManager.length() - 1);
            } 
            else if (i == 2){
                backup.CCGameManager2 = file::readString(readBackups.value()[i]).value();
                backup.CCGameManager2 = backup.CCGameManager2.substr(0, backup.CCGameManager2.length() - 1);
            }
            else if (i == 3){
                backup.CCLocalLevels = file::readString(readBackups.value()[i]).value();
                backup.CCLocalLevels = backup.CCLocalLevels.substr(0, backup.CCLocalLevels.length() - 1);
            }
            else if (i == 4){
                backup.CCLocalLevels2 = file::readString(readBackups.value()[i]).value();
                backup.CCLocalLevels2 = backup.CCLocalLevels2.substr(0, backup.CCLocalLevels2.length() - 1);
            }
            
        }

        //Result<> res = file::writeToJson<gdbackupFile>(Mod::get()->getSaveDir() / ("Backups/Exports/" + backupCell->Name + ".gdbackup"), backup);

        FLAlertLayer::create(
            "Exported!",
            backupCell->Name + " was exported to\n" + (Mod::get()->getSaveDir() / "Backups/Exports").string(),
            "OK"
        )->show();

        keyBackClicked();
    }
}