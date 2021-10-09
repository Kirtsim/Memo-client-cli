#pragma once
#include "model/ModelDefs.hpp"
#include "model/Memo.hpp"
#include "model/Tag.hpp"
#include "Memo.pb.h"
#include "Tag.pb.h"
#include <map>

namespace memo {
namespace Memo {

model::Memo ToModel(const proto::Memo& proto, const std::map<unsigned long, model::TagPtr>& tags);

proto::Memo ToProto(const model::Memo& model);

} // namespace Memo

namespace Tag {

model::Tag ToModel(const proto::Tag& proto);

proto::Tag ToProto(const model::Tag& model);

} // namespace Tag

int Color2Int(const model::Color& color);

model::Color Int2Color(int colorValue);

} // namespace memo

