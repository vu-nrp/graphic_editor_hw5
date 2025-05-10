#pragma once

#include <memory>

//!
//! \brief The IButtonInfo struct - description of tool button
//!

struct IButtonInfo
{
    using Shared = std::shared_ptr<IButtonInfo>;

    //!
    //! Type - unique button id
    //!
    using Type = uint32_t;

    virtual ~IButtonInfo() = default;

};
