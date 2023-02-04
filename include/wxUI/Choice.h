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
#include <wx/choice.h>

namespace wxUI {

struct Choice : public details::WidgetDetails<Choice, wxChoice> {
    using super = details::WidgetDetails<Choice, wxChoice>;

    explicit Choice(wxWindowID identity, std::vector<wxString> choices = {})
        : super(identity)
        , choices(std::move(choices))
    {
    }

    explicit Choice(std::vector<wxString> choices = {})
        : Choice(wxID_ANY, std::move(choices))
    {
    }

    explicit Choice(wxSizerFlags const& flags, wxWindowID identity, std::vector<wxString> choices = {})
        : super(flags, identity)
        , choices(std::move(choices))
    {
    }

    explicit Choice(wxSizerFlags const& flags, std::vector<wxString> choices = {})
        : Choice(flags, wxID_ANY, std::move(choices))
    {
    }

    auto withSelection(int which) -> Choice&
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
        return details::BindWidgetToEvent { *this, wxEVT_CHOICE, func };
    }

    virtual ~Choice() = default;
    Choice(Choice const&) = default;
    Choice(Choice&&) = default;
    auto operator=(Choice const&) -> Choice& = default;
    auto operator=(Choice&&) -> Choice& = default;

private:
    std::vector<wxString> choices {};
    int selection {};
};

static_assert(details::Widget<Choice>);
}
