#pragma once
#include <string>
#include <memory>

namespace memo::remote {

enum Status
{
    kSuccess, kWarning, kError
};

template <class ResponseType>
class IServiceResponse
{
public:
    virtual ~IServiceResponse() = default;

    virtual int code() const = 0;

    virtual Status status() const = 0;

    virtual const std::string& statusMessage() const = 0 ;

    virtual const std::string& requestId() const = 0;

    virtual const ResponseType data() const = 0;
};


template <class ResponseType>
class ServiceResponseImpl : public IServiceResponse<ResponseType>
{
public:
    ServiceResponseImpl(
            int code,
            Status status,
            const std::string& statusMessage,
            const std::string& requestId,
            const ResponseType& data)
            : code_(code)
            , status_(status)
            , statusMessage_(statusMessage)
            , requestId_(requestId)
            , data_(data)
    {}

    ~ServiceResponseImpl() = default;

    int code() const override
    {
        return code_;
    }

    Status status() const override
    {
        return status_;
    }

    const std::string& statusMessage() const override
    {
        return statusMessage_;
    }

    const std::string& requestId() const override
    {
        return requestId_;
    }

    const ResponseType& data() const override
    {
        return data_;
    }

private:
    int code_ = 0;
    Status status_ = kError;
    std::string statusMessage_;
    std::string requestId_;
    ResponseType data_;
};

} // namespace memo::remote
