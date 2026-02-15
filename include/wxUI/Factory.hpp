/*
MIT License

Copyright (c) 2022-2026 Richard Powell

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
#include <concepts>
#include <stdexcept>
#include <type_traits>
#include <variant>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/wx.h>

namespace wxUI {

template <typename Window>
struct FactoryProxy {
    FactoryProxy()
        : windower(std::make_shared<Window*>())
    {
    }

    [[nodiscard]] auto window() const -> Window*
    {
        if (!windower) {
            throw std::runtime_error("Proxy class has not been attached");
        }
        return *windower;
    }

    auto operator->() const { return window(); }
    auto operator*() const { return window(); }

    void setUnderlying(Window* window)
    {
        *windower = window;
    }

private:
    std::shared_ptr<Window*> windower {};
};

template <typename Window = wxWindow, typename Function = std::function<Window*(wxWindow*)>>
struct Factory {
    using CreateWindowFunction = Function;
    using Proxy = FactoryProxy<Window>;

    template <typename Callable>
    requires std::constructible_from<Function, Callable>
    explicit Factory(Callable&& windowFunction)
        : closure_(std::forward<Callable>(windowFunction))
    {
    }

    template <typename Callable>
    requires std::constructible_from<Function, Callable>
    explicit Factory(wxSizerFlags const& flags, Callable&& windowFunction)
        : flags_(flags)
        , closure_(std::forward<Callable>(windowFunction))
    {
    }

    template <typename Parent>
    requires std::invocable<Function&, Parent*>
        && std::is_pointer_v<std::invoke_result_t<Function&, Parent*>>
    auto create(Parent* parent)
    {
        auto* child = closure_(parent);
        bindProxy(child);
        return child;
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        auto* window = create(parent);
        // clang-format off
        if constexpr (requires(Sizer* s, decltype(window) w, wxSizerFlags f) { s->Add(w, f); }) {
            // clang-format on
            parentSizer->Add(window, flags_.value_or(parentFlags));
        }
        return window;
    }

    auto withProxy(Proxy const& proxy) & -> Factory<Window, Function>&
    {
        proxyHandles_.push_back(proxy);
        return *this;
    }

    auto withProxy(Proxy const& proxy) && -> Factory<Window, Function>&&
    {
        proxyHandles_.push_back(proxy);
        return std::move(*this);
    }

private:
    template <typename Widget>
    auto bindProxy(Widget* widget)
    {
        for (auto& proxyHandle : proxyHandles_) {
            using ::wxUI::customizations::ControllerBindProxy;
            ControllerBindProxy(widget, proxyHandle);
        }
        return widget;
    }

    std::optional<wxSizerFlags> flags_ {};
    CreateWindowFunction closure_;
    std::vector<Proxy> proxyHandles_ {};
};

template <typename Function>
Factory(Function) -> Factory<std::remove_pointer_t<std::invoke_result_t<Function&, wxWindow*>>, std::decay_t<Function>>;

template <typename Function>
Factory(wxSizerFlags const&, Function) -> Factory<std::remove_pointer_t<std::invoke_result_t<Function&, wxWindow*>>, std::decay_t<Function>>;
}

#include "ZapMacros.hpp"
