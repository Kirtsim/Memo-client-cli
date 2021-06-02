#pragma once
#include "model/Memo.hpp"
#include "model/Tag.hpp"
#include "Memo.pb.h"
#include "Tag.pb.h"

namespace memo {
namespace Memo {

model::Memo ToModel(const proto::Memo& proto);

proto::Memo ToProto(const model::Memo& model);

} // namespace Memo

namespace Tag {

model::Tag ToModel(const proto::Tag& proto);

proto::Tag ToProto(const model::Tag& proto);

} // namespace Tag

} // namespace memo

