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
#include <wx/spinctrl.h>

#include "HelperMacros.hpp"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_spin_ctrl.html
struct SpinCtrl : public details::WidgetDetails<SpinCtrl, wxSpinCtrl> {
    using super = details::WidgetDetails<SpinCtrl, wxSpinCtrl>;

    explicit SpinCtrl(std::optional<std::pair<int, int>> range = std::nullopt, std::optional<int> initial = std::nullopt)
        : SpinCtrl(wxID_ANY, range, initial)
    {
    }

    explicit SpinCtrl(wxWindowID identity, std::optional<std::pair<int, int>> range = std::nullopt, std::optional<int> initial = std::nullopt)
        : super(identity)
        , range_(std::move(range))
        , initial_(initial)
    {
    }

    using super::bind;
    template <typename Function>
    auto bind(Function func) & -> SpinCtrl&
    {
        return super::bind(wxEVT_SPINCTRL, func);
    }

    template <typename Function>
    auto bind(Function func) && -> SpinCtrl&&
    {
        return std::move(*this).super::bind(wxEVT_SPINCTRL, func);
    }

    struct Proxy : details::WidgetProxy<underlying_t> {
        PROXY_BOILERPLATE();

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

    RULE_OF_SIX_BOILERPLATE(SpinCtrl);

private:
    std::optional<std::pair<int, int>> range_;
    std::optional<int> initial_;

    auto createImpl(wxWindow* parent) -> wxWindow*
    {
        auto min = range_ ? range_->first : 0;
        auto max = range_ ? range_->second : 100;
        auto initvalue = initial_.value_or(min);
        return setProxy(new underlying_t(parent, getIdentity(), wxEmptyString, getPos(), getSize(), getStyle(), min, max, initvalue));
    }
};

WIDGET_STATIC_ASSERT_BOILERPLATE(SpinCtrl);
}

#include "ZapMacros.hpp"
