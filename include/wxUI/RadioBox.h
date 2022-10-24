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

#include <wx/radiobox.h>
#include <wxUI/Widget.h>

namespace wxUI {

struct RadioBox : details::WidgetDetails<RadioBox> {
    using super = details::WidgetDetails<RadioBox>;
    using underlying_t = wxRadioBox;

    std::string text = "";
    std::vector<wxString> choices;
    int majorDim_ {};
    int selection {};

    RadioBox(wxWindowID identity, std::string const& text = "", std::vector<wxString> const& choices = {})
        : super(identity)
        , text(text)
        , choices(choices)
    {
        usingStyle = wxRA_SPECIFY_COLS;
    }

    RadioBox(wxWindowID identity, std::vector<wxString> const& choices = {})
        : RadioBox(identity, "", choices)
    {
    }

    RadioBox(std::string const& text, std::vector<wxString> const& choices = {})
        : RadioBox(wxID_ANY, text, choices)
    {
    }

    RadioBox(std::vector<wxString> const& choices = {})
        : RadioBox(wxID_ANY, "", choices)
    {
    }

    RadioBox(wxSizerFlags const& flags, wxWindowID identity, std::string const& text = "", std::vector<wxString> const& choices = {})
        : super(flags, identity)
        , text(text)
        , choices(choices)
    {
        usingStyle = wxRA_SPECIFY_COLS;
    }

    RadioBox(wxSizerFlags const& flags, wxWindowID identity, std::vector<wxString> const& choices = {})
        : RadioBox(flags, identity, "", choices)
    {
    }

    RadioBox(wxSizerFlags const& flags, std::string const& text, std::vector<wxString> const& choices = {})
        : RadioBox(flags, wxID_ANY, text, choices)
    {
    }

    RadioBox(wxSizerFlags const& flags, std::vector<wxString> const& choices = {})
        : RadioBox(flags, wxID_ANY, "", choices)
    {
    }

    auto& withSelection(int which)
    {
        selection = which;
        return *this;
    }

    auto& majorDim(int majorDim)
    {
        majorDim_ = majorDim;
        return *this;
    }

    wxWindow* create(wxWindow* parent) override
    {
        auto widget = new underlying_t(parent, this->identity, text, this->pos, this->size, choices.size(), choices.data(), majorDim_, this->usingStyle);
        widget->SetSelection(selection);
        return widget;
    }

    using super::createAndAdd;
};
}
