#pragma once

#include <string>

namespace memo {
namespace ui {

class MenuItem
{
public:
    MenuItem();
    MenuItem(int id, const std::string& name, const std::string& description="");
    MenuItem(const MenuItem& other);
    MenuItem(const MenuItem&& other) noexcept;
    MenuItem& operator=(const MenuItem& other);

    int getId() const;
    void setId(int id);

    const std::string& getName() const;
    void setName(const std::string& name);

    const std::string& getDescription() const;
    void setDescription(const std::string description);

private:
    int id_;
    std::string name_;
    std::string description_;
};

} // namespace ui
} // namespace memo
