/*
MIT License

Copyright (c) 2022 Richard Powell

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once

#include <functional>
#include <memory>
#include <tuple>
#include <variant>
#include <wx/frame.h>
#include <wx/menu.h>

namespace wxUI {

namespace details {

    template <class>
    inline constexpr bool always_false_v = false;

    // clang-format off
    template <typename T>
    concept MenuBarItem = requires(T widget, wxFrame frame, wxMenuBar menu, int identity)
    {
        widget.createAndAdd(frame, menu, identity);
    };

    template <typename T>
    concept MenuItem = requires(T widget, wxFrame frame, wxMenu menu, int identity)
    {
        widget.createAndAdd(frame, menu, identity);
    };
    // clang-format on

}

namespace details {

    // We provide a way to bind either a function that takes in a commandEvent
    // not to a frame at a specific id
    using functionWithCmd_t = std::function<void(wxCommandEvent&)>;
    using functionWOCmd_t = std::function<void()>;
    using function_t = std::variant<functionWithCmd_t, functionWOCmd_t>;

    inline void bindToFrame(wxFrame& frame, int identity, function_t const& function)
    {
        std::visit([&frame, identity](auto const& funct) {
            using T = std::decay_t<decltype(funct)>;
            if constexpr (std::is_same_v<T, functionWithCmd_t>) {
                frame.Bind(wxEVT_MENU, funct, identity);
            } else if constexpr (std::is_same_v<T, functionWOCmd_t>) {
                frame.Bind(
                    wxEVT_MENU, [funct](wxCommandEvent&) { funct(); }, identity);
            } else {
                static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }
        },
            function);
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
        std::visit([&frame, &identity, appendFunction](auto const& item) {
            using T = std::decay_t<decltype(item)>;
            if constexpr (std::is_same_v<T, IDMenuDetails_t>) {
                appendFunction(std::get<0>(item), std::get<1>(item), std::get<2>(item));
            } else if constexpr (std::is_same_v<T, IDMenuDetailsWFunc_t>) {
                appendFunction(std::get<0>(item), std::get<1>(item), std::get<2>(item));
                bindToFrame(frame, std::get<0>(item), std::get<3>(item));
            } else if constexpr (std::is_same_v<T, NamedMenuDetails_t>) {
                appendFunction(identity, std::get<0>(item), std::get<1>(item));
                bindToFrame(frame, identity, std::get<2>(item));
                identity += 1;
            } else {
                static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }
        },
            item);
    }
}

struct Item {
    explicit Item(wxStandardID identity, std::string const& name = "", std::string const& helpString = "")
        : menuDetails(details::IDMenuDetails_t { identity, name, helpString })
    {
    }

    template <typename F>
    Item(wxStandardID identity, F function)
        : Item(identity, "", "", function)
    {
    }

    template <typename F>
    Item(wxStandardID identity, std::string const& name, F function)
        : Item(identity, name, "", function)
    {
    }

    template <typename F>
    Item(wxStandardID identity, std::string const& name, std::string const& helpString, F function)
        : menuDetails(details::IDMenuDetailsWFunc_t(identity, name, helpString, function))
    {
    }

    template <typename F>
    Item(std::string const& name, F function)
        : Item(name, "", function)
    {
    }

    template <typename F>
    Item(std::string const& name, std::string const& help, F function)
        : menuDetails(details::NamedMenuDetails_t(name, help, function))
    {
    }

    void createAndAdd(wxFrame& frame, wxMenu& menu, int& identity) const
    {
        details::createAndAdd(frame, menuDetails, identity, [&menu](int identity, wxString const& item, wxString const& helpString) {
            menu.Append(identity, item, helpString);
        });
    }
    details::MenuDetails menuDetails;
};

struct CheckItem {
    explicit CheckItem(wxStandardID identity, std::string const& name = "", std::string const& helpString = "")
        : menuDetails(details::IDMenuDetails_t { identity, name, helpString })
    {
    }

    template <typename F>
    CheckItem(wxStandardID identity, F function)
        : CheckItem(identity, "", "", function)
    {
    }

    template <typename F>
    CheckItem(wxStandardID identity, std::string const& name, F function)
        : CheckItem(identity, name, "", function)
    {
    }

    template <typename F>
    CheckItem(wxStandardID identity, std::string const& name, std::string const& helpString, F function)
        : menuDetails(details::IDMenuDetailsWFunc_t(identity, name, helpString, function))
    {
    }

    template <typename F>
    CheckItem(std::string const& name, F function)
        : CheckItem(name, "", function)
    {
    }

    template <typename F>
    CheckItem(std::string const& name, std::string const& help, F function)
        : menuDetails(details::NamedMenuDetails_t(name, help, function))
    {
    }

    void createAndAdd(wxFrame& frame, wxMenu& menu, int& identity) const
    {
        details::createAndAdd(frame, menuDetails, identity, [&menu](int identity, wxString const& item, wxString const& helpString) {
            menu.AppendCheckItem(identity, item, helpString);
        });
    }
    details::MenuDetails menuDetails;
};

struct RadioItem {
    explicit RadioItem(wxStandardID identity, std::string const& name = "", std::string const& helpString = "")
        : menuDetails(details::IDMenuDetails_t { identity, name, helpString })
    {
    }

    template <typename F>
    RadioItem(wxStandardID identity, F function)
        : RadioItem(identity, "", "", function)
    {
    }

    template <typename F>
    RadioItem(wxStandardID identity, std::string const& name, F function)
        : RadioItem(identity, name, "", function)
    {
    }

    template <typename F>
    RadioItem(wxStandardID identity, std::string const& name, std::string const& helpString, F function)
        : menuDetails(details::IDMenuDetailsWFunc_t(identity, name, helpString, function))
    {
    }

    template <typename F>
    RadioItem(std::string const& name, F function)
        : RadioItem(name, "", function)
    {
    }

    template <typename F>
    RadioItem(std::string const& name, std::string const& help, F function)
        : menuDetails(details::NamedMenuDetails_t(name, help, function))
    {
    }

    void createAndAdd(wxFrame& frame, wxMenu& menu, int& identity) const
    {
        details::createAndAdd(frame, menuDetails, identity, [&menu](int identity, wxString const& item, wxString const& helpString) {
            menu.AppendRadioItem(identity, item, helpString);
        });
    }
    details::MenuDetails menuDetails;
};

struct Separator {
    static void createAndAdd([[maybe_unused]] wxFrame& frame, wxMenu& menu, [[maybe_unused]] int& identity)
    {
        menu.AppendSeparator();
    }
};

// a submenu constructs menu to give to a menubar
template <details::MenuItem... M>
struct SubMenu {
    explicit SubMenu(std::string name, M const&... items)
        : SubMenu(std::move(name), std::make_tuple(items...))
    {
    }

    SubMenu(std::string name, std::tuple<M...> const& items)
        : name(std::move(name))
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

struct MenuProxy {
    [[nodiscard]] auto menu() const -> wxMenu*
    {
        if (mMenu == nullptr) {
            throw std::runtime_error("Proxy class has not been attached");
        }
        return mMenu;
    }

    void setUnderlying(wxMenu* control)
    {
        mMenu = control;
    }

    template <typename MenuType>
    auto operator=(MenuType&& menu) -> MenuType&& { return bind(std::forward<MenuType>(menu)); }
    template <typename MenuType>
    auto bind(MenuType&& widget) -> MenuType&&
    {
        widget.addProxyHandle(this);
        return std::forward<MenuType>(widget);
    }

private:
    wxMenu* mMenu {};
};

// a submenu constructs menu to give to a menubar
template <details::MenuItem... M>
struct Menu {
    explicit Menu(std::string name, M const&... items)
        : Menu(std::move(name), std::make_tuple(items...))
    {
    }

    Menu(std::string name, std::tuple<M...> const& items)
        : name(std::move(name))
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
        for (auto* proxy : proxyHandles) {
            proxy->setUnderlying(menu.get());
        }
        menuBar.Append(menu.release(), name);
    }

    void createAndAdd(wxFrame& frame, wxMenu& menuBar, int& identity)
    {
        auto menu = std::make_unique<wxMenu>();
        std::apply([&frame, menu = menu.get(), &identity](auto&&... tupleArg) {
            (tupleArg.createAndAdd(frame, *menu, identity), ...);
        },
            items);
        for (auto* proxy : proxyHandles) {
            proxy->setUnderlying(menu.get());
        }
        menuBar.AppendSubMenu(menu.release(), name);
    }

    void addProxyHandle(MenuProxy* proxy)
    {
        proxyHandles.push_back(proxy);
    }

private:
    std::string name;
    std::tuple<M...> items;
    // here's where we add proxies...?
    std::vector<MenuProxy*> proxyHandles;
};

template <details::MenuBarItem... M>
struct MenuBar {
    explicit MenuBar(M const&... menus)
        : MenuBar(std::make_tuple(menus...))
    {
    }
    explicit MenuBar(std::tuple<M...> const& menus)
        : menus(menus)
    {
    }

    auto attachTo(wxFrame* frame) -> auto&
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