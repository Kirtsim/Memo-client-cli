#pragma once

namespace memo::remote {

enum ModelView {
    kMinimalView,
    kCompleteView
};

enum ResponseStatus
{
    kSuccess, kWarning, kError
};
} // namespace memo::remote