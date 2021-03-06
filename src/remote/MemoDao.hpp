#pragma once
#include "model/Memo.pb.h"
#include <vector>
#include <memory>

namespace memo {
namespace remote {

class MemoCallFactory;

class IMemoDao
{
public:
    virtual ~IMemoDao() = default;
    virtual std::vector<model::Memo> fetchAll() = 0;
};

class MemoDaoImpl : public IMemoDao
{
public:
    static std::shared_ptr<MemoDaoImpl> Create(std::unique_ptr<MemoCallFactory> callFactory);

    MemoDaoImpl(std::unique_ptr<MemoCallFactory> callFactory);
    std::vector<model::Memo> fetchAll() override;

    bool success();

private:
    bool success_ = false;
    std::unique_ptr<MemoCallFactory> factory_;
};

} // namespace remote
} // namespace memo
