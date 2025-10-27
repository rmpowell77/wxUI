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
#include <type_traits>
#include <variant>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/wx.h>

namespace wxUI {

// Generic is for use in contexts where we are Creating and possibly Adding the
// object into a sizing hierachy.
template <typename Window = wxWindow>
struct Generic {
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

    auto create()
    {
        bindProxy(child_);
        return child_;
    }

    template <typename Parent>
    auto create([[maybe_unused]] Parent* parent)
    {
        return create();
    }

    // Provide a createAndAdd template so Generic is treated like other
    // CreateAndAddable items by the layout machinery. We keep the
    // implementation conservative: it calls create() (which for Generic
    // simply returns the underlying pointer) and then only attempts to
    // add the created window to the sizer if the customization
    // SizerAddController(sizer, window, flags) is well-formed for the
    // given Sizer/Window types. This avoids hard template-deduction
    // failures in mixed test/real contexts while preserving expected
    // behavior when the customization exists.
    template <typename Parent, typename Sizer>
    auto createAndAdd([[maybe_unused]] Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        auto* window = create(parent);
        // clang-format off
        if constexpr (requires(Sizer* s, Window* w, wxSizerFlags f) { s->Add(w, f); }) {
            // clang-format on
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

    auto withProxy(Proxy const& proxy) & -> Generic<Window>&
    {
        proxyHandles_.push_back(proxy);
        return *this;
    }

    auto withProxy(Proxy const& proxy) && -> Generic<Window>&&
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
