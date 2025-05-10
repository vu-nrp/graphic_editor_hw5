#pragma once

#include <vector>
#include "buttons.h"

//!
//! \brief The SettingStorage class - some application storage
//!

class SettingStorage
{
public:
    std::vector<IButtonInfo::Type> viewButtons() const
    {
        // some read from storage buttons ids list
        return {};
    }

    static SettingStorage &instance();

protected:
    SettingStorage()
    {
        std::printf("greate application setting storage\n");
    }

};
