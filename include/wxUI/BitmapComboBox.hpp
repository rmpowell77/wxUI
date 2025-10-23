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
#include <ranges>
#include <wx/bmpcbox.h>

#include "HelperMacros.hpp"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_bitmap_combo_box.html
struct BitmapComboBox {
    using underlying_t = wxBitmapComboBox;

    explicit BitmapComboBox(std::initializer_list<std::tuple<std::string, wxBitmap>> bitmapChoices)
        : BitmapComboBox(wxID_ANY, bitmapChoices)
    {
    }

    BitmapComboBox(wxWindowID identity, std::initializer_list<std::tuple<std::string, wxBitmap>> bitmapChoices)
        : details_(identity)
        , choices_([&bitmapChoices] {
            std::vector<wxString> result;
            result.reserve(bitmapChoices.size());
            for (auto&& [first, _] : bitmapChoices) {
                result.push_back(first);
            }
            return result;
        }())
        , bitmaps_([&bitmapChoices] {
            std::vector<wxBitmap> result;
            result.reserve(bitmapChoices.size());
            for (auto&& [_, second] : bitmapChoices) {
                result.push_back(second);
            }
            return result;
        }())
    {
    }

    explicit BitmapComboBox(details::Ranges::input_range_of<std::tuple<wxString, wxBitmap>> auto&& choices)
        : BitmapComboBox(wxID_ANY, std::forward<decltype(choices)>(choices))
    {
    }

    BitmapComboBox(wxWindowID identity, details::Ranges::input_range_of<std::tuple<wxString, wxBitmap>> auto&& choices)
        : details_(identity)
        , choices_(details::Ranges::ToVector<wxString>(choices | std::views::transform([](auto&& item) { return std::get<0>(item); })))
        , bitmaps_(details::Ranges::ToVector<wxBitmap>(choices | std::views::transform([](auto&& item) { return std::get<1>(item); })))
    {
    }

    auto withSelection(int which) & -> BitmapComboBox&
    {
        selection_ = which;
        return *this;
    }

    auto withSelection(int which) && -> BitmapComboBox&&
    {
        selection_ = which;
        return std::move(*this);
    }

    template <typename Function>
    auto bind(Function func) & -> BitmapComboBox&
    {
        details_.bind(wxEVT_COMBOBOX, func);
        return *this;
    }

    template <typename Function>
    auto bind(Function func) && -> BitmapComboBox&&
    {
        details_.bind(wxEVT_COMBOBOX, func);
        return std::move(*this);
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

private:
    details::WidgetDetails<BitmapComboBox, wxBitmapComboBox> details_;
    std::vector<wxString> choices_;
    std::vector<wxBitmap> bitmaps_;
    int selection_ = 0;

    template <typename Parent>
    auto createImpl()
    {
        return [&choices = choices_, &bitmaps = bitmaps_, selection = selection_](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            auto&& first = (choices.size() > 0) ? wxString(choices.at(0)) : wxString(wxEmptyString);
            auto* widget = customizations::ParentCreate<underlying_t>(parent, id, first, pos, size, static_cast<int>(choices.size()), choices.data(), style);

            for (auto i = 0lu; i < bitmaps.size(); ++i) {
                using ::wxUI::customizations::ControllerSetItemBitmap;
                ControllerSetItemBitmap(widget, i, bitmaps[i]);
            }
            if (!choices.empty()) {
                using ::wxUI::customizations::ControllerSetSelection;
                ControllerSetSelection(widget, selection);
            }
            return widget;
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(BitmapComboBox)
};

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(BitmapComboBox);
}

#include "ZapMacros.hpp"
