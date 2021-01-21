#include "view/View.hpp"

namespace memo {
namespace ui {

View::View(IComponent* parent) :
    View(Size(), Position(), parent)
{}

View::View(const Size& size, IComponent* parent) :
    View(size, Position(), parent)
{}

View::View(const Size& size, const Position& position, IComponent* parent) :
    BaseComponent(size, position, parent)
{}

} // namespace ui
} // namespace memo
