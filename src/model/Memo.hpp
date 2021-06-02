#pragma once
#include <vector>
#include <string>

namespace memo::model {

class Memo
{
public:
    const std::string& id() const;
    void setId(const std::string& id);

    const std::string& title() const;
    void setTitle(const std::string& title);

    const std::string& description() const;
    void setDescription(const std::string& description);

    const std::vector<std::string>& tagNames() const;
    void setTagNames(const std::vector<std::string>& tagNames);
    void addTagName(const std::string& tagName);

    unsigned long timestamp() const;
    void setTimestamp(unsigned long timestamp);

private:
    std::string id_;
    std::string title_;
    std::string description_;
    std::vector<std::string> tagNames_;
    unsigned long timestamp_;
};

} // namespace memo::model
