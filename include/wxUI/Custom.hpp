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

namespace wxUI {

// A Custom Controller can only be created with something that supports the CreateAndAdd function.

// clang-format off
// snippet requires
template <typename T>
concept CreateAndAddFunction = requires(T function, wxWindow* window, wxSizer* sizer)
{
    function(window, sizer, wxSizerFlags {});
};
// endsnippet requires
// clang-format on

template <CreateAndAddFunction Function>
struct Custom {
    using Result = decltype(std::declval<Function>()(static_cast<wxWindow *>(nullptr), static_cast<wxSizer *>(nullptr), wxSizerFlags{}));
    using Window = std::conditional_t<std::same_as<Result, void>, wxWindow *, std::remove_pointer_t<Result>>;

    Custom(wxSizerFlags const& flags, Function const& function)
        : flags_(flags)
        , function_(function)
    {
    }

    explicit Custom(Function const& function)
        : function_(function)
    {
    }

    auto createAndAdd(wxWindow* parent, wxSizer* parentSizer, wxSizerFlags const& parentFlags) const
    {
        if constexpr (std::same_as<Result, void>) {
            return function_(parent, parentSizer, flags_.value_or(parentFlags));
        } else {
            auto child = function_(parent, parentSizer, flags_.value_or(parentFlags));

            bindProxy(child);

            return child;
        }
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

    auto withProxy(Proxy const& proxy) & -> Custom<Function>&
    {
        proxyHandles_.push_back(proxy);
        return *this;
    }

    auto withProxy(Proxy const& proxy) && -> Custom<Function>&&
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

    std::optional<wxSizerFlags> flags_;
    Function function_;
    std::vector<Proxy> proxyHandles_ {};
};

}

#include "ZapMacros.hpp"
