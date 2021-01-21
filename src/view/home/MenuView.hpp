#pragma once
#include "view/BaseView.hpp"
#include "view/home/MenuItem.hpp"

#include <string>
#include <vector>
#include <array>

struct tagITEM;
struct tagMENU;

namespace memo {
namespace curses {
    class SubWindow;
} // namespace curses
namespace ui {

class MenuView : public BaseView
{
    static const int MENU_ITEM_COUNT = 6;
    static const std::vector<std::string> kMenuItemNames;

    class Layout
    {
    public:
        Layout(Rows rows, Cols cols);

        bool operator==(const Layout& other) const;
        bool operator!=(const Layout& other) const;

        int rows;
        int cols;
    };

public:
    explicit MenuView(IComponent* parent=nullptr);
    explicit MenuView(const Size& size, IComponent* parent=nullptr);
    MenuView(const Size& size, const Position& position, IComponent* parent=nullptr);
    ~MenuView();

    void setMenuItems(const std::vector<MenuItem>& items);
    void setLayout(Rows rows, Cols cols);
    void setSelectionMark(const std::string& mark);

    int navigateMenuUp();
    int navigateMenuDown();
    int navigateMenuLeft();
    int navigateMenuRight();

    std::pair<bool, MenuItem> getSelected() const;

    Size getMinimumRequiredSize() const;
    void applyMenuChanges();

protected:
    void positionComponents() override;

private:
    void freeTagItems(std::vector<tagITEM*>& ioTagItems);
    bool menuChanged();

    bool menuPositionChanged();
    bool menuMarkerChanged();
    bool menuSizeChanged();
    bool menuLayoutChanged();


    std::vector<MenuItem> menuItems_;
    std::vector<tagITEM*> tagItems_;
    std::unique_ptr<curses::SubWindow> menuWindow_;
    std::unique_ptr<tagMENU> menu_;

    Size menuWindowSize_;
    Position menuWindowPos_;
    Layout menuWindowLayout_;
    std::string selectionMark_;
    bool menuItemsChanged_;
};

} // namespace ui
} // namespace memo
