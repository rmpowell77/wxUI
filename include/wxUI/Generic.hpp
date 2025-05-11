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

#include "Widget.hpp"
#include <stdexcept>
#include <variant>
#include <wx/sizer.h>
#include <wx/statbox.h>

namespace wxUI::details {
template <typename T>
// clang-format off
// Concept for a function takes a wxWindow* and returns something convertible to wxWindow*
concept CreateableWindowFunction = requires(T function, wxWindow* window) {
    { function(window) } -> std::convertible_to<wxWindow*>;
};
// clang-format on

}

namespace wxUI {

// Generic is for use in contexts where we are Creating and possibly Adding the
// object into a sizing hierachy.
template <typename Window = wxWindow>
struct Generic {
    using CreateWindowFunction = std::function<Window*(wxWindow*)>;

    struct Proxy;

    explicit Generic(Window* window)
        : child_(window)
    {
    }

    Generic(wxSizerFlags const& flags, Window* window)
        : flags_(flags)
        , child_(window)
    {
    }

    template <details::CreateableWindowFunction Function>
    Generic(Function&& windowFunction)
        : child_(std::forward<Function>(windowFunction))
    {
    }

    template <details::CreateableWindowFunction Function>
    Generic(wxSizerFlags const& flags, Function&& windowFunction)
        : flags_(flags)
        , child_(std::forward<Function>(windowFunction))
    {
    }

    void bindProxy(Proxy* proxy)
    {
        proxy_ = proxy;
    }

    auto create(wxWindow* parent) const -> Window*;
    void createAndAdd(wxWindow* parent, wxSizer* parentSizer, wxSizerFlags const& parentFlags) const;

    struct Proxy {
        Proxy() = default;

        auto operator=(Generic&& controller) -> Generic&&
        {
            return bind(std::forward<Generic>(controller));
        }
        template <typename Generic>
        auto bind(Generic&& widget) -> Generic&&
        {
            widget.bindProxy(this);
            return std::forward<Generic>(widget);
        }

        [[nodiscard]] auto window() const -> Window*
        {
            if (windower == nullptr) {
                throw std::runtime_error("Proxy class has not been attached");
            }
            return windower;
        }

        auto operator->() const { return window(); }
        auto operator*() const { return window(); }

        void setUnderlying(Window* window)
        {
            windower = window;
        }

    private:
        Window* windower {};
    };

private:
    std::optional<wxSizerFlags> flags_ {};
    std::variant<Window*, CreateWindowFunction> child_;
    Proxy* proxy_ {};
};

template <typename Window>
inline auto Generic<Window>::create(wxWindow* parent) const -> Window*
{
    auto* window = std::visit(details::overloaded {
                                  [parent](CreateWindowFunction const& arg) {
                                      return arg(parent);
                                  },
                                  [](auto arg) {
                                      return arg;
                                  },
                              },
        child_);
    if (proxy_) {
        proxy_->setUnderlying(window);
    }
    return window;
}

template <typename Window>
inline void Generic<Window>::createAndAdd(wxWindow* parent, wxSizer* parentSizer, wxSizerFlags const& parentFlags) const
{
    auto* window = create(parent);
    parentSizer->Add(window, flags_.value_or(parentFlags));
}

}

#include "ZapMacros.hpp"
