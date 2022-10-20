#pragma once

#include <functional>
#include <tuple>
#include <variant>
#include <wx/frame.h>
#include <wx/menu.h>

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