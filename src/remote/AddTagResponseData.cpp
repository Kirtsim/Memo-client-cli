#include "remote/AddTagResponseData.hpp"


namespace memo::remote {

const std::string& AddTagResponseData::requestUuid() const
{
    return requestUuid_;
}

const model::TagPtr& AddTagResponseData::tag() const
{
    return tag_;
}

AddTagResponseDataBuilder& AddTagResponseDataBuilder::setRequestUuid(const std::string& uuid)
{
    data_.requestUuid_ = uuid;
    return *this;
}

AddTagResponseDataBuilder& AddTagResponseDataBuilder::setTag(const model::TagPtr& tag)
{
    data_.tag_ = tag;
    return *this;
}

AddTagResponseData AddTagResponseDataBuilder::build() const
{
    return data_;
}
} // namespace memo::remote