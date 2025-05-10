#include "settingsstorage.h"

//!
//! \brief SettingStorage::instance - take application settings storage
//! \return
//!

SettingStorage &SettingStorage::instance()
{
    static SettingStorage storage;
    return storage;
}
