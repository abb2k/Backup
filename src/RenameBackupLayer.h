#pragma once
#include "Geode/ui/TextInput.hpp"
#include <BackupCell.h>
#include <BackupsLayer.h>
#include <include.h>

class RenameBackupLayer : public Popup<BackupsLayer* const&, BackupCell* const&>, public FLAlertLayerProtocol {
protected:
  virtual bool setup(BackupsLayer* const& _parentLayer, BackupCell* const& cell);

public:
  static RenameBackupLayer* create(BackupsLayer* const& parentLayer, BackupCell* const& cell);

  virtual void show(CCNode* parent);

  void keyBackClicked();

  void onClose(CCObject* object);

  TextInput* NameInput;

  void DoneAndSave(CCObject* object);

  BackupsLayer* parentLayer;

  BackupCell* backupCell;

  FLAlertLayer* exportAlert;

  void OnExport(CCObject* object);

  void FLAlert_Clicked(FLAlertLayer* p0, bool p1);
};
