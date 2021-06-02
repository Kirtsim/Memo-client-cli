#pragma once
#include <string>

namespace memo::model {

class Tag
{
public:
    const std::string& name() const;
    void setName(const std::string& name);

    const std::string& color() const;
    void setColor(const std::string& color);

    unsigned long timestamp() const;
    void setTimestamp(unsigned long timestamp);

private:
    std::string name_;
    std::string color_;
    unsigned long timestamp_;
};

} // namespace memo::model
