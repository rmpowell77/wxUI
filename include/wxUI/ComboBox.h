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
#include <wx/combobox.h>

namespace wxUI {

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

    auto create(wxWindow* parent) -> wxWindow* override
    {
        auto&& first = (choices.size() > 0) ? wxString(choices.at(0)) : wxString(wxEmptyString);
        auto* widget = new underlying_t(parent, getIdentity(), first, getPos(), getSize(), static_cast<int>(choices.size()), choices.data(), getStyle());
        widget->SetSelection(selection);
        return widget;
    }

    auto withSelection(int which) -> ComboBox&
    {
        selection = which;
        return *this;
    }

    template <typename Function>
    auto bind(Function func)
    {
        return details::BindWidgetToEvent { *this, wxEVT_COMBOBOX, func };
    }

    virtual ~ComboBox() = default;
    ComboBox(ComboBox const&) = default;
    ComboBox(ComboBox&&) = default;
    auto operator=(ComboBox const&) -> ComboBox& = default;
    auto operator=(ComboBox&&) -> ComboBox& = default;

private:
    std::vector<wxString> choices;
    int selection = 0;
};

static_assert(details::Widget<ComboBox>);
}