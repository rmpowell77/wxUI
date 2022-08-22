#pragma once

#include <wx/menu.h>
#include <wx/frame.h>


namespace wxUI {

namespace Menu {

struct Item;
struct CheckItem;
struct RadioItem;
struct Separator;
struct SubMenu;

struct If;
struct IfElse;

namespace details {

using MenuDetailsVariant = std::variant<Item, CheckItem, RadioItem, Separator, SubMenu>;
using Conditional = std::variant<If, IfElse>;
using MenuVariant = std::variant<MenuDetailsVariant, Conditional>;

template<class> inline constexpr bool always_false_v = false;

struct Menu {
    Menu(std::initializer_list<details::MenuVariant> items);
    wxMenu *operator*() const { return menu; }
private:
    wxMenu *menu = nullptr;
};

}

struct Item {
    int id = wxID_ANY;
    wxString item = wxEmptyString;
    wxString help = wxEmptyString;
};

struct CheckItem {
    int id = wxID_ANY;
    wxString item = wxEmptyString;
    wxString help = wxEmptyString;
};

struct RadioItem {
    int id = wxID_ANY;
    wxString item = wxEmptyString;
    wxString help = wxEmptyString;
};

struct Separator {
};

struct SubMenu {
    int id = wxID_ANY;
    wxString item = "";
    wxString help = wxEmptyString;
    details::Menu menu;

    SubMenu(int id, wxString&& item, details::Menu&& menu)
    : id(id)
    , item(std::forward<wxString>(item))
    , menu(std::forward<details::Menu>(menu))
    {}

    SubMenu(wxString&& item, details::Menu&& menu)
    : id(wxID_ANY)
    , item(std::forward<wxString>(item))
    , menu(std::forward<details::Menu>(menu))
    {}
};

struct If {
    bool condition;
    std::unique_ptr<details::MenuDetailsVariant> menu;
    If(bool condition, details::MenuDetailsVariant const& menu)
    : condition(condition)
    , menu(new details::MenuDetailsVariant(menu)) {}
};

struct IfElse {
    bool condition;
    std::unique_ptr<details::MenuDetailsVariant> ifMenu;
    std::unique_ptr<details::MenuDetailsVariant> elseMenu;

    IfElse(bool condition, details::MenuDetailsVariant const& ifMenu, details::MenuDetailsVariant const& elseMenu)
    : condition(condition)
    , ifMenu(new details::MenuDetailsVariant(ifMenu))
    , elseMenu(new details::MenuDetailsVariant(elseMenu)) {}
};

namespace details {
inline auto process(wxMenu* menu, MenuDetailsVariant const& item) {
    std::visit([&menu](auto const& i) {
        using T = std::decay_t<decltype(i)>;
        if constexpr (std::is_same_v<T, Item>) {
            menu->Append(i.id, i.item, i.help);
        }
        else if constexpr (std::is_same_v<T, CheckItem>) {
            menu->AppendCheckItem(i.id, i.item, i.help);
        }
        else if constexpr (std::is_same_v<T, RadioItem>) {
            menu->AppendRadioItem(i.id, i.item, i.help);
        }
        else if constexpr (std::is_same_v<T, Separator>) {
            menu->AppendSeparator();
        }
        else if constexpr (std::is_same_v<T, SubMenu>) {
            menu->Append(i.id, i.item, *i.menu, i.help);
        }
        else {
            static_assert(always_false_v<T>, "non-exhaustive visitor!");
        }
    }, item);
    return menu;
}

inline auto process(wxMenu* menu, Conditional const& item) {
    std::visit([&menu](auto const& i) {
        using T = std::decay_t<decltype(i)>;
        if constexpr (std::is_same_v<T, If>) {
            if (i.condition) {
                process(menu, *i.menu);
            }
        }
        else if constexpr (std::is_same_v<T, IfElse>) {
            if (i.condition) {
                process(menu, *i.ifMenu);
            }
            else {
                process(menu, *i.elseMenu);
            }
        }
        else {
            static_assert(always_false_v<T>, "non-exhaustive visitor!");
        }
    }, item);
    return menu;
}

inline auto process(wxMenu* menu, MenuVariant const& item) {
    std::visit([&menu](auto const& i) {
        using T = std::decay_t<decltype(i)>;
        if constexpr (std::is_same_v<T, MenuDetailsVariant>) {
            process(menu, i);
        }
        else if constexpr (std::is_same_v<T, Conditional>) {
            process(menu, i);
        }
        else {
            static_assert(always_false_v<T>, "non-exhaustive visitor!");
        }
    }, item);
    return menu;
}

inline Menu::Menu(std::initializer_list<details::MenuVariant> items)
    : menu(new wxMenu)
{
    for (auto const& item : items) {
        details::process(menu, item);
    }
}

struct MenuBarItem {
    wxString title;
    Menu menu;
};

struct MenuBar {
    MenuBar(std::initializer_list<details::MenuBarItem> items);

    wxMenuBar *operator*() const { return menuBar; }
private:
    wxMenuBar *menuBar = nullptr;
};

inline auto process(wxMenuBar* menuBar, MenuBarItem const& item) {
    menuBar->Append(*item.menu, item.title);
}

inline MenuBar::MenuBar(std::initializer_list<details::MenuBarItem> items)
    : menuBar(new wxMenuBar)
{
    for (auto const& item : items) {
        details::process(menuBar, item);
    }
}
}
}

inline void MenuBar(wxFrame* frame, Menu::details::MenuBar const& menuBar) {
    frame->SetMenuBar(*menuBar);
}

}
