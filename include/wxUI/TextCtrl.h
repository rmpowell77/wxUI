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

#include <wx/textctrl.h>
#include <wxUI/Widget.h>

namespace wxUI {

struct TextCtrl : public details::WidgetDetails<TextCtrl> {
    using super = details::WidgetDetails<TextCtrl>;
    using underlying_t = wxTextCtrl;

    std::string text = "";

    TextCtrl(wxWindowID identity, std::string const& text = "")
        : super(identity)
        , text(text)
    {
    }

    TextCtrl(std::string const& text = "")
        : TextCtrl(wxID_ANY, text)
    {
    }

    TextCtrl(wxSizerFlags const& flags, wxWindowID identity, std::string const& text = "")
        : super(flags, identity)
        , text(text)
    {
    }

    TextCtrl(wxSizerFlags const& flags, std::string const& text = "")
        : TextCtrl(flags, wxID_ANY, text)
    {
    }

    wxWindow* create(wxWindow* parent) override
    {
        return new underlying_t(parent, this->identity, text, this->pos, this->size, this->usingStyle);
    }

    using super::createAndAdd;
};

}
