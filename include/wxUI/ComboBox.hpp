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
#include <wx/combobox.h>

#include "HelperMacros.hpp"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_combo_box.html
struct ComboBox : public details::WidgetDetails<ComboBox, wxComboBox> {
    using super = details::WidgetDetails<ComboBox, wxComboBox>;

    ComboBox(std::initializer_list<std::string> choices = {})
        : ComboBox(wxID_ANY, choices)
    {
    }

    explicit ComboBox(wxWindowID identity, std::initializer_list<std::string> choices = {})
        : super(identity)
        , choices_(details::Ranges::convertTo(choices))
    {
    }

    explicit ComboBox(details::Ranges::input_range_of<wxString> auto&& choices)
        : ComboBox(wxID_ANY, std::forward<decltype(choices)>(choices))
    {
    }

    ComboBox(wxWindowID identity, details::Ranges::input_range_of<wxString> auto&& choices)
        : super(identity)
        , choices_(details::Ranges::ToVector<wxString>(std::forward<decltype(choices)>(choices)))
    {
    }

    auto withSelection(int which) -> ComboBox&
    {
        selection_ = which;
        return *this;
    }

    using super::bind;
    template <typename Function>
    auto bind(Function func) & -> ComboBox&
    {
        return super::bind(wxEVT_COMBOBOX, func);
    }

    template <typename Function>
    auto bind(Function func) && -> ComboBox&&
    {
        return std::move(*this).super::bind(wxEVT_COMBOBOX, func);
    }

    struct Proxy : details::WidgetProxy<underlying_t> {
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

        auto operator*() const { return value(); }
    };
    RULE_OF_SIX_BOILERPLATE(ComboBox);

private:
    std::vector<wxString> choices_;
    int selection_ = 0;

    auto createImpl(wxWindow* parent) -> wxWindow* override
    {
        auto&& first = (choices_.size() > 0) ? wxString(choices_.at(0)) : wxString(wxEmptyString);
        auto* widget = bindProxy(new underlying_t(parent, getIdentity(), first, getPos(), getSize(), static_cast<int>(choices_.size()), choices_.data(), getStyle()));
        if (!choices_.empty()) {
            widget->SetSelection(selection_);
        }
        return widget;
    }
};

WIDGET_STATIC_ASSERT_BOILERPLATE(ComboBox);
}

#include "ZapMacros.hpp"
