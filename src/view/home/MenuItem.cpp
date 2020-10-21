#include "view/home/MenuItem.hpp"

namespace memo {
namespace ui {

////////////////////////////////////////////////////////////////////////
///                     MenuItem
////////////////////////////////////////////////////////////////////////
MenuItem::MenuItem() :
    MenuItem(0, "", "")
{}

MenuItem::MenuItem(int iId, const std::string& iName, const std::string& iDescription) :
    id_(iId),
    name_(iName),
    description_(iDescription)
{}

MenuItem::MenuItem(const MenuItem& iOther) :
    id_(iOther.id_),
    name_(iOther.name_),
    description_(iOther.description_)
{}

MenuItem::MenuItem(const MenuItem&& iOther) noexcept :
    id_(iOther.id_),
    name_(std::move(iOther.name_)),
    description_(std::move(iOther.description_))
{}

MenuItem& MenuItem::operator=(const MenuItem& iOther)
{
    if (this != &iOther)
    {
        id_ = iOther.id_;
        name_ = iOther.name_;
        description_ = iOther.description_;
    }
    return *this;
}

int MenuItem::getId() const
{
    return id_;
}

void MenuItem::setId(int iId)
{
    id_ = iId;
}

const std::string& MenuItem::getName() const
{
    return name_;
}

void MenuItem::setName(const std::string& iName)
{
    name_ = iName;
}

const std::string& MenuItem::getDescription() const
{
    return description_;
}

void MenuItem::setDescription(const std::string iDescription)
{
    description_ = iDescription;
}

} // namespace ui
} // namespace memo
