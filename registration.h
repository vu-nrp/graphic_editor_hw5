#pragma once

#include <memory>
#include <string>
#include <unordered_map>

//!
//! \brief The CreateItemService class - base registration primitive
//!

class CreateItemService
{
public:
    using Shared = std::shared_ptr<CreateItemService>;
    virtual ~CreateItemService() = default;

};

//!
//! \brief The Registrator class - registration category helper class
//!

struct Registrator
{
    //!
    //! \brief regCreator - registrate creation class
    //! \param storageName - category name
    //! \param creator - category item creator
    //! \return true on success
    //!

    static bool regCreator(const std::string &storageName, const CreateItemService::Shared &creator)
    {
        auto &inst = instance();
        return inst.addCreator(storageName, creator);
    }

    //!
    //! \brief takeStorage - taking item creator class by category
    //! \param storageName - category name
    //! \return
    //!

    static CreateItemService::Shared takeStorage(const std::string &storageName)
    {
        const auto &inst = instance();
        if (inst.contains(storageName))
            return inst.storage(storageName);
        return CreateItemService::Shared();
    }

protected:
    static Registrator &instance()
    {
        static Registrator instance;
        return instance;
    }

    bool addCreator(const std::string &storageName, const CreateItemService::Shared &creator)
    {
        m_regStorage[storageName] = creator;
        return contains(storageName);
    }

    bool contains(const std::string &storageName) const
    {
        return (m_regStorage.count(storageName) > 0);
    }

    CreateItemService::Shared storage(const std::string &storageName) const
    {
        return m_regStorage.at(storageName);
    }

private:
    std::unordered_map<std::string, CreateItemService::Shared> m_regStorage;

};

