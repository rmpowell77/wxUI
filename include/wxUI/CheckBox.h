/*
MIT License

Copyright (c) 2022 Richard Powell

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

#include "GetterSetter.h"
#include "Widget.h"
#include <wx/checkbox.h>

#include "HelperMacros.h"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_check_box.html
struct CheckBox : public details::WidgetDetails<CheckBox, wxCheckBox> {
    using super = details::WidgetDetails<CheckBox, wxCheckBox>;

    explicit CheckBox(std::string text = "")
        : CheckBox(wxID_ANY, std::move(text))
    {
    }

    explicit CheckBox(wxWindowID identity, std::string text = "")
        : super(identity)
        , text(std::move(text))
    {
    }

    explicit CheckBox(wxSizerFlags const& flags, std::string text = "")
        : CheckBox(flags, wxID_ANY, std::move(text))
    {
    }

    CheckBox(wxSizerFlags const& flags, wxWindowID identity, std::string text = "")
        : super(flags, identity)
        , text(std::move(text))
    {
    }

    auto createImpl(wxWindow* parent) -> wxWindow* override
    {
        auto* widget = setProxy(new underlying_t(parent, getIdentity(), text, getPos(), getSize(), getStyle()));
        widget->SetValue(value);
        return widget;
    }

    auto withValue(bool value_) -> CheckBox&
    {
        value = value_;
        return *this;
    }

    using super::bind;
    template <typename Function>
    auto bind(Function func)
    {
        return super::bind(wxEVT_CHECKBOX, func);
    }

    struct Proxy : super::WidgetProxy {
        PROXY_BOILERPLATE();
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
    RULE_OF_SIX_BOILERPLATE(CheckBox);

private:
    std::string text;
    bool value = false;
};

WIDGET_STATIC_ASSERT_BOILERPLATE(CheckBox);
}

#include "ZapMacros.h"
