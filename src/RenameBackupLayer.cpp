
#include "RenameBackupLayer.h"
#include <BackupCell.h>

RenameBackupLayer* RenameBackupLayer::create(BackupsLayer* _parentLayer, std::string cellBackupDir) {
    auto ret = new RenameBackupLayer();
    if (ret && ret->init(280, 97, _parentLayer, cellBackupDir, "GJ_square01.png")) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool RenameBackupLayer::init(float _w, float _h, BackupsLayer* _parentLayer, std::string _cellBackupDir, const char* _spr){
    //create window
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize size = {_w, _h};

    parentLayer = _parentLayer;
    cellBackupDir = _cellBackupDir;

    if (!this->initWithColor({0, 0, 0, 105})) return false;
    m_mainLayer = CCLayer::create();
    this->addChild(m_mainLayer);

    CCScale9Sprite* bg = CCScale9Sprite::create(_spr, {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(size);
    bg->setPosition(winSize.width / 2, winSize.height / 2);
    m_mainLayer->addChild(bg);

    m_buttonMenu = CCMenu::create();
    m_mainLayer->addChild(m_buttonMenu);

    //create close window button
    auto CloseS = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    CloseS->setScale(1);

    auto CloseButton = CCMenuItemSpriteExtra::create(
        CloseS,
        nullptr,
        this,
        menu_selector(RenameBackupLayer::backButtonCallback)
    );
    CloseButton->setUserData(reinterpret_cast<void*>(this));

    m_buttonMenu->addChild(CloseButton);

    CloseButton->setPosition(-_w / 2.1f, _h / 2.1f);
    CloseButton->setZOrder(1);

    CCLabelBMFont* Label = CCLabelBMFont::create("Rename Backup", "bigFont.fnt");
    Label->setPosition({285, 193});
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

        file.open(cellBackupDir + "\\Backup.dat");

        if (!file.fail()){
            std::vector<std::string> lines;
            std::string currLine;

            while (std::getline(file, currLine)){
                lines.push_back(currLine);
            }

            file.close();
            if (0 <= lines.size()){
                std::ofstream fileW;

                fileW.open(cellBackupDir + "\\Backup.dat");

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