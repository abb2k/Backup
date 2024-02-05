#include <include.h>
#include <Geode/modify/MenuLayer.hpp>
#include <BackupsLayer.h>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/OptionsLayer.hpp>

class $modify(_MenuLayer, MenuLayer){
    bool init(){
        if (!MenuLayer::init()) return false;

        if (!Mod::get()->getSettingValue<bool>("Button_In_Options_Menu"))
        {
            auto sprite = CCSprite::createWithSpriteFrameName("GJ_duplicateBtn_001.png");

            auto button = CCMenuItemSpriteExtra::create(sprite, nullptr, this, menu_selector(_MenuLayer::OpenBackupsLayerButton));
            button->setZOrder(-1);

            auto menu = static_cast<CCMenu *>(this->getChildByID("right-side-menu"));
            menu->setPositionY(menu->getPositionY() - sprite->getContentSize().height / 2);

            menu->addChild(button);
            menu->updateLayout();
        }

        return true;
    }

    void OpenBackupsLayerButton(CCObject *target)
    {
        BackupsLayer::create()->show(this);
    }
};

class $modify(_OptionsLayer, OptionsLayer){
    void customSetup(){
        OptionsLayer::customSetup();

        if (Mod::get()->getSettingValue<bool>("Button_In_Options_Menu"))
        {
            auto sprite = CCSprite::createWithSpriteFrameName("GJ_duplicateBtn_001.png");

            auto button = CCMenuItemSpriteExtra::create(sprite, nullptr, this, menu_selector(_OptionsLayer::OpenBackupsLayerButton));
            button->setPosition({138, -171});

            auto menu = CCMenu::create();

            menu->addChild(button);
            this->m_listLayer->addChild(menu);
        }
    }

    void OpenBackupsLayerButton(CCObject *target)
    {
        BackupsLayer::create()->show(this);
    }
};

class $modify(_myGameManager, GameManager){
    void doQuickSave(){
        GameManager::doQuickSave();
        if (Mod::get()->getSettingValue<bool>("Auto_Backup"))
        {
            geode::Result<> res = geode::utils::file::createDirectory(Mod::get()->getSaveDir() / "Backups");
            res = geode::utils::file::createDirectory(Mod::get()->getSaveDir() / "Backups/Auto-Backups");

            ghc::filesystem::path pathToAutosaves = Mod::get()->getSaveDir() / "Backups/Auto-Backups";

            auto readBackups = file::readDirectory(pathToAutosaves);

            int maxAmount = Mod::get()->getSettingValue<int64_t>("Max_Auto_Backups");

            typedef struct {
                ghc::filesystem::path path;
                long long dateOfCreation;
            } backupWPath;

            std::vector<backupWPath> pathsWithTime;
            
            if (readBackups.value().size() == maxAmount)
            {
                for (int i = 0; i < readBackups.value().size(); i++)
                {
                    std::string path_string = readBackups.value()[i].string();
                    struct stat __stat;
                    stat(path_string.c_str(), &__stat);
                    struct tm* _time;
                    _time = gmtime(&(__stat.st_ctime));
                    
                    std::string concatenatedDate = std::to_string(_time->tm_year) + std::to_string(_time->tm_mon) + std::to_string(_time->tm_mday) + std::to_string(_time->tm_hour) + std::to_string(_time->tm_min) + std::to_string(_time->tm_sec);

                    backupWPath meRN;
                    meRN.dateOfCreation = std::stoll(concatenatedDate);
                    meRN.path = readBackups.value()[i];
                    pathsWithTime.push_back(meRN);
                    
                    int currentMEIndex = pathsWithTime.size() - 1;

                    backupWPath prevSave;

                    for (int b = 0; b < pathsWithTime.size(); b++)
                    {
                        if (currentMEIndex != 0){
                            if (pathsWithTime[currentMEIndex - 1].dateOfCreation > meRN.dateOfCreation){
                                prevSave = pathsWithTime[currentMEIndex - 1];
                                pathsWithTime[currentMEIndex - 1] = meRN;
                                pathsWithTime[currentMEIndex] = prevSave;
                            }
                        }
                    }
                    
                    
                }
                
                ghc::filesystem::remove_all(pathsWithTime[0].path);      
            }

            auto creationTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            const char *Time = std::ctime(&creationTime);
            std::string t = Time;
            std::string fileName = "Backups/Auto-Backups/--AutoBackup-- " + t;
            for (size_t i = 0; i < fileName.length(); i++)
            {
                if (fileName[i] == ':')
                {
                    fileName[i] = '-';
                }
                if (fileName[i] == ' ')
                {
                    fileName[i] = '_';
                }
            }
            fileName.erase(std::remove(fileName.begin(), fileName.end(), '\n'), fileName.cend());
            ghc::filesystem::path fullpath = Mod::get()->getSaveDir() / fileName;
            res = file::createDirectory(fullpath);

            std::string tempWPath = CCFileUtils::get()->getWritablePath();
            ghc::filesystem::path GDAPPDATAPATH(tempWPath);

            ghc::filesystem::copy(GDAPPDATAPATH / "CCGameManager.dat", fullpath);
            ghc::filesystem::copy(GDAPPDATAPATH / "CCGameManager2.dat", fullpath);
            ghc::filesystem::copy(GDAPPDATAPATH / "CCLocalLevels.dat", fullpath);
            ghc::filesystem::copy(GDAPPDATAPATH / "CCLocalLevels2.dat", fullpath);

            ghc::filesystem::path dataPath = fullpath / "Backup.dat";
            std::ofstream bkData(dataPath);

            bkData << "Auto-Backup" << std::endl;
            std::string timeDisp = "-" + t;
            bkData << timeDisp << std::endl;

            bkData.close();
        }
    }
};

$execute
{
    if (Mod::get()->getSettingValue<bool>("Auto_Backup"))
    {
        geode::Result<> res = geode::utils::file::createDirectory(Mod::get()->getSaveDir() / "Backups");
        res = geode::utils::file::createDirectory(Mod::get()->getSaveDir() / "Backups/Auto-Backups");

        ghc::filesystem::path pathToAutosaves = Mod::get()->getSaveDir() / "Backups/Auto-Backups";

        auto readBackups = file::readDirectory(pathToAutosaves);

        int maxAmount = Mod::get()->getSettingValue<int64_t>("Max_Auto_Backups");

        typedef struct {
            ghc::filesystem::path path;
            long long dateOfCreation;
        } backupWPath;

        std::vector<backupWPath> pathsWithTime;
            
        if (readBackups.value().size() == maxAmount)
        {
            for (int i = 0; i < readBackups.value().size(); i++)
            {
                std::string path_string = readBackups.value()[i].string();
                struct stat __stat;
                stat(path_string.c_str(), &__stat);
                struct tm* _time;
                _time = gmtime(&(__stat.st_ctime));
                    
                 std::string concatenatedDate = std::to_string(_time->tm_year) + std::to_string(_time->tm_mon) + std::to_string(_time->tm_mday) + std::to_string(_time->tm_hour) + std::to_string(_time->tm_min) + std::to_string(_time->tm_sec);

                backupWPath meRN;
                meRN.dateOfCreation = std::stoll(concatenatedDate);
                meRN.path = readBackups.value()[i];
                pathsWithTime.push_back(meRN);
                    
                int currentMEIndex = pathsWithTime.size() - 1;

                backupWPath prevSave;

                for (int b = 0; b < pathsWithTime.size(); b++)
                {
                    if (currentMEIndex != 0){
                        if (pathsWithTime[currentMEIndex - 1].dateOfCreation > meRN.dateOfCreation){
                            prevSave = pathsWithTime[currentMEIndex - 1];
                            pathsWithTime[currentMEIndex - 1] = meRN;
                            pathsWithTime[currentMEIndex] = prevSave;
                        }
                    }
                }
                    
                    
            }
                
            ghc::filesystem::remove_all(pathsWithTime[0].path);      
        }

            auto creationTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            const char *Time = std::ctime(&creationTime);
            std::string t = Time;
            std::string fileName = "Backups/Auto-Backups/--AutoBackup-- " + t;
            for (size_t i = 0; i < fileName.length(); i++)
            {
                if (fileName[i] == ':')
                {
                    fileName[i] = '-';
                }
                if (fileName[i] == ' ')
                {
                    fileName[i] = '_';
                }
            }
            fileName.erase(std::remove(fileName.begin(), fileName.end(), '\n'), fileName.cend());
            ghc::filesystem::path fullpath = Mod::get()->getSaveDir() / fileName;
            res = file::createDirectory(fullpath);

            std::string tempWPath = CCFileUtils::get()->getWritablePath();
            ghc::filesystem::path GDAPPDATAPATH(tempWPath);

            ghc::filesystem::copy(GDAPPDATAPATH / "CCGameManager.dat", fullpath);
            ghc::filesystem::copy(GDAPPDATAPATH / "CCGameManager2.dat", fullpath);
            ghc::filesystem::copy(GDAPPDATAPATH / "CCLocalLevels.dat", fullpath);
            ghc::filesystem::copy(GDAPPDATAPATH / "CCLocalLevels2.dat", fullpath);

            ghc::filesystem::path dataPath = fullpath / "Backup.dat";
            std::ofstream bkData(dataPath);

            bkData << "Auto-Backup" << std::endl;
            std::string timeDisp = "-" + t;
            bkData << timeDisp << std::endl;

            bkData.close();
    }
    
}
