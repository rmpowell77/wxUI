#pragma once

#include <functional>
#include <tuple>
#include <variant>
#include <wx/frame.h>
#include <wx/menu.h>

// wxUI::Menu is a way to lay out menus in a declarative, visual way.
//
// The general concept is you declare a set of structures and then `attachTo`
// a frame.
//
// ```
// wxUI::MenuBar {
//     wxUI::Menu {
//         "&File",
//         wxUI::Item { "&Example...\tCtrl-D", [this]() {
//                   ExampleDialog1 dialog(this);
//                   dialog.ShowModal();
//               } },
//         wxUI::Separator {},
//         wxUI::Item { wxID_EXIT },
//     },
// }.attachTo(theFrame);
// ```
//
// In wxWidgets the general paradigm is to create an enumeration
// of identity ints that you associate with a member, then you would bind,
// either statically or dynamically, to a function.  With wxUI::Menu the
// construction of the identify and assocation with a function is handled
// automatically.  By default wxUI::Menu starts the enumeration with
// `wxID_AUTO_LOWEST` and increments for each item.  Take caution if you use
// these enumerations as it may collide with other ids assocated with the frame.
//
// The top level `MenuBar` holds a collection of `Menu` objects.  The `Menu`
// object consists of a name of the menu, and a collection of "Items", which
// can be one of `Item` (normal), `Separator`, `CheckItem`, and `RadioItem`.
//
// Menu Items are generally a name with a handler lambda, or name and help with
// a lambda.  Menu Items can also be assocated with `wxStandardID`.  Many of
// these like `wxID_EXIT` and `wxID_HELP` have predefined name, help, and
// handlers, so declaration with just an ID is allowed.
//
// Handlers are callable items that handle events.  The handler can be declared
// with both no arguments or the `wxCommandEvent` argument for deeper inspection
// of the event.
//
// ```
//         wxUI::Item { "&Example1...\tCtrl-D", [this]() {
//                   wxLogMessage("Hello World!");
//               } },
//         wxUI::CheckItem { "&Example2...\tCtrl-D", [this](wxCommandEvent& event) {
//                   wxLogMessage(event.IsChecked() ? "is checked" : "is not checked");
//               } },
// ```
//
// Menu items (except `Separator`) follow the general pattern:
//
// ```
// Items { ID }  // for primatives that have a system handler
// Items { ID, "Name" }
// Items { ID, "Name", "Help" }
// Items { ID, Handler }
// Items { ID, "Name", Handler }
// Items { ID, "Name", "Help", Handler }
// Items { "Name", Handler }
// Items { "Name", "Help", Handler }
// ```
//
// wxUI::Menu also allows nesting of menus.  This allows complicated menus to
// be composed easily.
//
// ```
// wxUI::MenuBar {
//     wxUI::Menu {
//         "Menu",
//         wxUI::Menu {
//             "SubMenu",
//             wxUI::Item { wxID_EXIT },
//         },
//     },
// }.attachTo(theFrame);
// ```
//
// The wxUI::MenuBar and related objects are generally "lazy" objects.  They
// hold the details of the menu layout, but do not call any wxWidget primatives
// on construction.  When `attachTo` a frame is invoked does the underlying
// logic construct the menu structure.

namespace wxUI {

namespace details {

    template <class>
    inline constexpr bool always_false_v = false;

    template <typename T>
    concept MenuBarItem = requires(T widget, wxFrame f, wxMenuBar m, int i)
    {
        widget.createAndAdd(f, m, i);
    };

    template <typename T>
    concept MenuItem = requires(T widget, wxFrame f, wxMenu m, int i)
    {
        widget.createAndAdd(f, m, i);
    };

}

namespace details {

    // We provide a way to bind either a function that takes in a commandEvent
    // not to a frame at a specific id
    using functionWithCmd_t = std::function<void(wxCommandEvent&)>;
    using functionWOCmd_t = std::function<void()>;
    using function_t = std::variant<functionWithCmd_t, functionWOCmd_t>;

    inline void bindToFrame(wxFrame& frame, int identity, function_t const& f)
    {
        std::visit([&frame, identity](auto const& i) {
            using T = std::decay_t<decltype(i)>;
            if constexpr (std::is_same_v<T, functionWithCmd_t>) {
                frame.Bind(wxEVT_MENU, i, identity);
            } else if constexpr (std::is_same_v<T, functionWOCmd_t>) {
                frame.Bind(
                    wxEVT_MENU, [i](wxCommandEvent&) { i(); }, identity);
            } else {
                static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }
        },
            f);
    }

}

namespace details {
    // This is for "type erasure": we support several modes of creating a menu,
    // these are the details on how it is added.
    // We support
    // ID -> IDMenuDetails_t with defaults
    // ID, name -> IDMenuDetails_t with defaults
    // ID, name, help -> IDMenuDetails_t
    // ID, function -> IDMenuDetailsWFunc_t with defaults
    // ID, name, function -> IDMenuDetailsWFunc_t with defaults
    // ID, name, help, function -> IDMenuDetailsWFunc_t
    // name, function -> NamedMenuDetails_t with defaults
    // name, help, function -> NamedMenuDetails_t with defaults
    using IDMenuDetails_t = std::tuple<wxStandardID, std::string, std::string>;
    using IDMenuDetailsWFunc_t = std::tuple<wxStandardID, std::string, std::string, function_t>;
    using NamedMenuDetails_t = std::tuple<std::string, std::string, function_t>;
    using MenuDetails = std::variant<IDMenuDetails_t, IDMenuDetailsWFunc_t, NamedMenuDetails_t>;

    // If the details are named, we use and increment the identity supplied.
    // The way to add is passed by the caller.  And
    template <typename AppendFunction>
    inline void createAndAdd(wxFrame& frame, MenuDetails const& item, int& identity, AppendFunction appendFunction)
    {
        std::visit([&frame, &identity, appendFunction](auto const& i) {
            using T = std::decay_t<decltype(i)>;
            if constexpr (std::is_same_v<T, IDMenuDetails_t>) {
                appendFunction(std::get<0>(i), std::get<1>(i), std::get<2>(i));
            } else if constexpr (std::is_same_v<T, IDMenuDetailsWFunc_t>) {
                appendFunction(std::get<0>(i), std::get<1>(i), std::get<2>(i));
                bindToFrame(frame, std::get<0>(i), std::get<3>(i));
            } else if constexpr (std::is_same_v<T, NamedMenuDetails_t>) {
                appendFunction(identity, std::get<0>(i), std::get<1>(i));
                bindToFrame(frame, identity, std::get<2>(i));
                identity += 1;
            } else {
                static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }
        },
            item);
    }
}

struct Item {
    Item(wxStandardID identity, std::string const& name = "", std::string const& helpString = "")
        : menuDetails(details::IDMenuDetails_t { identity, name, helpString })
    {
    }

    template <typename F>
    Item(wxStandardID identity, F f)
        : Item(identity, "", "", f)
    {
    }

    template <typename F>
    Item(wxStandardID identity, std::string const& name, F f)
        : Item(identity, name, "", f)
    {
    }

    template <typename F>
    Item(wxStandardID identity, std::string const& name, std::string const& helpString, F f)
        : menuDetails(details::IDMenuDetailsWFunc_t(identity, name, helpString, f))
    {
    }

    template <typename F>
    Item(std::string const& name, F f)
        : Item(name, "", f)
    {
    }

    template <typename F>
    Item(std::string const& name, std::string const& help, F f)
        : menuDetails(details::NamedMenuDetails_t(name, help, f))
    {
    }

    void createAndAdd(wxFrame& frame, wxMenu& menu, int& identity)
    {
        details::createAndAdd(frame, menuDetails, identity, [&menu](int id, wxString const& item, wxString const& helpString) {
            menu.Append(id, item, helpString);
        });
    }
    details::MenuDetails menuDetails;
};

struct CheckItem {
    CheckItem(wxStandardID identity, std::string const& name = "", std::string const& helpString = "")
        : menuDetails(details::IDMenuDetails_t { identity, name, helpString })
    {
    }

    template <typename F>
    CheckItem(wxStandardID identity, F f)
        : CheckItem(identity, "", "", f)
    {
    }

    template <typename F>
    CheckItem(wxStandardID identity, std::string const& name, F f)
        : CheckItem(identity, name, "", f)
    {
    }

    template <typename F>
    CheckItem(wxStandardID identity, std::string const& name, std::string const& helpString, F f)
        : menuDetails(details::IDMenuDetailsWFunc_t(identity, name, helpString, f))
    {
    }

    template <typename F>
    CheckItem(std::string const& name, F f)
        : CheckItem(name, "", f)
    {
    }

    template <typename F>
    CheckItem(std::string const& name, std::string const& help, F f)
        : menuDetails(details::NamedMenuDetails_t(name, help, f))
    {
    }

    void createAndAdd(wxFrame& frame, wxMenu& menu, int& identity)
    {
        details::createAndAdd(frame, menuDetails, identity, [&menu](int id, wxString const& item, wxString const& helpString) {
            menu.AppendCheckItem(id, item, helpString);
        });
    }
    details::MenuDetails menuDetails;
};

struct RadioItem {
    RadioItem(wxStandardID identity, std::string const& name = "", std::string const& helpString = "")
        : menuDetails(details::IDMenuDetails_t { identity, name, helpString })
    {
    }

    template <typename F>
    RadioItem(wxStandardID identity, F f)
        : RadioItem(identity, "", "", f)
    {
    }

    template <typename F>
    RadioItem(wxStandardID identity, std::string const& name, F f)
        : RadioItem(identity, name, "", f)
    {
    }

    template <typename F>
    RadioItem(wxStandardID identity, std::string const& name, std::string const& helpString, F f)
        : menuDetails(details::IDMenuDetailsWFunc_t(identity, name, helpString, f))
    {
    }

    template <typename F>
    RadioItem(std::string const& name, F f)
        : RadioItem(name, "", f)
    {
    }

    template <typename F>
    RadioItem(std::string const& name, std::string const& help, F f)
        : menuDetails(details::NamedMenuDetails_t(name, help, f))
    {
    }

    void createAndAdd(wxFrame& frame, wxMenu& menu, int& identity)
    {
        details::createAndAdd(frame, menuDetails, identity, [&menu](int id, wxString const& item, wxString const& helpString) {
            menu.AppendRadioItem(id, item, helpString);
        });
    }
    details::MenuDetails menuDetails;
};

struct Separator {
    void createAndAdd(wxFrame&, wxMenu& menu, int& identity)
    {
        menu.AppendSeparator();
    }
};

// a submenu constructs menu to give to a menubar
template <details::MenuItem... M>
struct SubMenu {
    SubMenu(std::string const& name, M const&... items)
        : SubMenu(name, std::make_tuple(items...))
    {
    }

    SubMenu(std::string const& name, std::tuple<M...> const& items)
        : name(name)
        , items(items)
    {
    }

    void createAndAdd(wxFrame& frame, wxMenu& menuBar, int& identity)
    {
        auto menu = std::make_unique<wxMenu>();
        std::apply([&frame, menu = menu.get(), &identity](auto&&... tupleArg) {
            (tupleArg.createAndAdd(frame, *menu, identity), ...);
        },
            items);
        menuBar.AppendSubMenu(menu.release(), name);
    }

    std::string name;
    std::tuple<M...> items;
};

// a submenu constructs menu to give to a menubar
template <details::MenuItem... M>
struct Menu {
    Menu(std::string const& name, M const&... items)
        : Menu(name, std::make_tuple(items...))
    {
    }

    Menu(std::string const& name, std::tuple<M...> const& items)
        : name(name)
        , items(items)
    {
    }

    void createAndAdd(wxFrame& frame, wxMenuBar& menuBar, int& identity)
    {
        auto menu = std::make_unique<wxMenu>();
        std::apply([&frame, menu = menu.get(), &identity](auto&&... tupleArg) {
            (tupleArg.createAndAdd(frame, *menu, identity), ...);
        },
            items);
        menuBar.Append(menu.release(), name);
    }

    void createAndAdd(wxFrame& frame, wxMenu& menuBar, int& identity)
    {
        auto menu = std::make_unique<wxMenu>();
        std::apply([&frame, menu = menu.get(), &identity](auto&&... tupleArg) {
            (tupleArg.createAndAdd(frame, *menu, identity), ...);
        },
            items);
        menuBar.AppendSubMenu(menu.release(), name);
    }

    std::string name;
    std::tuple<M...> items;
};

template <details::MenuBarItem... M>
struct MenuBar {
    MenuBar(M const&... menus)
        : MenuBar(std::make_tuple(menus...))
    {
    }
    MenuBar(std::tuple<M...> const& menus)
        : menus(menus)
    {
    }

    auto& attachTo(wxFrame* frame)
    {
        auto numbering = int(wxID_AUTO_LOWEST);
        auto menuBar = std::make_unique<wxMenuBar>();
        std::apply([frame, menuBar = menuBar.get(), &numbering](auto&&... tupleArg) {
            (tupleArg.createAndAdd(*frame, *menuBar, numbering), ...);
        },
            menus);
        frame->SetMenuBar(menuBar.release());
        return *this;
    }

    std::tuple<M...> menus;
};
}