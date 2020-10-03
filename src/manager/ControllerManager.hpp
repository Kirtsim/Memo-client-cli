#pragma once
#include <list>
#include <memory>

namespace memo {
namespace ctrl {
    class IController;
}
namespace manager {

class ControllerManager
{
    using ControllerPtr_t = std::shared_ptr<ctrl::IController>;
public:
    void replaceLast(const ControllerPtr_t& iController);
    void add(const ControllerPtr_t& iController);
    ControllerPtr_t pop();

    ControllerPtr_t getCurrent();

    int count() const;
    bool empty() const;

    /// Previous Controllers are not destroyed immediately after a new Controller is added,
    /// given that they still may have ongoing work to finish. Instead, they
    /// are cached and can be removed by calling this function.
    void clearUnused();

private:
    std::list<ControllerPtr_t> controllers_;
    std::list<ControllerPtr_t> forRemoval_;
};

} // namespace manager
} // namespace memo
