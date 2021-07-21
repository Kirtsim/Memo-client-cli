#pragma once
#include "model/ModelDefs.hpp"
#include <string>

namespace memo::remote {

class AddTagRequest
{
    friend class AddTagRequestBuilder;
public:
    const std::string& uuid() const;

    const model::TagPtr& tag() const;

private:
    std::string uuid_;
    model::TagPtr tag_;
};

class AddTagRequestBuilder
{
public:
    AddTagRequestBuilder& setUuid(const std::string& uuid);

    AddTagRequestBuilder& setTag(const model::TagPtr& tag);

    AddTagRequest build() const;

private:
    AddTagRequest request_;
};


} // namespace memo::remote
