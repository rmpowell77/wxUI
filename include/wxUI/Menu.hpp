/*
MIT License

Copyright (c) 2022-2025 Richard Powell

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

#include "Customizations.hpp"
#include "wxUITypes.hpp"
#include <functional>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <variant>
#include <wx/frame.h>
#include <wx/menu.h>

namespace wxUI::details {

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

template <typename F, typename Arg>
concept MenuForEachFunction = MenuItem<typename invoke_apply_result<F, Arg>::type>;

// We provide a way to bind either a function that takes in a commandEvent
// not to a frame at a specific id
using function_t = std::variant<std::function<void(wxCommandEvent&)>, std::function<void()>>;

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
template <typename Frame, typename AppendFunction>
inline void createAndAdd(Frame& frame, MenuDetails const& item, int& identity, AppendFunction appendFunction)
{
    using ::wxUI::customizations::MenuBindToFrame;
    std::visit([&frame, &identity, appendFunction](auto const& item) {
        using T = std::decay_t<decltype(item)>;
        if constexpr (std::is_same_v<T, IDMenuDetails_t>) {
            appendFunction(std::get<0>(item), std::get<1>(item), std::get<2>(item));
        } else if constexpr (std::is_same_v<T, IDMenuDetailsWFunc_t>) {
            appendFunction(std::get<0>(item), std::get<1>(item), std::get<2>(item));
            MenuBindToFrame(frame, std::get<0>(item), std::get<3>(item));
        } else if constexpr (std::is_same_v<T, NamedMenuDetails_t>) {
            appendFunction(identity, std::get<0>(item), std::get<1>(item));
            MenuBindToFrame(frame, identity, std::get<2>(item));
            identity += 1;
        } else {
            static_assert(always_false_v<T>, "non-exhaustive visitor!");
        }
    },
        item);
}

template <typename Underlying>
struct MenuProxy {
    MenuProxy()
        : controller(std::make_shared<Underlying*>())
    {
    }

    [[nodiscard]] auto control() const -> Underlying*
    {
        if (!controller) {
            throw std::runtime_error("Proxy class has not been attached");
        }
        return *controller;
    }

    void setUnderlying(Underlying* control)
    {
        *controller = control;
    }

    auto operator->() const { return control(); }

private:
    std::shared_ptr<Underlying*> controller {};
};
}

namespace wxUI {

using MenuProxy = details::MenuProxy<wxMenu>;
using MenuBarProxy = details::MenuProxy<wxMenuBar>;
using MenuItemProxy = details::MenuProxy<wxMenuItem>;

struct Item {
    explicit Item(wxStandardID identity, std::string const& name = "", std::string const& helpString = "")
        : menuDetails_(details::IDMenuDetails_t { identity, name, helpString })
    {
    }

    Item(wxStandardID identity, details::function_t function)
        : Item(identity, "", "", std::move(function))
    {
    }

    Item(wxStandardID identity, std::string const& name, details::function_t function)
        : Item(identity, name, "", std::move(function))
    {
    }

    Item(wxStandardID identity, std::string const& name, std::string const& helpString, details::function_t function)
        : menuDetails_(details::IDMenuDetailsWFunc_t(identity, name, helpString, std::move(function)))
    {
    }

    Item(std::string const& name, details::function_t function)
        : Item(name, "", std::move(function))
    {
    }

    Item(std::string const& name, std::string const& help, details::function_t function)
        : menuDetails_(details::NamedMenuDetails_t(name, help, std::move(function)))
    {
    }

    auto withProxy(MenuItemProxy const& proxy) & -> Item&
    {
        proxyHandles_.push_back(proxy);
        return *this;
    }

    auto withProxy(MenuItemProxy const& proxy) && -> Item&&
    {
        proxyHandles_.push_back(proxy);
        return std::move(*this);
    }

    template <typename Frame>
    void createAndAdd(Frame& frame, wxMenu& menu, int& identity)
    {
        details::createAndAdd(frame, menuDetails_, identity, [this, &menu](int identity, wxString const& item, wxString const& helpString) {
            auto* menuItem = menu.Append(identity, item, helpString);
            for (auto& proxyHandle : proxyHandles_) {
                proxyHandle.setUnderlying(menuItem);
            }
        });
    }

private:
    details::MenuDetails menuDetails_;
    std::vector<MenuItemProxy> proxyHandles_;
};

struct CheckItem {
    explicit CheckItem(wxStandardID identity, std::string const& name = "", std::string const& helpString = "")
        : menuDetails_(details::IDMenuDetails_t { identity, name, helpString })
    {
    }

    CheckItem(wxStandardID identity, details::function_t function)
        : CheckItem(identity, "", "", std::move(function))
    {
    }

    CheckItem(wxStandardID identity, std::string const& name, details::function_t function)
        : CheckItem(identity, name, "", std::move(function))
    {
    }

    CheckItem(wxStandardID identity, std::string const& name, std::string const& helpString, details::function_t function)
        : menuDetails_(details::IDMenuDetailsWFunc_t(identity, name, helpString, std::move(function)))
    {
    }

    CheckItem(std::string const& name, details::function_t function)
        : CheckItem(name, "", std::move(function))
    {
    }

    CheckItem(std::string const& name, std::string const& help, details::function_t function)
        : menuDetails_(details::NamedMenuDetails_t(name, help, std::move(function)))
    {
    }

    auto withProxy(MenuItemProxy const& proxy) & -> CheckItem&
    {
        proxyHandles_.push_back(proxy);
        return *this;
    }

    auto withProxy(MenuItemProxy const& proxy) && -> CheckItem&&
    {
        proxyHandles_.push_back(proxy);
        return std::move(*this);
    }

    template <typename Frame>
    void createAndAdd(Frame& frame, wxMenu& menu, int& identity)
    {
        details::createAndAdd(frame, menuDetails_, identity, [this, &menu](int identity, wxString const& item, wxString const& helpString) {
            auto* menuItem = menu.AppendCheckItem(identity, item, helpString);
            for (auto& proxyHandle : proxyHandles_) {
                proxyHandle.setUnderlying(menuItem);
            }
        });
    }

private:
    details::MenuDetails menuDetails_;
    std::vector<MenuItemProxy> proxyHandles_;
};

struct RadioItem {
    explicit RadioItem(wxStandardID identity, std::string const& name = "", std::string const& helpString = "")
        : menuDetails_(details::IDMenuDetails_t { identity, name, helpString })
    {
    }

    RadioItem(wxStandardID identity, details::function_t function)
        : RadioItem(identity, "", "", std::move(function))
    {
    }

    RadioItem(wxStandardID identity, std::string const& name, details::function_t function)
        : RadioItem(identity, name, "", std::move(function))
    {
    }

    RadioItem(wxStandardID identity, std::string const& name, std::string const& helpString, details::function_t function)
        : menuDetails_(details::IDMenuDetailsWFunc_t(identity, name, helpString, std::move(function)))
    {
    }

    RadioItem(std::string const& name, details::function_t function)
        : RadioItem(name, "", std::move(function))
    {
    }

    RadioItem(std::string const& name, std::string const& help, details::function_t function)
        : menuDetails_(details::NamedMenuDetails_t(name, help, std::move(function)))
    {
    }

    auto withProxy(MenuItemProxy const& proxy) & -> RadioItem&
    {
        proxyHandles_.push_back(proxy);
        return *this;
    }

    auto withProxy(MenuItemProxy const& proxy) && -> RadioItem&&
    {
        proxyHandles_.push_back(proxy);
        return std::move(*this);
    }

    template <typename Frame>
    void createAndAdd(Frame& frame, wxMenu& menu, int& identity)
    {
        details::createAndAdd(frame, menuDetails_, identity, [this, &menu](int identity, wxString const& item, wxString const& helpString) {
            auto* menuItem = menu.AppendRadioItem(identity, item, helpString);
            for (auto& proxyHandle : proxyHandles_) {
                proxyHandle.setUnderlying(menuItem);
            }
        });
    }

private:
    details::MenuDetails menuDetails_;
    std::vector<MenuItemProxy> proxyHandles_;
};

struct Separator {
    template <typename Frame>
    static void createAndAdd([[maybe_unused]] Frame& frame, wxMenu& menu, [[maybe_unused]] int& identity)
    {
        menu.AppendSeparator();
    }
};

// clang-format off
template <std::ranges::input_range Range, typename Function>
requires(details::MenuForEachFunction<Function, std::ranges::range_value_t<Range>>)
struct MenuForEach {
    // clang-format on
    MenuForEach(Range&& args, Function&& createFunction)
        : args_(std::forward<Range>(args))
        , createFunction_(std::forward<Function>(createFunction))
    {
    }

    template <typename Frame>
    void createAndAdd(Frame& frame, wxMenu& menu, int& identity)
    {
        using RawArg = std::remove_cvref_t<std::ranges::range_value_t<Range>>;
        for (auto&& item : args_) {
            if constexpr (details::CanApply<Function, RawArg>::value) {
                std::apply(createFunction_, item).createAndAdd(frame, menu, identity);
            } else {
                createFunction_(item).createAndAdd(frame, menu, identity);
            }
        }
    }

private:
    Range args_;
    Function createFunction_;
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

    auto withProxy(MenuProxy const& proxy) & -> Menu&
    {
        proxyHandles_.push_back(proxy);
        return *this;
    }

    auto withProxy(MenuProxy const& proxy) && -> Menu&&
    {
        proxyHandles_.push_back(proxy);
        return std::move(*this);
    }

    template <typename Frame>
    void createAndAdd(Frame& frame, wxMenuBar& menuBar, int& identity)
    {
        auto menu = std::make_unique<wxMenu>();
        std::apply([&frame, menu = menu.get(), &identity](auto&&... tupleArg) {
            (tupleArg.createAndAdd(frame, *menu, identity), ...);
        },
            items);
        for (auto& proxyHandle : proxyHandles_) {
            proxyHandle.setUnderlying(menu.get());
        }
        menuBar.Append(menu.release(), name);
    }

    template <typename Frame>
    void createAndAdd(Frame& frame, wxMenu& menu, int& identity)
    {
        auto subMenu = std::make_unique<wxMenu>();
        std::apply([&frame, subMenu = subMenu.get(), &identity](auto&&... tupleArg) {
            (tupleArg.createAndAdd(frame, *subMenu, identity), ...);
        },
            items);
        for (auto& proxyHandle : proxyHandles_) {
            proxyHandle.setUnderlying(subMenu.get());
        }
        menu.AppendSubMenu(subMenu.release(), name);
    }

private:
    std::string name;
    std::tuple<M...> items;
    std::vector<MenuProxy> proxyHandles_;
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

    auto withProxy(MenuBarProxy const& proxy) & -> MenuBar&
    {
        proxyHandles_.push_back(proxy);
        return *this;
    }

    auto withProxy(MenuBarProxy const& proxy) && -> MenuBar&&
    {
        proxyHandles_.push_back(proxy);
        return std::move(*this);
    }

    template <typename Frame>
    auto fitTo(Frame* frame) -> auto&
    {
        auto numbering = int(wxID_AUTO_LOWEST);
        auto menuBar = std::make_unique<wxMenuBar>();
        std::apply([frame, menuBar = menuBar.get(), &numbering](auto&&... tupleArg) {
            (tupleArg.createAndAdd(*frame, *menuBar, numbering), ...);
        },
            menus);
        for (auto& proxyHandle : proxyHandles_) {
            proxyHandle.setUnderlying(menuBar.get());
        }
        using ::wxUI::customizations::MenuSetMenuBar;
        MenuSetMenuBar(frame, menuBar.release());
        return *this;
    }

private:
    std::tuple<M...> menus;
    std::vector<MenuBarProxy> proxyHandles_;
};
}

#include "ZapMacros.hpp"
