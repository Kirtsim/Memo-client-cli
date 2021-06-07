#pragma once
#include <string>

namespace memo::model {

class Tag
{
public:
    unsigned long id() const;
    void setId(unsigned long id);

    const std::string& name() const;
    void setName(const std::string& name);

    const std::string& color() const;
    void setColor(const std::string& color);

    unsigned long timestamp() const;
    void setTimestamp(unsigned long timestamp);

private:
    unsigned long id_;
    std::string name_;
    std::string color_;
    unsigned long timestamp_;
};

} // namespace memo::model
