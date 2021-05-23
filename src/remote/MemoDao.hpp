#pragma once
#include "model/Memo.pb.h"
#include "model/MemoSvc.pb.h"
#include <vector>
#include <memory>

namespace memo::remote {

class MemoCallFactory;

class IMemoDao
{
public:
    virtual ~IMemoDao() = default;
    virtual std::vector<model::Memo> fetchAll() = 0;
    virtual model::MemoCreateRs add(const model::Memo& memo) = 0;
};

class MemoDaoImpl : public IMemoDao
{
public:
    static std::shared_ptr<MemoDaoImpl> Create(std::unique_ptr<MemoCallFactory> callFactory);

    explicit MemoDaoImpl(std::unique_ptr<MemoCallFactory> callFactory);

    std::vector<model::Memo> fetchAll() override;

    model::MemoCreateRs add(const model::Memo& memo) override;

    bool success() const;

private:
    bool success_ = false;
    std::unique_ptr<MemoCallFactory> factory_;
};

} // namespace memo::remote
