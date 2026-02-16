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
#include <stdexcept>
#include <type_traits>
#include <variant>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/wx.h>

namespace wxUI {

// Wrapper is for use where we are not creating the underlying widget, but still want to be able to bind events and use proxies.  This is useful for example when we want to wrap an entire wxDialog, or a wxSplitterWindow where the underlying widget is created by the controller itself and not by the caller.
template <typename Window = wxWindow>
struct Wrapper {
    struct Proxy;

    explicit Wrapper(Window* window)
        : child_(window)
    {
    }

    Wrapper(wxSizerFlags const& flags, Window* window)
        : flags_(flags)
        , child_(window)
    {
    }

    auto create()
    {
        bindProxy(child_);
        return child_;
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd([[maybe_unused]] Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        auto* window = create();
        if constexpr (requires(Sizer* sizer, Window* window, wxSizerFlags flags) { sizer->Add(window, flags); }) {
            parentSizer->Add(window, flags_.value_or(parentFlags));
        }
        return window;
    }

    struct Proxy {
        Proxy()
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

    auto withProxy(Proxy const& proxy) & -> Wrapper<Window>&
    {
        proxyHandles_.push_back(proxy);
        return *this;
    }

    auto withProxy(Proxy const& proxy) && -> Wrapper<Window>&&
    {
        proxyHandles_.push_back(proxy);
        return std::move(*this);
    }

private:
    auto bindProxy(Window* widget)
    {
        for (auto& proxyHandle : proxyHandles_) {
            using ::wxUI::customizations::ControllerBindProxy;
            ControllerBindProxy(widget, proxyHandle);
        }
        return widget;
    }

    std::optional<wxSizerFlags> flags_ {};
    Window* child_ {};
    std::vector<Proxy> proxyHandles_ {};
};
}

#include "ZapMacros.hpp"
