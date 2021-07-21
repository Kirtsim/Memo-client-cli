#include "utils/ModelMapper.hpp"

namespace memo {
namespace Memo {

model::Memo ToModel(const proto::Memo& proto, const std::map<unsigned long, model::TagPtr>& tags)
{
    model::Memo memo;
    memo.setId(proto.id());
    memo.setTitle(proto.title());
    memo.setDescription(proto.description());
    for (const auto& tagId : proto.tag_ids())
    {
        auto iter = tags.find(tagId);
        if (iter != std::end(tags) && iter->second)
            memo.addTag(iter->second);
    }
    memo.setTimestamp(proto.timestamp());
    return memo;
}

proto::Memo ToProto(const model::Memo& model)
{
    proto::Memo memo;
    memo.set_id(model.id());
    memo.set_title(model.title());
    memo.set_description(model.description());
    for (const auto& tag : model.tags())
    {
        if (tag)
        {
            memo.add_tag_ids(tag->id());
        }
    }
    memo.set_timestamp(model.timestamp());
    return memo;
}

} // namespace Memo

namespace Tag {

model::Tag ToModel(const proto::Tag& proto)
{
    model::Tag tag;
    tag.setId(proto.id());
    tag.setName(proto.name());
    tag.setTimestamp(proto.timestamp());
    const auto& protoColour = proto.colour();
    model::Colour colour { protoColour.red(), protoColour.green(), protoColour.blue() };
    tag.setColour(colour);
    return tag;
}

proto::Tag ToProto(const model::Tag& model)
{
    proto::Tag tag;
    tag.set_name(model.name());
    tag.set_timestamp(model.timestamp());
    const auto& modelColour = model.colour();
    tag.mutable_colour()->set_red(modelColour.red);
    tag.mutable_colour()->set_green(modelColour.green);
    tag.mutable_colour()->set_blue(modelColour.blue);
    return tag;
}

} // namespace Tag

} // namespace memo
