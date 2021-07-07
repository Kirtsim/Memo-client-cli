#pragma once
#include "model/ModelDefs.hpp"
#include <vector>
#include <string>

namespace memo::model {

class Memo
{
public:
    unsigned long id() const;
    void setId(unsigned long id);

    const std::string& title() const;
    void setTitle(const std::string& title);

    const std::string& description() const;
    void setDescription(const std::string& description);

    const std::vector<TagPtr>& tags() const;
    void setTags(const std::vector<TagPtr>& tagNames);
    void addTag(const TagPtr& tagName);

    unsigned long timestamp() const;
    void setTimestamp(unsigned long timestamp);

    bool operator==(const Memo& other);

    bool operator!=(const Memo& other);

private:
    unsigned long id_;
    std::string title_;
    std::string description_;
    std::vector<TagPtr> tags_;
    unsigned long timestamp_;
};

} // namespace memo::model
