#pragma once
#include "Memo.pb.h"
#include "MemoSvc.pb.h"
#include "model/Memo.hpp"
#include <vector>
#include <memory>

namespace memo::remote {

class MemoCallFactory;

class IMemoDao
{
public:
    virtual ~IMemoDao() = default;
    virtual std::vector<proto::Memo> fetchAll() = 0;
    virtual proto::MemoCreateRs add(const proto::Memo& memo) = 0;
};

class MemoDaoImpl : public IMemoDao
{
public:
    static std::shared_ptr<MemoDaoImpl> Create(std::unique_ptr<MemoCallFactory> callFactory);

    explicit MemoDaoImpl(std::unique_ptr<MemoCallFactory> callFactory);

    std::vector<proto::Memo> fetchAll() override;

    proto::MemoCreateRs add(const proto::Memo& memo) override;

    bool success() const;

private:
    bool success_ = false;
    std::unique_ptr<MemoCallFactory> factory_;
};

} // namespace memo::remote
