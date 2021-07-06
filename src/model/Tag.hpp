#pragma once
#include <string>

namespace memo::model {

struct Colour
{
    int red;
    int green;
    int blue;

    bool operator==(const Colour& other) const;

    bool operator!=(const Colour& other) const;
};

class Tag
{
public:
    unsigned long id() const;
    void setId(unsigned long id);

    const std::string& name() const;
    void setName(const std::string& name);

    const Colour& colour() const;
    void setColour(const Colour& colour);

    unsigned long timestamp() const;
    void setTimestamp(unsigned long timestamp);

    bool operator==(const Tag& other) const;

    bool operator!=(const Tag& other) const;

private:
    unsigned long id_;
    std::string name_;
    Colour colour_;
    unsigned long timestamp_;
};

} // namespace memo::model
