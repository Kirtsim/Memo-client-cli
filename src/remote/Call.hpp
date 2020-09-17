#pragma once
#include <grpcpp/grpcpp.h>

namespace memo {
namespace remote {

class Call
{
public:
    virtual bool exec() = 0;

    virtual grpc::Status getStatus() const = 0;
};

} // namespace remote
} // namespace memo
