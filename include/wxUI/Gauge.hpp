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

#include "GetterSetter.hpp"
#include "Widget.hpp"
#include <wx/gauge.h>

#include "HelperMacros.hpp"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_gauge.html
struct Gauge : public details::WidgetDetails<Gauge, wxGauge> {
    using super = details::WidgetDetails<Gauge, wxGauge>;

    // clang-format off
    struct withRange { };
    struct withIdentity { };
    // clang-format on

    explicit Gauge()
        : Gauge(withIdentity {}, wxID_ANY, 100)
    {
    }

    explicit Gauge([[maybe_unused]] withRange unused, int range)
        : Gauge(withIdentity {}, wxID_ANY, range)
    {
    }

    explicit Gauge([[maybe_unused]] withIdentity unused, wxWindowID identity, int range = 100)
        : super(identity, super::WithStyle { wxGA_HORIZONTAL })
        , range_(range)
    {
    }

    struct Proxy : details::WidgetProxy<underlying_t> {
        [[nodiscard]] auto value() const
        {
            auto* controller = control();
            return details::GetterSetter {
                [controller] { return static_cast<int>(controller->GetValue()); },
                [controller](int value) { controller->SetValue(value); }
            };
        }
        [[nodiscard]] auto range() const
        {
            auto* controller = control();
            return details::GetterSetter {
                [controller] { return static_cast<int>(controller->GetRange()); },
                [controller](int range) { controller->SetRange(range); }
            };
        }

        auto operator*() const { return value(); }
    };

    RULE_OF_SIX_BOILERPLATE(Gauge);

private:
    int range_;

    auto createImpl(wxWindow* parent) -> wxWindow* override
    {
        return bindProxy(new underlying_t(parent, getIdentity(), range_, getPos(), getSize(), getStyle()));
    }
};

WIDGET_STATIC_ASSERT_BOILERPLATE(Gauge);
}

#include "ZapMacros.hpp"
