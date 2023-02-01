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
#include <wx/listbox.h>

namespace wxUI {

struct ListBox : public details::WidgetDetails<ListBox, wxListBox> {
    using super = details::WidgetDetails<ListBox, wxListBox>;

    std::vector<wxString> choices {};
    int selection = wxNOT_FOUND;

    explicit ListBox(wxWindowID identity, std::vector<wxString> choices = {})
        : super(identity)
        , choices(std::move(choices))
    {
    }

    explicit ListBox(std::vector<wxString> choices = {})
        : ListBox(wxID_ANY, std::move(choices))
    {
    }

    explicit ListBox(wxSizerFlags const& flags, wxWindowID identity, std::vector<wxString> choices = {})
        : super(flags, identity)
        , choices(std::move(choices))
    {
    }

    explicit ListBox(wxSizerFlags const& flags, std::vector<wxString> choices = {})
        : ListBox(flags, wxID_ANY, std::move(choices))
    {
    }

    auto withSelection(int which) -> ListBox&
    {
        selection = which;
        return *this;
    }

    auto create(wxWindow* parent) -> wxWindow* override
    {
        auto* widget = new underlying_t(parent, getIdentity(), getPos(), getSize(), static_cast<int>(choices.size()), choices.data(), getStyle());
        widget->SetSelection(selection);
        return widget;
    }

    template <typename Function>
    auto bind(Function func)
    {
        return details::BindWidgetToEvent { *this, wxEVT_LISTBOX, func };
    }

    virtual ~ListBox() = default;
    ListBox(ListBox const&) = default;
    ListBox(ListBox&&) = default;
    auto operator=(ListBox const&) -> ListBox& = default;
    auto operator=(ListBox&&) -> ListBox& = default;
};

static_assert(details::Widget<ListBox>);
}
