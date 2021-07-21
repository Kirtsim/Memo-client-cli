#pragma once
#include "OperationStatus.pb.h"
#include "remote/ServiceEnums.hpp"
#include "ModelView.pb.h"

namespace memo::remote::utils  {

proto::OperationStatus_Type Model2Proto(ResponseStatus status);

ResponseStatus Proto2Model(proto::OperationStatus_Type statusType);

proto::ModelView Model2Proto(ModelView modelView);

ModelView Proto2Model(proto::ModelView modelView);



} // namespace memo::remote::utils