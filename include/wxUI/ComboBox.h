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
#include <wx/combobox.h>

#include "HelperMacros.h"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_combo_box.html
struct ComboBox : public details::WidgetDetails<ComboBox, wxComboBox> {
    using super = details::WidgetDetails<ComboBox, wxComboBox>;

    ComboBox(wxWindowID identity, std::vector<wxString> const& choices)
        : super(identity)
        , choices(choices)
    {
    }

    explicit ComboBox(std::vector<wxString> const& choices)
        : ComboBox(wxID_ANY, choices)
    {
    }

    ComboBox(wxSizerFlags const& flags, wxWindowID identity, std::vector<wxString> const& choices)
        : super(flags, identity)
        , choices(choices)
    {
    }

    ComboBox(wxSizerFlags const& flags, std::vector<wxString> const& choices)
        : ComboBox(flags, wxID_ANY, choices)
    {
    }

    auto createImpl(wxWindow* parent) -> wxWindow* override
    {
        auto&& first = (choices.size() > 0) ? wxString(choices.at(0)) : wxString(wxEmptyString);
        auto* widget = setProxy(new underlying_t(parent, getIdentity(), first, getPos(), getSize(), static_cast<int>(choices.size()), choices.data(), getStyle()));
        widget->SetSelection(selection);
        return widget;
    }

    auto withSelection(int which) -> ComboBox&
    {
        selection = which;
        return *this;
    }

    using super::bind;
    template <typename Function>
    auto bind(Function func)
    {
        return super::bind(wxEVT_COMBOBOX, func);
    }

    struct Proxy : super::WidgetProxy {
        PROXY_BOILERPLATE();
        [[nodiscard]] auto value() const
        {
            auto* controller = control();
            return details::GetterSetter {
                [controller] { return static_cast<std::string>(controller->GetValue()); },
                [controller](std::string value) { controller->SetValue(value); }
            };
        }
        [[nodiscard]] auto selection() const
        {
            auto* controller = control();
            return details::GetterSetter {
                [controller] { return controller->GetSelection(); },
                [controller](int selection) { controller->SetSelection(selection); }
            };
        }

        auto operator->() const { return value(); }
        auto operator*() const { return value(); }
    };
    RULE_OF_SIX_BOILERPLATE(ComboBox);

private:
    std::vector<wxString> choices;
    int selection = 0;
};

WIDGET_STATIC_ASSERT_BOILERPLATE(ComboBox);
}

#include "ZapMacros.h"
