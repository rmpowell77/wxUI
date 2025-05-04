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
#include <ranges>
#include <wx/bmpcbox.h>

#include "HelperMacros.h"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_bitmap_combo_box.html
struct BitmapComboBox : public details::WidgetDetails<BitmapComboBox, wxBitmapComboBox> {
    using super = details::WidgetDetails<BitmapComboBox, wxBitmapComboBox>;

    explicit BitmapComboBox(std::initializer_list<std::tuple<std::string, wxBitmap>> bitmapChoices)
        : BitmapComboBox(wxID_ANY, bitmapChoices)
    {
    }

    BitmapComboBox(wxWindowID identity, std::initializer_list<std::tuple<std::string, wxBitmap>> bitmapChoices)
        : super(identity)
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

    BitmapComboBox(wxSizerFlags const& flags, std::initializer_list<std::tuple<std::string, wxBitmap>> bitmapChoices)
        : BitmapComboBox(flags, wxID_ANY, bitmapChoices)
    {
    }

    BitmapComboBox(wxSizerFlags const& flags, wxWindowID identity, std::initializer_list<std::tuple<std::string, wxBitmap>> bitmapChoices)
        : super(flags, identity)
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
        : super(identity)
        , choices_(details::Ranges::ToVector<wxString>(choices | std::views::transform([](auto&& item) { return std::get<0>(item); })))
        , bitmaps_(details::Ranges::ToVector<wxBitmap>(choices | std::views::transform([](auto&& item) { return std::get<1>(item); })))
    {
    }

    BitmapComboBox(wxSizerFlags const& flags, details::Ranges::input_range_of<std::tuple<wxString, wxBitmap>> auto&& choices)
        : BitmapComboBox(flags, wxID_ANY, std::forward<decltype(choices)>(choices))
    {
    }

    BitmapComboBox(wxSizerFlags const& flags, wxWindowID identity, details::Ranges::input_range_of<std::tuple<wxString, wxBitmap>> auto&& choices)
        : super(flags, identity)
        , choices_(details::Ranges::ToVector<wxString>(choices | std::views::transform([](auto&& item) { return std::get<0>(item); })))
        , bitmaps_(details::Ranges::ToVector<wxBitmap>(choices | std::views::transform([](auto&& item) { return std::get<1>(item); })))
    {
    }

    auto withSelection(int which) -> BitmapComboBox&
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

    struct Proxy : details::WidgetProxy<underlying_t> {
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

        auto operator*() const { return value(); }
    };
    RULE_OF_SIX_BOILERPLATE(BitmapComboBox);

private:
    std::vector<wxString> choices_;
    std::vector<wxBitmap> bitmaps_;
    int selection = 0;

    auto createImpl(wxWindow* parent) -> wxWindow* override
    {
        auto&& first = (choices_.size() > 0) ? wxString(choices_.at(0)) : wxString(wxEmptyString);
        auto* widget = setProxy(new underlying_t(parent, getIdentity(), first, getPos(), getSize(), static_cast<int>(choices_.size()), choices_.data(), getStyle()));
        for (auto i = 0lu; i < bitmaps_.size(); ++i) {
            widget->SetItemBitmap(i, bitmaps_[i]);
        }
        if (!choices_.empty()) {
            widget->SetSelection(selection);
        }
        return widget;
    }
};

WIDGET_STATIC_ASSERT_BOILERPLATE(BitmapComboBox);
}

#include "ZapMacros.h"
