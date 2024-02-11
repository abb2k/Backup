#pragma once
#include <include.h>

typedef struct {
        std::string backupName;

        std::string CCGameManager;
        std::string CCGameManager2;

        std::string CCLocalLevels;
        std::string CCLocalLevels2;
} gdbackupFile;

template <>
    struct matjson::Serialize<gdbackupFile> {
        static gdbackupFile from_json(const matjson::Value& value) {
            return gdbackupFile {
                .backupName = value["backupName"].as_string(),
                .CCGameManager = value["CCGameManager"].as_string(),
                .CCGameManager2 = value["CCGameManager2"].as_string(),
                .CCLocalLevels = value["CCLocalLevels"].as_string(),
                .CCLocalLevels2 = value["CCLocalLevels2"].as_string(),
            };
        }
        static matjson::Value to_json(const gdbackupFile& file) {
            return matjson::Object {
                { "backupName", file.backupName },
                { "CCGameManager", file.CCGameManager },
                { "CCGameManager2", file.CCGameManager2 },
                { "CCLocalLevels", file.CCLocalLevels },
                { "CCLocalLevels2", file.CCLocalLevels2 },
            };
        }
    };