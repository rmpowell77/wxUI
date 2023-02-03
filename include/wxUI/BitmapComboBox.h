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

#include "Widget.h"
#include <wx/bmpcbox.h>

namespace wxUI {

struct BitmapComboBox : public details::WidgetDetails<BitmapComboBox, wxBitmapComboBox> {
    using super = details::WidgetDetails<BitmapComboBox, wxBitmapComboBox>;

    BitmapComboBox(wxWindowID identity, std::vector<std::tuple<wxString, wxBitmap>> const& bitmapChoices)
        : super(identity)
        , choices([&bitmapChoices] {
            std::vector<wxString> result;
            result.reserve(bitmapChoices.size());
            for (auto&& [first, _] : bitmapChoices) {
                result.push_back(first);
            }
            return result;
        }())
        , bitmaps([&bitmapChoices] {
            std::vector<wxBitmap> result;
            result.reserve(bitmapChoices.size());
            for (auto&& [_, second] : bitmapChoices) {
                result.push_back(second);
            }
            return result;
        }())
    {
    }

    explicit BitmapComboBox(std::vector<std::tuple<wxString, wxBitmap>> const& bitmapChoices)
        : BitmapComboBox(wxID_ANY, bitmapChoices)
    {
    }

    BitmapComboBox(wxSizerFlags const& flags, wxWindowID identity, std::vector<std::tuple<wxString, wxBitmap>> const& bitmapChoices)
        : super(flags, identity)
        , choices([&bitmapChoices] {
            std::vector<wxString> result;
            result.reserve(bitmapChoices.size());
            for (auto&& [first, _] : bitmapChoices) {
                result.push_back(first);
            }
            return result;
        }())
        , bitmaps([&bitmapChoices] {
            std::vector<wxBitmap> result;
            result.reserve(bitmapChoices.size());
            for (auto&& [_, second] : bitmapChoices) {
                result.push_back(second);
            }
            return result;
        }())
    {
    }

    BitmapComboBox(wxSizerFlags const& flags, std::vector<std::tuple<wxString, wxBitmap>> const& bitmapChoices)
        : BitmapComboBox(flags, wxID_ANY, bitmapChoices)
    {
    }

    auto create(wxWindow* parent) -> wxWindow* override
    {
        auto&& first = (choices.size() > 0) ? wxString(choices.at(0)) : wxString(wxEmptyString);
        auto* widget = new underlying_t(parent, getIdentity(), first, getPos(), getSize(), static_cast<int>(choices.size()), choices.data(), getStyle());
        for (auto i = 0lu; i < bitmaps.size(); ++i) {
            widget->SetItemBitmap(i, bitmaps[i]);
        }
        widget->SetSelection(selection);
        return widget;
    }

    auto withSelection(int which) -> BitmapComboBox&
    {
        selection = which;
        return *this;
    }

    template <typename Function>
    auto bind(Function func)
    {
        return details::BindWidgetToEvent { *this, wxEVT_COMBOBOX, func };
    }

    virtual ~BitmapComboBox() = default;
    BitmapComboBox(BitmapComboBox const&) = default;
    BitmapComboBox(BitmapComboBox&&) = default;
    auto operator=(BitmapComboBox const&) -> BitmapComboBox& = default;
    auto operator=(BitmapComboBox&&) -> BitmapComboBox& = default;

private:
    std::vector<wxString> choices;
    std::vector<wxBitmap> bitmaps;
    int selection = 0;
};

static_assert(details::Widget<BitmapComboBox>);
}
