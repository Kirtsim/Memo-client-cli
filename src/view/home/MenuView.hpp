#pragma once
#include "view/BaseView.hpp"
#include "view/home/MenuItem.hpp"

#include <string>
#include <vector>
#include <array>

struct tagITEM;
struct tagMENU;

namespace memo {
namespace ui {

class MenuView : public BaseView
{
    static const int MENU_ITEM_COUNT = 6;
    static const std::vector<std::string> kMenuItemNames;

    class Layout
    {
    public:
        Layout(Rows iRows, Cols iCols);

        bool operator==(const Layout& other);
        bool operator!=(const Layout& other);

        int rows;
        int cols;
    };

public:
    explicit MenuView(IView* iParent=nullptr);
    explicit MenuView(const Size& iSize, IView* iParent=nullptr);
    MenuView(const Size& iSize, const Position& iPosition, IView* iParent=nullptr);
    ~MenuView();

    void setMenuItems(const std::vector<MenuItem>& iItems);
    void setLayout(Rows iRows, Cols iCols);
    void setSelectionMark(const std::string& iMark);

    int navigateMenuUp();
    int navigateMenuDown();
    int navigateMenuLeft();
    int navigateMenuRight();

    std::pair<bool, MenuItem> getSelected() const;

    Size getMinimumRequiredSize() const;
    void applyMenuChanges();

protected:
    void beforeViewResized() override;
    void positionComponents(Window_t& ioWindow) override;

private:
    void freeTagItems(std::vector<tagITEM*>& ioTagItems);
    bool menuChanged();

    bool menuPositionChanged();
    bool menuMarkerChanged();
    bool menuSizeChanged();
    bool menuLayoutChanged();


    std::vector<MenuItem> menuItems_;
    std::vector<tagITEM*> tagItems_;
    WindowPtr_t menuWindow_;
    std::unique_ptr<tagMENU> menu_;

    Size oldMenuWindowSize_;
    Size menuWindowSize_;
    Position oldMenuWindowPos_;
    Position menuWindowPos_;
    Layout menuWindowLayout_;
    std::string selectionMark_;
    bool menuItemsChanged_;
};

} // namespace ui
} // namespace memo
