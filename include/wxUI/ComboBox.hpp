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

#include <wx/combobox.h>
#include <wxUI/GetterSetter.hpp>
#include <wxUI/Widget.hpp>

#include <wxUI/detail/HelperMacros.hpp>

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_combo_box.html
struct ComboBox {
    using underlying_t = wxComboBox;

    ComboBox(std::initializer_list<char const*> choices)
        : ComboBox(wxID_ANY, choices)
    {
    }

    ComboBox(std::initializer_list<std::initializer_list<char const*>> choices)
        : ComboBox(wxID_ANY, choices)
    {
    }

    ComboBox(std::initializer_list<std::string_view> choices)
        : ComboBox(wxID_ANY, choices)
    {
    }

    ComboBox(std::initializer_list<std::string> choices = {})
        : ComboBox(wxID_ANY, choices)
    {
    }

    explicit ComboBox(wxWindowID identity, std::initializer_list<char const*> choices)
        : details_(identity)
        , choices_(details::Ranges::convertToUtf8(choices))
    {
    }

    explicit ComboBox(wxWindowID identity, std::initializer_list<std::initializer_list<char const*>> choices)
        : details_(identity)
        , choices_(details::Ranges::flattenToUtf8(choices))
    {
    }

    explicit ComboBox(wxWindowID identity, std::initializer_list<std::string_view> choices)
        : details_(identity)
        , choices_(details::Ranges::convertToUtf8(choices))
    {
    }

    explicit ComboBox(wxWindowID identity, std::initializer_list<std::string> choices = {})
        : details_(identity)
        , choices_(details::Ranges::convertTo(choices))
    {
    }

    explicit ComboBox(details::Ranges::utf8_text_input_range auto&& choices)
        : ComboBox(wxID_ANY, std::forward<decltype(choices)>(choices))
    {
    }

    ComboBox(wxWindowID identity, details::Ranges::utf8_text_input_range auto&& choices)
        : details_(identity)
        , choices_(details::Ranges::ToVectorUtf8(std::forward<decltype(choices)>(choices)))
    {
    }

    auto withSelection(int which) & -> ComboBox&
    {
        selection_ = which;
        return *this;
    }

    auto withSelection(int which) && -> ComboBox&&
    {
        selection_ = which;
        return std::move(*this);
    }

    template <typename Function>
    auto bind(Function func) & -> ComboBox&
    {
        details_.bind(wxEVT_COMBOBOX, func);
        return *this;
    }

    template <typename Function>
    auto bind(Function func) && -> ComboBox&&
    {
        details_.bind(wxEVT_COMBOBOX, func);
        return std::move(*this);
    }

    struct Proxy : details::Proxy<underlying_t> {
        [[nodiscard]] auto value() const
        {
            auto* controller = control();
            return details::GetterSetter {
                [controller] { return controller->GetValue().utf8_string(); },
                [controller](std::string const& value) { controller->SetValue(wxString::FromUTF8(value)); }
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

private:
    details::WidgetDetails<ComboBox, wxComboBox> details_;
    std::vector<wxString> choices_;
    int selection_ = 0;

    template <typename Parent>
    auto createImpl()
    {
        return [&choices = choices_, selection = selection_](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            auto&& first = (choices.size() > 0) ? wxString(choices.at(0)) : wxString(wxEmptyString);
            auto* widget = customizations::ParentCreate<underlying_t>(parent, id, first, pos, size, static_cast<int>(choices.size()), choices.data(), style);
            if (!choices.empty()) {
                widget->SetSelection(selection);
            }
            return widget;
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(ComboBox)
};

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(ComboBox);
}

#include <wxUI/detail/ZapMacros.hpp>
