#include "remote/utils/Mapping.hpp"

namespace memo::remote::utils {

proto::OperationStatus_Type Model2Proto(ResponseStatus status)
{
    switch (status)
    {
        case ResponseStatus::kSuccess: return proto::OperationStatus_Type_SUCCESS;
        case ResponseStatus::kWarning: return proto::OperationStatus_Type_WARNING;
        default: return proto::OperationStatus_Type_FAILURE;
    }
}

ResponseStatus Proto2Model(proto::OperationStatus_Type statusType)
{
    switch (statusType)
    {
        case proto::OperationStatus_Type_SUCCESS: return ResponseStatus::kSuccess;
        case proto::OperationStatus_Type_WARNING: return ResponseStatus::kWarning;
        default: return ResponseStatus::kError;
    }
}

proto::ModelView Model2Proto(ModelView modelView)
{
    switch (modelView)
    {
        case ModelView::kMinimalView: return proto::ModelView::MINIMAL;
        default: return proto::ModelView::COMPLETE;
    }
}

ModelView Proto2Model(proto::ModelView modelView)
{
    switch (modelView)
    {
        case proto::ModelView::MINIMAL: return ModelView::kMinimalView;
        default: return ModelView::kCompleteView;
    }
}

} // namespace memo::remote::utils