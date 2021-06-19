#pragma once
#include "remote/RemoteDefs.hpp"
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

    virtual const ResponseType& data() const = 0;
};

template <class ResponseType>
class ServiceResponseBuilder;

template <class ResponseType>
class ServiceResponseImpl : public IServiceResponse<ResponseType>
{
    friend class ServiceResponseBuilder<ResponseType>;
public:
    ~ServiceResponseImpl() = default;

    int code() const override;

    Status status() const override;

    const std::string& statusMessage() const override;

    const std::string& requestId() const override;

    const ResponseType& data() const override;

private:
    void setCode(int code);

    void setStatus(Status status);

    void setStatusMessage(const std::string& statusMessage);

    void setRequestId(const std::string& requestId);

    void setData(const ResponseType& data);

private:
    int code_ = 0;
    Status status_ = kError;
    std::string statusMessage_;
    std::string requestId_;
    ResponseType data_;
};

template<class ResponseType>
int ServiceResponseImpl<ResponseType>::code() const
{
    return code_;
}

template<class ResponseType>
Status ServiceResponseImpl<ResponseType>::status() const
{
    return status_;
}

template<class ResponseType>
const std::string& ServiceResponseImpl<ResponseType>::statusMessage() const
{
    return statusMessage_;
}

template<class ResponseType>
const std::string& ServiceResponseImpl<ResponseType>::requestId() const
{
    return requestId_;
}

template<class ResponseType>
const ResponseType& ServiceResponseImpl<ResponseType>::data() const
{
    return data_;
}

template<class ResponseType>
void ServiceResponseImpl<ResponseType>::setCode(int code)
{
    code_ = code;
}

template<class ResponseType>
void ServiceResponseImpl<ResponseType>::setStatus(Status status)
{
    status_ = status;
}

template<class ResponseType>
void ServiceResponseImpl<ResponseType>::setStatusMessage(const std::string& statusMessage)
{
    statusMessage_ = statusMessage;
}

template<class ResponseType>
void ServiceResponseImpl<ResponseType>::setRequestId(const std::string& requestId)
{
    requestId_ = requestId;
}

template<class ResponseType>
void ServiceResponseImpl<ResponseType>::setData(const ResponseType& data)
{
    data_ = data;
}

template <class ResponseType>
class ServiceResponseBuilder
{
public:
    ServiceResponseBuilder& setCode(int code)
    {
        response_.setCode(code);
        return *this;
    }

    ServiceResponseBuilder& setStatus(Status status)
    {
        response_.setStatus(status);
        return *this;
    }

    ServiceResponseBuilder& setStatusMessage(const std::string& statusMessage)
    {
        response_.setStatusMessage(statusMessage);
        return *this;
    }

    ServiceResponseBuilder& setRequestId(const std::string& requestId)
    {
        response_.setRequestId(requestId);
        return *this;
    }

    const ResponseType& setData(const ResponseType& data)
    {
        response_.setData(data);
        return *this;
    }

    std::shared_ptr<IServiceResponse<ResponseType>> build() const
    {
        return std::make_shared<ServiceResponseImpl<ResponseType>>(response_);
    }

private:
    ServiceResponseImpl<ResponseType> response_;
};

} // namespace memo::remote
