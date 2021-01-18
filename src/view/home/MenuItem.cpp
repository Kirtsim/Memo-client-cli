#include "view/home/MenuItem.hpp"

namespace memo {
namespace ui {

////////////////////////////////////////////////////////////////////////
///                     MenuItem
////////////////////////////////////////////////////////////////////////
MenuItem::MenuItem() :
    MenuItem(0, "", "")
{}

MenuItem::MenuItem(int id, const std::string& name, const std::string& description) :
    id_(id),
    name_(name),
    description_(description)
{}

MenuItem::MenuItem(const MenuItem& other) :
    id_(other.id_),
    name_(other.name_),
    description_(other.description_)
{}

MenuItem::MenuItem(const MenuItem&& other) noexcept :
    id_(other.id_),
    name_(std::move(other.name_)),
    description_(std::move(other.description_))
{}

MenuItem& MenuItem::operator=(const MenuItem& other)
{
    if (this != &other)
    {
        id_ = other.id_;
        name_ = other.name_;
        description_ = other.description_;
    }
    return *this;
}

int MenuItem::getId() const
{
    return id_;
}

void MenuItem::setId(int id)
{
    id_ = id;
}

const std::string& MenuItem::getName() const
{
    return name_;
}

void MenuItem::setName(const std::string& name)
{
    name_ = name;
}

const std::string& MenuItem::getDescription() const
{
    return description_;
}

void MenuItem::setDescription(const std::string description)
{
    description_ = description;
}

} // namespace ui
} // namespace memo
