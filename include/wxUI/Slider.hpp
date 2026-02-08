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

#include "GetterSetter.hpp"
#include "Widget.hpp"
#include <wx/slider.h>

#include "HelperMacros.hpp"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_slider.html
struct Slider {
    using underlying_t = wxSlider;

    explicit Slider(std::optional<std::pair<int, int>> range = std::nullopt, std::optional<int> initial = std::nullopt)
        : Slider(wxID_ANY, range, initial)
    {
    }

    // Convenience overload to allow brace-init like Slider{{10,20}, 12}
    explicit Slider(std::pair<int, int> range, std::optional<int> initial = std::nullopt)
        : details_(wxID_ANY)
        , range_(range)
        , initial_(initial)
    {
    }

    explicit Slider(wxWindowID identity, std::optional<std::pair<int, int>> range = std::nullopt, std::optional<int> initial = std::nullopt)
        : details_(identity)
        , range_(std::move(range))
        , initial_(initial)
    {
    }

    // Convenience overload with explicit identity
    explicit Slider(wxWindowID identity, std::pair<int, int> range, std::optional<int> initial = std::nullopt)
        : details_(identity)
        , range_(range)
        , initial_(initial)
    {
    }

    template <typename Function>
    auto bind(Function func) & -> Slider&
    {
        details_.bind(wxEVT_SLIDER, func);
        return *this;
    }

    template <typename Function>
    auto bind(Function func) && -> Slider&&
    {
        details_.bind(wxEVT_SLIDER, func);
        return std::move(*this);
    }

    struct Proxy : details::Proxy<underlying_t> {
        [[nodiscard]] auto value() const
        {
            auto* controller = control();
            return details::GetterSetter {
                [controller] { return controller->GetValue(); },
                [controller](int value) { controller->SetValue(value); }
            };
        }

        auto operator*() const { return value(); }
    };

private:
    details::WidgetDetails<Slider, wxSlider> details_;
    std::optional<std::pair<int, int>> range_;
    std::optional<int> initial_;

    template <typename Parent>
    auto createImpl()
    {
        return [&range = range_, &initial = initial_](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            auto min = range ? range->first : 0;
            auto max = range ? range->second : 100;
            auto initvalue = initial.value_or(min);
            return customizations::ParentCreate<underlying_t>(parent, id, initvalue, min, max, pos, size, style);
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(Slider)
};

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(Slider);
}

#include "ZapMacros.hpp"
