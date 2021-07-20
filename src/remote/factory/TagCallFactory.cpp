#include "TagCallFactory.hpp"
#include "remote/ListTagsCall.hpp"
#include "remote/AddTagCall.hpp"

namespace memo::remote {

TagCallFactoryImpl::TagCallFactoryImpl(const TagServiceStubPtr_t& tagServiceStub)
    : stub_(tagServiceStub)
{
}

TagCallFactoryImpl::~TagCallFactoryImpl() = default;


std::shared_ptr<ListTagsCall> TagCallFactoryImpl::createListTagsCall()
{
    if (stub_)
        return std::make_shared<ListTagsCall>(*stub_);
    return nullptr;
}

std::shared_ptr<AddTagCall> TagCallFactoryImpl::createAddTagCall()
{
    if (stub_)
        return std::make_shared<AddTagCall>(*stub_);
    return nullptr;
}
} // namespace memo::remote
