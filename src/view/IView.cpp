#include "view/IView.hpp"

namespace memo {
namespace ui {

IView::IView(IComponent* parent) :
    IView(Size(), Position(), parent)
{}

IView::IView(const Size& size, IComponent* parent) :
    IView(size, Position(), parent)
{}

IView::IView(const Size& size, const Position& position, IComponent* parent) :
    BaseComponent(size, position, parent)
{}

} // namespace ui
} // namespace memo
