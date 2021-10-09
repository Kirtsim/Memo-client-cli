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
    tag.setColour(Int2Color(proto.color()));
    return tag;
}

proto::Tag ToProto(const model::Tag& model)
{
    proto::Tag tag;
    tag.set_id(model.id());
    tag.set_name(model.name());
    tag.set_timestamp(model.timestamp());
    tag.set_color(Color2Int(model.colour()));
    return tag;
}

} // namespace Tag

int Color2Int(const model::Color& color)
{
    int ones = ~0;
    int redValue = (ones & color.red) << 16;
    int greenValue = (ones & color.green) << 8;
    int blueValue = (ones & color.blue);

    return (redValue | greenValue | blueValue );
}

model::Color Int2Color(int colorValue)
{
    int ones = 0xFF; // 0000 0000  0000 0000  0000 0000  1111 1111
    model::Color color;
    color.blue = (colorValue & ones);
    colorValue = colorValue >> 8;
    color.green = (colorValue & ones);
    colorValue = colorValue >> 8;
    color.red = (colorValue & ones);
    return color;
}
} // namespace memo
