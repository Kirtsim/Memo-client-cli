#pragma once

#include <string>

namespace memo {
namespace ui {

class MenuItem
{
public:
    MenuItem();
    MenuItem(int iId, const std::string& iName, const std::string& iDescription="");
    MenuItem(const MenuItem& iOther);
    MenuItem(const MenuItem&& iOther) noexcept;
    MenuItem& operator=(const MenuItem& iOther);

    int getId() const;
    void setId(int iId);

    const std::string& getName() const;
    void setName(const std::string& iName);

    const std::string& getDescription() const;
    void setDescription(const std::string iDescription);

private:
    int id_;
    std::string name_;
    std::string description_;
};

} // namespace ui
} // namespace memo
