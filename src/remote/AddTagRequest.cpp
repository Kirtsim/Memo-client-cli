#include "remote/AddTagRequest.hpp"
#include "model/Tag.hpp"

namespace memo::remote {

const std::string& AddTagRequest::uuid() const
{
    return uuid_;
}

const model::TagPtr& AddTagRequest::tag() const
{
    return tag_;
}

AddTagRequestBuilder& AddTagRequestBuilder::setUuid(const std::string& uuid)
{
    request_.uuid_ = uuid;
    return *this;
}

AddTagRequestBuilder& AddTagRequestBuilder::setTag(const model::TagPtr& tag)
{
    request_.tag_ = tag;
    return *this;
}

AddTagRequest AddTagRequestBuilder::build() const
{
    return request_;
}

} // namespace memo::remote
