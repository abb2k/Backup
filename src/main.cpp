#include <include.h>
#include <Geode/modify/MenuLayer.hpp>
#include <BackupsLayer.h>
#include <Geode/modify/GameManager.hpp>

class $modify(_MenuLayer, MenuLayer) {
	bool init() {
        if (!MenuLayer::init()) return false;

		auto sprite = CCSprite::createWithSpriteFrameName("GJ_duplicateBtn_001.png");

        auto button = CCMenuItemSpriteExtra::create(sprite, nullptr, this, menu_selector(_MenuLayer::OpenBackupsLayerButton));
        button->setZOrder(-1);

		auto menu = static_cast<CCMenu*>(this->getChildByID("right-side-menu"));
        menu->setPositionY(menu->getPositionY() - sprite->getContentSize().height / 2);

		menu->addChild(button);
        menu->updateLayout();
        
        return true;
    }

	void OpenBackupsLayerButton(CCObject* target) {
        BackupsLayer::create()->show(this);
    }
};

class $modify(GameManager) {
    void doQuickSave(){
        GameManager::doQuickSave();
        if (Mod::get()->getSettingValue<bool>("Auto_Backup")){
            std::string fileName = "\\Backups";
            std::filesystem::create_directory(Mod::get()->getSaveDir().string() + fileName);
            fileName += "\\Auto-Backups";
            std::filesystem::create_directory(Mod::get()->getSaveDir().string() + fileName);

            std::string pathToAutosaves = Mod::get()->getSaveDir().string() + fileName;

            auto creationTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            const char* Time = std::ctime(&creationTime);
            std::string t = Time;
            fileName = "\\Backups\\Auto-Backups\\--AutoBackup-- " + t;
            for (size_t i = 0; i < fileName.length(); i++)
            {
                if (fileName[i] == ':'){
                    fileName[i] = '-';
                }
                if (fileName[i] == ' '){
                    fileName[i] = '_';
                }
            }
            fileName.erase(std::remove(fileName.begin(), fileName.end(), '\n'), fileName.cend());
            std::string fullpath = Mod::get()->getSaveDir().string() + fileName;
            std::filesystem::create_directory(fullpath);

            std::string GDAPPDATAPATH = CCFileUtils::get()->getWritablePath();

            std::string CCManagerPath = GDAPPDATAPATH + "\\CCGameManager.dat";
            std::filesystem::copy(CCManagerPath, fullpath, std::filesystem::copy_options::overwrite_existing);

            std::string CCManagerPath2 = GDAPPDATAPATH + "\\CCGameManager2.dat";
            std::filesystem::copy(CCManagerPath2, fullpath, std::filesystem::copy_options::overwrite_existing);

            std::string CCLevelsPath = GDAPPDATAPATH + "\\CCLocalLevels.dat";
            std::filesystem::copy(CCLevelsPath, fullpath, std::filesystem::copy_options::overwrite_existing);

            std::string CCLevelsPath2 = GDAPPDATAPATH + "\\CCLocalLevels2.dat";
            std::filesystem::copy(CCLevelsPath2, fullpath, std::filesystem::copy_options::overwrite_existing);

            std::string dataPath = fullpath + "\\Backup.dat";
            std::ofstream bkData (dataPath);

            bkData << "Auto-Backup" << std::endl;
            std::string timeDisp = "-" + t;
            bkData << timeDisp << std::endl;

            bkData.close();

            auto readBackups = file::readDirectory(pathToAutosaves);

            int maxAmount = Mod::get()->getSettingValue<int64_t>("Max_Auto_Backups");

            if (readBackups.value().size() > maxAmount){
                int startDeleting = readBackups.value().size() - maxAmount;
                for (int i = 0; i < readBackups.value().size(); i++)
                {
                    if (i < startDeleting){
                        std::filesystem::remove_all(readBackups.value()[i].string());
                    }
                }
            }
            
        }
    }
};

$execute {
    if (Mod::get()->getSettingValue<bool>("Auto_Backup")){
        std::string fileName = "\\Backups";
        std::filesystem::create_directory(Mod::get()->getSaveDir().string() + fileName);
        fileName += "\\Auto-Backups";
        std::filesystem::create_directory(Mod::get()->getSaveDir().string() + fileName);

        std::string pathToAutosaves = Mod::get()->getSaveDir().string() + fileName;

        auto creationTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        const char* Time = std::ctime(&creationTime);
        std::string t = Time;
        fileName = "\\Backups\\Auto-Backups\\--AutoBackup-- " + t;
        for (size_t i = 0; i < fileName.length(); i++)
        {
            if (fileName[i] == ':'){
                fileName[i] = '-';
            }
            if (fileName[i] == ' '){
                fileName[i] = '_';
            }
        }
        fileName.erase(std::remove(fileName.begin(), fileName.end(), '\n'), fileName.cend());
        std::string fullpath = Mod::get()->getSaveDir().string() + fileName;
        std::filesystem::create_directory(fullpath);

        std::string GDAPPDATAPATH = CCFileUtils::get()->getWritablePath();

        std::string CCManagerPath = GDAPPDATAPATH + "\\CCGameManager.dat";
        std::filesystem::copy(CCManagerPath, fullpath, std::filesystem::copy_options::overwrite_existing);

        std::string CCManagerPath2 = GDAPPDATAPATH + "\\CCGameManager2.dat";
        std::filesystem::copy(CCManagerPath2, fullpath, std::filesystem::copy_options::overwrite_existing);

        std::string CCLevelsPath = GDAPPDATAPATH + "\\CCLocalLevels.dat";
        std::filesystem::copy(CCLevelsPath, fullpath, std::filesystem::copy_options::overwrite_existing);

        std::string CCLevelsPath2 = GDAPPDATAPATH + "\\CCLocalLevels2.dat";
        std::filesystem::copy(CCLevelsPath2, fullpath, std::filesystem::copy_options::overwrite_existing);

        std::string dataPath = fullpath + "\\Backup.dat";
        std::ofstream bkData (dataPath);

        bkData << "Auto-Backup" << std::endl;
        std::string timeDisp = "-" + t;
        bkData << timeDisp << std::endl;

        bkData.close();

        auto readBackups = file::readDirectory(pathToAutosaves);

        int maxAmount = Mod::get()->getSettingValue<int64_t>("Max_Auto_Backups");

        if (readBackups.value().size() > maxAmount){
            int startDeleting = readBackups.value().size() - maxAmount;
            for (int i = 0; i < readBackups.value().size(); i++)
            {
                if (i < startDeleting){
                    std::filesystem::remove_all(readBackups.value()[i].string());
                }
            }
        }
        
    }
}
