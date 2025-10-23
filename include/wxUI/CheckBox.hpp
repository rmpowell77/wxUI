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
#include <wx/checkbox.h>

#include "HelperMacros.hpp"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_check_box.html
struct CheckBox {
    using underlying_t = wxCheckBox;

    explicit CheckBox(std::string text = "")
        : CheckBox(wxID_ANY, std::move(text))
    {
    }

    explicit CheckBox(wxWindowID identity, std::string text = "")
        : details_(identity)
        , text_(std::move(text))
    {
    }

    auto withValue(bool value) & -> CheckBox&
    {
        value_ = value;
        return *this;
    }

    auto withValue(bool value) && -> CheckBox&&
    {
        value_ = value;
        return std::move(*this);
    }

    template <typename Function>
    auto bind(Function func) & -> CheckBox&
    {
        details_.bind(wxEVT_CHECKBOX, func);
        return *this;
    }

    template <typename Function>
    auto bind(Function func) && -> CheckBox&&
    {
        details_.bind(wxEVT_CHECKBOX, func);
        return std::move(*this);
    }

    struct Proxy : details::WidgetProxy<underlying_t> {
        [[nodiscard]] auto value() const
        {
            auto* controller = control();
            return details::GetterSetter {
                [controller] { return controller->GetValue(); },
                [controller](bool value) { controller->SetValue(value); }
            };
        }

        auto operator*() const { return value(); }
    };

private:
    details::WidgetDetails<CheckBox, wxCheckBox> details_;
    std::string text_;
    bool value_ = false;

    template <typename Parent>
    auto createImpl()
    {
        return [&text = text_, value = value_](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            auto* widget = customizations::ParentCreate<underlying_t>(parent, id, text, pos, size, style);
            using ::wxUI::customizations::ControllerSetValue;
            ControllerSetValue(widget, value);
            return widget;
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(CheckBox)
};

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(CheckBox);
}

#include "ZapMacros.hpp"
