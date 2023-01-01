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
#include <wx/radiobox.h>

namespace wxUI {

struct RadioBox : details::WidgetDetails<RadioBox, wxRadioBox> {
    using super = details::WidgetDetails<RadioBox, wxRadioBox>;

    std::string text;
    std::vector<wxString> choices;
    int majorDim_ {};
    int selection {};

    explicit RadioBox(wxWindowID identity, std::string text = "", std::vector<wxString> choices = {})
        : super(identity)
        , text(std::move(text))
        , choices(std::move(choices))
    {
        usingStyle = wxRA_SPECIFY_COLS;
    }

    RadioBox(wxWindowID identity, std::vector<wxString> choices)
        : RadioBox(identity, "", std::move(choices))
    {
    }

    RadioBox(std::string text, std::vector<wxString> choices)
        : RadioBox(wxID_ANY, std::move(text), std::move(choices))
    {
    }

    explicit RadioBox(std::vector<wxString> choices)
        : RadioBox(wxID_ANY, "", std::move(choices))
    {
    }

    RadioBox(wxSizerFlags const& flags, wxWindowID identity, std::string text, std::vector<wxString> choices)
        : super(flags, identity)
        , text(std::move(text))
        , choices(std::move(choices))
    {
        usingStyle = wxRA_SPECIFY_COLS;
    }

    RadioBox(wxSizerFlags const& flags, wxWindowID identity, std::vector<wxString> choices)
        : RadioBox(flags, identity, "", std::move(choices))
    {
    }

    RadioBox(wxSizerFlags const& flags, std::string text, std::vector<wxString> choices)
        : RadioBox(flags, wxID_ANY, std::move(text), std::move(choices))
    {
    }

    RadioBox(wxSizerFlags const& flags, std::vector<wxString> choices)
        : RadioBox(flags, wxID_ANY, "", std::move(choices))
    {
    }

    auto withSelection(int which) -> RadioBox&
    {
        selection = which;
        return *this;
    }

    auto majorDim(int majorDim) -> RadioBox&
    {
        majorDim_ = majorDim;
        return *this;
    }

    auto create(wxWindow* parent) -> wxWindow* override
    {
        auto* widget = new underlying_t(parent, this->identity, text, this->pos, this->size, static_cast<int>(choices.size()), choices.data(), majorDim_, this->usingStyle);
        widget->SetSelection(selection);
        return widget;
    }

    virtual ~RadioBox() = default;
    RadioBox(RadioBox const&) = default;
    RadioBox(RadioBox&&) = default;
    auto operator=(RadioBox const&) -> RadioBox& = default;
    auto operator=(RadioBox&&) -> RadioBox& = default;
};

static_assert(details::Widget<RadioBox>);
}
