#pragma once
#include "model/ModelDefs.hpp"
#include <string>

namespace memo::remote {


class AddTagResponseData
{
    friend class AddTagResponseDataBuilder;
public:
    const std::string& requestUuid() const;

    const model::TagPtr& tag() const;

private:
    std::string requestUuid_;
    model::TagPtr tag_;
};

class AddTagResponseDataBuilder
{
public:
    AddTagResponseDataBuilder& setRequestUuid(const std::string& uuid);

    AddTagResponseDataBuilder& setTag(const model::TagPtr& tag);

    AddTagResponseData build() const;
private:
    AddTagResponseData data_;
};


} // namespace memo::remote
