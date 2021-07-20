#pragma once
#include "TagSvc.grpc.pb.h"
#include <memory>

namespace memo::remote {

class ListTagsCall;
class AddTagCall;
using TagServiceStubPtr_t = std::shared_ptr<proto::TagService::Stub>;

class ITagCallFactory
{
public:
    virtual ~ITagCallFactory() = default;

    virtual std::shared_ptr<ListTagsCall> createListTagsCall() = 0;

    virtual std::shared_ptr<AddTagCall> createAddTagCall() = 0;
};

class TagCallFactoryImpl : public ITagCallFactory
{
public:

    explicit TagCallFactoryImpl(const TagServiceStubPtr_t& tagServiceStub);

    ~TagCallFactoryImpl() override;

    std::shared_ptr<ListTagsCall> createListTagsCall() override;

    std::shared_ptr<AddTagCall> createAddTagCall() override;

private:
    TagServiceStubPtr_t stub_;
};
} // namespace memo::remote
